// *****************************************************************************
//
//
//
// *****************************************************************************
#include "rom.h"
#include "chip_type.h"
#include "chip_chipregs.h"

#include "nds32_regs.h"
#include "nds32_defs.h"
#include <nds32_intrinsic.h>

//******************************************************************************
//
//******************************************************************************
// Scratch SRAM 5 Address
#define	R_SCRA5L	(EC_Register_Base_Address+0x1081)   // Low Byte Register
#define	R_SCRA5M	(EC_Register_Base_Address+0x1082)	// Middle Byte Register
#define	R_SCRA5H	(EC_Register_Base_Address+0x1083)	// High Byte Register

// Scratch SRAM 0 Address
#define	R_SCRA0L	(EC_Register_Base_Address+0x1040)	// Low Byte Register
#define	R_SCRA0M	(EC_Register_Base_Address+0x1041)	// Middle Byte Register
#define	R_SCRA0H	(EC_Register_Base_Address+0x1042)	// High Byte Register

//******************************************************************************
#define SET_BIT_FIELD(reg, mask, val)   \
    ECReg((reg)) = ((ECReg((reg)) & ~(mask)) | ((val) & (mask)))

//******************************************************************************
#define DM_DBG_L(fmt, arg...)		//printf(fmt, ##arg)
#define DM_DBG_H(fmt, arg...)		printf(fmt, ##arg)

// *****************************************************************************
//  Prototype:
//      void dm_set_vma_address(unsigned long Addr, unsigned char Index);
//
//  Description:
//      Set VMA address of the direct map segment.
//  
//  Parameters:
//      Addr: Virtual address.(CPU view)
//
//      Index: SRAM segment index (0~12).
//
//  Return Value:
//      None.
// *****************************************************************************
void dm_set_vma_address(UINT32 u32Address, UINT8 u8SRAMIndex)
{
	if(u8SRAMIndex>4)
	{
        u8SRAMIndex -= 5;
        SET_BIT_FIELD((R_SCRA5H+0x3*u8SRAMIndex), 0x07, 
                      (UINT8)((u32Address & 0x00FF0000) >> 16));
        
		ECReg(R_SCRA5L+0x3*u8SRAMIndex) 
            = (UINT8)((u32Address & 0x000000FF) >> 0 );
        
		ECReg(R_SCRA5M+0x3*u8SRAMIndex) 
            = (UINT8)((u32Address & 0x0000FF00) >> 8 );        
	}
	else
	{
        SET_BIT_FIELD((R_SCRA0H+0x3*u8SRAMIndex), 0x07, 
                      (UINT8)((u32Address & 0x00FF0000) >> 16));
        
		ECReg(R_SCRA0L+0x3*u8SRAMIndex) 
            = (UINT8)((u32Address & 0x000000FF) >> 0 );
        
		ECReg(R_SCRA0M+0x3*u8SRAMIndex) 
            = (UINT8)((u32Address & 0x0000FF00) >> 8 );
	}
}

// *****************************************************************************
//  Prototype:
//      void dm_set_segment_valid(unsigned char Index);
//
//  Description:
//      Enable direct map segment.
//  
//  Parameters:
//      Index: SRAM segment index (0~12).
//
//  Return Value:
//      None.
// *****************************************************************************
void dm_set_segment_valid(UINT8 u8SRAMIndex)
{
	// Valid VMA
	if(u8SRAMIndex>4)
	{
        u8SRAMIndex -= 5;
		ECReg(R_SCRA5H+0x3*u8SRAMIndex) &= ~BIT3;
	}
	else
	{
		ECReg(R_SCRA0H+0x3*u8SRAMIndex) &= ~BIT3;
	}
}

// *****************************************************************************
//  Prototype:
//      void dm_set_segment_invalid(unsigned char Index)
//
//  Description:
//      Disable direct map segment.
//  
//  Parameters:
//      Index: SRAM segment index (0~12).
//
//  Return Value:
//      None.
// *****************************************************************************
void dm_set_segment_invalid(UINT8 u8SRAMIndex)
{
	// Valid VMA
	if(u8SRAMIndex>4)
	{
        u8SRAMIndex -= 5;
		ECReg(R_SCRA5H+0x3*u8SRAMIndex) |= BIT3;
	}
	else
	{
		ECReg(R_SCRA0H+0x3*u8SRAMIndex) |= BIT3;
	}
}



// ==========================================================================
/**
// Function name: dm_get_dma_map()
// Description:
//   i2c dma address mapping
// Arguments:
//   UINT32 addr: Ram address.
//
// @return Values:
//   UINT16 : DMA address
//
// Note:
//   none
*/
// ==========================================================================
UINT16 dm_get_dma_map( UINT32 addr )
{
	//0x80000 -> 0xd000
	//0x81000 -> 0xe000
	//0x82000 -> 0xa000
	//0x83000 -> 0xb000
	//0x84000 -> 0x8000
	//0x85000 -> 0x9000
	//0x86000 -> 0x3000
	//0x87000 -> 0x4000
	//0x88000 -> 0x5000
	//0x89000 -> 0x6000
	//0x8A000 -> 0x7000
	//0x8B000 -> 0x1000
	//0x8C000 -> 0xC000
	//0x8D000 -> 0x0000
	//0x8E000 -> 0x2000
	//I2CERR_DBG("i2c_dma_map test byte:%x,%x\r\n",tmp,addr);

	if ( ( 0x80000 <= addr ) && ( addr < 0x81000 ) )
		return ( 0xd000 + ( addr & 0x0FFF ) );
	else if ( ( 0x81000 <= addr ) && ( addr < 0x82000 ) )
		return ( 0xe000 + ( addr & 0x0FFF ) );
	else if ( ( 0x82000 <= addr ) && ( addr < 0x83000 ) )
		return ( 0xa000 + ( addr & 0x0FFF ) );
	else if ( ( 0x83000 <= addr ) && ( addr < 0x84000 ) )
		return ( 0xb000 + ( addr & 0x0FFF ) );
    else if ( ( 0x84000 <= addr ) && ( addr < 0x85000 ) )
		return ( 0x8000 + ( addr & 0x0FFF ) );  
     else if ( ( 0x85000 <= addr ) && ( addr < 0x86000 ) )
		return ( 0x9000 + ( addr & 0x0FFF ) );            
	else if ( ( 0x86000 <= addr ) && ( addr < 0x87000 ) )
		return ( 0x3000 + ( addr & 0x0FFF ) );
	else if ( ( 0x87000 <= addr ) && ( addr < 0x88000 ) )
		return ( 0x4000 + ( addr & 0x0FFF ) );
    else if ( ( 0x88000 <= addr ) && ( addr < 0x89000 ) )
		return ( 0x5000 + ( addr & 0x0FFF ) ); 
    else if ( ( 0x89000 <= addr ) && ( addr < 0x8A000 ) )
		return ( 0x6000 + ( addr & 0x0FFF ) ); 
    else if ( ( 0x8A000 <= addr ) && ( addr < 0x8B000 ) )
		return ( 0x7000 + ( addr & 0x0FFF ) ); 
    else if ( ( 0x8B000 <= addr ) && ( addr < 0x8C000 ) )
		return ( 0x1000 + ( addr & 0x0FFF ) );  
    else if ( ( 0x8C000 <= addr ) && ( addr < 0x8D000 ) )
		return ( 0xC000 + ( addr & 0x0FFF ) ); 
    else if ( ( 0x8D000 <= addr ) && ( addr < 0x8E000 ) )
		return ( 0x0000 + ( addr & 0x0FFF ) ); 
    else if ( ( 0x8E000 <= addr ) && ( addr < 0x8F000 ) )
		return ( 0x2000 + ( addr & 0x0FFF ) );      
	else {
		DM_DBG_L("i2c_dma_map no map:%x\r\n",addr);
		return ( 0 );
	}

}

#if 0 // {
//******************************************************************************
//  Prototype:
//      void immu_cache_dma_burst_length_select(unsigned char Param)
//
//  Description:
//      Select IMMU cache DMA Burst Length.
//  
//  Parameters:
//      Param: 
//          0: 16 bytes.
//          1: 32 bytes.
//
//  Return Value:
//      None.
//******************************************************************************
void immu_cache_dma_burst_length_select(UINT8 u8Param)
{
    unsigned long ulGIE = ( GET_PSW() & (PSW_mskINTL|PSW_mskGIE) );
	
    if(PSW_mskGIE == ulGIE)
    {
        __nds32__gie_dis();
    }
    //---------------------------------------
    //  Enter Critical Section
    //---------------------------------------
    if(u8Param)
        MCCR |= BIT1;  // 32 bytes
    else
        MCCR &= ~BIT1; // 16 bytes
	
    immu_reset_ext();
    //---------------------------------------
    //  Exit Critical Section
    //---------------------------------------    
    if(PSW_mskGIE == ulGIE)
    {
        __nds32__gie_en();
    }
}

//******************************************************************************
//  Prototype:
//      void immu_cache_size_select(unsigned char Param)
//
//  Description:
//      Select IMMU cache size.
//  
//  Parameters:
//      Param: 
//          0: 8K bytes.
//          1: 4K bytes.
//
//  Return Value:
//      None.
//******************************************************************************
void immu_cache_size_select(UINT8 u8Param)
{
    unsigned long ulGIE = ( GET_PSW() & (PSW_mskINTL|PSW_mskGIE) );

    if(PSW_mskGIE == ulGIE)
    {
        __nds32__gie_dis();
    }
    //---------------------------------------
    //  Enter Critical Section
    //---------------------------------------
    if(u8Param)
        MCCR |= BIT0;  // 4K bytes
    else
        MCCR &= ~BIT0; // 8K bytes

    immu_reset_ext();
    //---------------------------------------
    //  Exit Critical Section
    //---------------------------------------
    if(PSW_mskGIE == ulGIE)
    {
        __nds32__gie_en();
    }
}

//******************************************************************************
// 
//******************************************************************************
void dlm_config(UINT8 u8Index, BOOL bEnable)
{
    switch(u8Index)
	{
    	case 0:
    	case 1:
    		break;            

        // 8k~16k
        case 2:
        case 3:
            if(bEnable)
                MCCR |= (BIT2 << (u8Index - 2)); 
            else
                MCCR &= ~(BIT2 << (u8Index - 2)); 
        	break;

        // 16k~36k
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if(bEnable)
                MCCR1 |= (BIT2 << (u8Index - 4));
            else
                MCCR1 &= ~(BIT2 << (u8Index - 4));
        	break;
            
        // 36k~60k
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            if(bEnable)
            {
                if(u8Index == 12)
                    immu_cache_size_select(1); // immu cache -> 4K
                    
                MCCR2 |= (BIT0 << (u8Index - 9)); 
            }
            else
                MCCR2 &= ~(BIT0 << (u8Index - 9)); 
        	break;        
	}
    
    EIDSR |= 0x40; // External Data Local Memory Size: 64k byte
    immu_reset_ext();
}
#endif // } 0

// *****************************************************************************