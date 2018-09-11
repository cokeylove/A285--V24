#ifndef _osc_h_
#define _osc_h_

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************
* Name:        lz.h
* Author:      Marcus Geelnard
* Description: LZ77 coder/decoder interface.
* Reentrant:   Yes
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

/*************************************************************************
* Macro Define
*************************************************************************/
#define SEG_HEADER_SIZE         (10)

/*************************************************************************
* Function prototypes
*************************************************************************/
typedef struct
{
    unsigned long ulStartAdd;
    unsigned long ulZipStartAdd;
    unsigned long ulUnzipSize;
    unsigned long ulZipSize;
}_SEG_HEADER_;
typedef _SEG_HEADER_ TSegHeader;

/*************************************************************************
* Function prototypes
*************************************************************************/
extern void do_osc_flow(unsigned long ulipc);

//*************************************************************************
#ifdef __cplusplus
}
#endif
#endif /* _osc_h_ */
