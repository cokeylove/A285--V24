/*************************************************************************
* Name:        lz.c
* Author:      Marcus Geelnard
* Description: LZ77 coder/decoder implementation.
* Reentrant:   Yes
*
* The LZ77 compression scheme is a substitutional compression scheme
* proposed by Abraham Lempel and Jakob Ziv in 1977. It is very simple in
* its design, and uses no fancy bit level compression.
*
* This is my first attempt at an implementation of a LZ77 code/decoder.
*
* The principle of the LZ77 compression algorithm is to store repeated
* occurrences of strings as references to previous occurrences of the same
* string. The point is that the reference consumes less space than the
* string itself, provided that the string is long enough (in this
* implementation, the string has to be at least 4 bytes long, since the
* minimum coded reference is 3 bytes long). Also note that the term
* "string" refers to any kind of byte sequence (it does not have to be
* an ASCII string, for instance).
*
* The coder uses a brute force approach to finding string matches in the
* history buffer (or "sliding window", if you wish), which is very, very
* slow. I recon the complexity is somewhere between O(n^2) and O(n^3),
* depending on the input data.
*
* There is also a faster implementation that uses a large working buffer
* in which a "jump table" is stored, which is used to quickly find
* possible string matches (see the source code for LZ_CompressFast() for
* more information). The faster method is an order of magnitude faster,
* but still quite slow compared to other compression methods.
*
* The upside is that decompression is very fast, and the compression ratio
* is often very good.
*
* The reference to a string is coded as a (length,offset) pair, where the
* length indicates the length of the string, and the offset gives the
* offset from the current data position. To distinguish between string
* references and literal strings (uncompressed bytes), a string reference
* is preceded by a marker byte, which is chosen as the least common byte
* symbol in the input data stream (this marker byte is stored in the
* output stream as the first byte).
*
* Occurrences of the marker byte in the stream are encoded as the marker
* byte followed by a zero byte, which means that occurrences of the marker
* byte have to be coded with two bytes.
*
* The lengths and offsets are coded in a variable length fashion, allowing
* values of any magnitude (up to 4294967295 in this implementation).
*
* With this compression scheme, the worst case compression result is
* (257/256)*insize + 1.
*
*-------------------------------------------------------------------------
* Copyright (c) 2003-2006 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* Marcus Geelnard
* marcus.geelnard at home.se
*************************************************************************/
#include "osc.h"
#include "mem_layout.h"
#include "chip_type.h"
#include "chip_chipregs.h"

#include "rom.h"
#include "debug_print.h"


//#define __ENABLE_OSC_ZIP__

#ifdef __ENABLE_OSC_ZIP__  // { 
/*************************************************************************
* Constants used for LZ77 coding
*************************************************************************/

/* Maximum offset (can be any size < 2^31). Lower values give faster
   compression, while higher values gives better compression. The default
   value of 100000 is quite high. Experiment to see what works best for
   you. */

TSegHeader gpstSegHeader[SEG_HEADER_SIZE];
UINT8      gucParsingHeaderFlag = 0;
UINT8      gucHeaderSegNum  = 0;
UINT8      gucTargetSeg;

/*************************************************************************
* _LZ_ReadVarSize() - Read unsigned integer with variable number of
* bytes depending on value.
*************************************************************************/
static int _LZ_ReadVarSize( unsigned int * x, unsigned char * buf )
{
    unsigned int y, b, num_bytes;

    /* Read complete value (stop when byte contains zero in 8:th bit) */
    y = 0;
    num_bytes = 0;
    do
    {
        b = (unsigned int) (*buf ++);
        y = (y << 7) | (b & 0x0000007f);
        ++ num_bytes;
    }
    while( b & 0x00000080 );

    /* Store value in x */
    *x = y;

    /* Return number of bytes read */
    return num_bytes;
}

/*************************************************************************
*                            PUBLIC FUNCTIONS                            *
*************************************************************************/

/*************************************************************************
* LZ_Uncompress() - Uncompress a block of data using an LZ77 decoder.
*  in      - Input (compressed) buffer.
*  out     - Output (uncompressed) buffer. This buffer must be large
*            enough to hold the uncompressed data.
*  insize  - Number of input bytes.
*************************************************************************/
void LZ_Uncompress( unsigned char *in, unsigned char *out,
    unsigned int insize )
{
    unsigned char marker, symbol;
    unsigned int  i, inpos, outpos, length, offset;

    /* Do we have anything to uncompress? */
    if( insize < 1 )
    {
        return;
    }

    /* Get marker symbol from input stream */
    marker = in[ 0 ];
    inpos = 1;

    /* Main decompression loop */
    outpos = 0;
    do
    {
        symbol = in[ inpos ++ ];
        if( symbol == marker )
        {
            /* We had a marker byte */
            if( in[ inpos ] == 0 )
            {
                /* It was a single occurrence of the marker byte */
                out[ outpos ++ ] = marker;
                ++ inpos;
            }
            else
            {
                /* Extract true length and offset */
                inpos += _LZ_ReadVarSize( &length, &in[ inpos ] );
                inpos += _LZ_ReadVarSize( &offset, &in[ inpos ] );

                /* Copy corresponding data from history window */
                for( i = 0; i < length; ++ i )
                {
                    out[ outpos ] = out[ outpos - offset ];
                    ++ outpos;
                }
            }
        }
        else
        {
            /* No marker, plain copy */
            out[ outpos ++ ] = symbol;
        }
    }
    while( inpos < insize );
}

void ite_lz_parsing_header(void)
{
	UINT32 ulZipSize = ECReg(0x10);
	UINT8 *pSrcData;
	UINT8 *pDstData = (UINT8*)gpstSegHeader;
	volatile int i= 0x1000;

	//RTL_PRINTF("gucHeaderSegNum: %X %X %X\r\n",gucHeaderSegNum,ulZipSize,REG_UL(0x7F000));
	// Enable SPI DMA INTC
	ECReg(REG_BASE_ADDR + 0x1173) = 	(ECReg(REG_BASE_ADDR + 0x1173) | 0x04); // Enable interrupt
	eflash_to_direct_map((ulZipSize &0xFFFFF000),5);

	dm_set_segment_valid(5);
	while(i--);

	gucHeaderSegNum = ECReg((ulZipSize - 1));
	//RTL_PRINTF("gucHeaderSegNum: %X %X %X\r\n",gucHeaderSegNum,ulZipSize,REG_UL(0x7F000));
	pSrcData = (UINT8*)((ulZipSize - 1) - (gucHeaderSegNum * sizeof(TSegHeader)));

	for(i = 0; i< (gucHeaderSegNum * sizeof(TSegHeader)); i++)
	{
		*(pDstData + i) = *(pSrcData + i);
	}
	/*for(i = 0 ;i < gucHeaderSegNum; i++)
	{
		RTL_PRINTF("Seg[%X]-> Add: %X, Len: %X, zip: %X\r\n",i,gpstSegHeader[i].ulStartAdd,gpstSegHeader[i].ulUnzipSize,gpstSegHeader[i].ulZipStartAdd);
	}*/
	//GPDRA = (UINT8)*(pDstData + 0x09);
	//GPDRA = (UINT8)(gpstSegHeader[1].ulStartAdd >> 8);

	//DebugHex(gpstSegHeader[1].ulStartAdd);
	gucParsingHeaderFlag = 1;
	dm_set_segment_invalid(5);

}

void ite_load_zip_code(UINT32 ulOSCAddr)
{
	int i;
	UINT32 ulZipStartAdd;

	//RTL_PRINTF("ite_load_zip_code: %X\r\n",ulOSCAddr);
	for(i = 0 ; i < gucHeaderSegNum; i++)
	{
	    if((ulOSCAddr >= (gpstSegHeader[i].ulStartAdd & 0x0FFFFFFF)) && (ulOSCAddr < (gpstSegHeader[i].ulStartAdd & 0x0FFFFFFF )+ gpstSegHeader[i].ulUnzipSize))
	    {
	    	gucTargetSeg = i;

	    }
	}
	//RTL_PRINTF("Tag: %X\r\n",gucTargetSeg);
	ulZipStartAdd = gpstSegHeader[gucTargetSeg].ulZipStartAdd;
	eflash_to_direct_map(ulZipStartAdd, 10);

}

void ite_lz_decompress(void)
{
	UINT8 *pSrcData = (UINT8*)(GET_DATA_VMA_ADDR + 0x2000);
	UINT8 *pDstData = (UINT8*)(GET_DATA_VMA_ADDR + 0x3000);
	UINT32 ulZipSize = 0;
	//UINT32 *pTmp = (UINT32*)pDstData;
	//int i;

	//RTL_PRINTF("ite_lz_decompress\r\n");
	//GPDRA = gucTargetSeg;
	ulZipSize = gpstSegHeader[gucTargetSeg].ulZipSize;

	LZ_Uncompress( pSrcData, pDstData, ulZipSize );

	dm_set_vma_address((gpstSegHeader[gucTargetSeg].ulStartAdd& 0x0FFFFFFF),11);
    dm_set_segment_valid(11);
}

#endif // } __ENABLE_OSC_ZIP__

void OSC_CODE do_osc_flow(unsigned long ulipc)
{
    #ifdef __ENABLE_TEST_ZIP_OSC__    	
    // Load code to SRAM06
    ite_load_zip_code(ulipc);
    MCCR |= 0x0C;
    ite_lz_decompress();
    MCCR &= 0xF3;
    //DEBUG_PORT(0xdd);
	#endif  // __ENABLE_TEST_ZIP_OSC__
}
