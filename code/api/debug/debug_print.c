// **************************************************************************
// debug_print.c
// 
// Implementation debug print
//
// Copyright (c) ITE, All Rights Reserved
//
// **************************************************************************
#include "debug_print.h"
#include "chip_type.h"
#include "chip_chipregs.h"
#include <stdarg.h>

#ifndef _STDIO_H_ // {
/* **************************************************************************
** TABLE
** radix_table
**
** DESCRIPTION
** for fast radix transform.
 ************************************************************************** */
const char radix_table[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#define	utoa(dest, num, base)	__ltoa__(dest, num, base, 0)
#define	dtoa(dest, num, base)	__ltoa__(dest, num, base, CONVERT_SIGN)

/* **************************************************************************
** conversions
 ************************************************************************** */
#define	CONVERT_SIGN                (1 << 0)
#define	CONVERT_SIGN_FORCED         (1 << 1)
#define	CONVERT_LONG                (1 << 2)
#define	CONVERT_LONGLONG            (1 << 3)
#define	CONVERT_LEADING_ZERO        (1 << 4)
#define	CONVERT_LEFT_ADJUST         (1 << 5)
#define	CONVERT_START               (1 << 15)

/* **************************************************************************
 *
  ************************************************************************** */
typedef	union
{
    INT64   i64;
    INT32   i32;
    UINT64  u64;
    UINT32  u32;
    char    *p;
} t_num;

/* **************************************************************************
** FUNCTION
** xtoa(unsigned long x, char *s, int n)
**
** DESCRIPTION
 ************************************************************************** */
void xtoa(unsigned long x, char *s, int n)
{
  const char *p = radix_table;
  n *= 4;
  while (n>0)
  {
    n -= 4;
    *s++ = p[(x>>n)&0x0f];
  }
  *s = '\0';
}
/* **************************************************************************
** FUNCTION
** __ltoa16__
**
** DESCRIPTION
** Transform a number into base-16 string.
 ************************************************************************** */
static char * __ltoa16__(char *dest, t_num *X, unsigned conversion, int width)
{
    char      *src, *src2;
    UINT32    num;

    /* check long */
    if (conversion&CONVERT_LONGLONG)
        num = X->u64;
    else
        num = X->u32;

    /* generating bytes. */
    src = dest;         /* save first pos   */
    do {
        int frac;
        frac     = (int)(num&0x0f);
        num      = num>>4;
        src2     = dest;
        *dest++  = radix_table [ frac ];
        width--;
    } while (num);

    /* Now fill leadings (or trailing) ZEROs or SPACEs */
    {
        char c = ((conversion&(CONVERT_LEADING_ZERO|CONVERT_LEFT_ADJUST))==CONVERT_LEADING_ZERO) ? '0' : ' ';
        while (width>0)
        {
            if ((conversion&CONVERT_LEFT_ADJUST)==0)
            {
                src2 = dest;
            }
            *dest++ = c;
            width--;
        }
    }

    /* reverse bytes within (src src2) */
    while (src<src2)
    {
        char c;
        c     = *src2;
        *src2 = *src;
        *src  = c;
        src++;
        src2--;
    }

    return dest;
}
/* **************************************************************************
** FUNCTION
** __ltoa__
**
** DESCRIPTION
** transform a number in base specified
 ************************************************************************** */
static char * __ltoa__(char *dest, t_num *X, unsigned base, int conversion, int width)
{
  char	*src, *src2;

  // check long

  if (conversion&CONVERT_LONGLONG)
  {
    UINT64	num = X->u64;
    // conversion sign
    if (conversion&CONVERT_SIGN)
    {
      if (((INT64)num)<0)
      {
        num = (UINT64)(-(INT64)num);
        *dest++ = '-';
        width--;
      }
    }
    // generating bytes.
    src = dest;
    do {
      int frac;
      frac     = num%base;
      num      = num/base;
      src2     = dest;
      *dest++  = radix_table [ frac ];
      width--;
    } while (num);
  }
  else
  {
    UINT32	num = X->u32;
    /* conversion sign */
    if (conversion&CONVERT_SIGN)
    {
      if (((INT32)num)<0)
      {
        num = (UINT32)(-(INT32)num);
        *dest++ = '-';
        width--;
      }
    }
    /* generating bytes. */
    src = dest;
    do {
      int frac;
      frac     = num%base;
      num      = num/base;
      src2     = dest;
      *dest++  = radix_table [ frac ];
      width--;
    } while (num);
  }

  /* Now fill leadings (or trailing) ZEROs or SPACEs */
  {
    char c = ((conversion&(CONVERT_LEADING_ZERO|CONVERT_LEFT_ADJUST))==CONVERT_LEADING_ZERO) ? '0' : ' ';
    while (width>0)
    {
      if ((conversion&CONVERT_LEFT_ADJUST)==0)
        src2 = dest;
      *dest++ = c;
      width--;
    }
  }

  // reverse bytes within (src src2)
  while (src<src2)
  {
    char c;
    c     = *src2;
    *src2 = *src;
    *src  = c;
    src++;
    src2--;
  }

  return dest;
}
//***************************************************************************
int sio_vprintf(char *dest, const char *format, void* ap)
{	int		width		= 0;
	unsigned	conversion	= 0;
	while (1)
	{
		int c = *format++;
		*dest = c;
		if (c=='\0') break;
		if (conversion & CONVERT_START)
		{
			if (c>='0' && c<='9')
			{
				width = width*10 + (c-'0');
				if (width==0)
				conversion |= CONVERT_LEADING_ZERO;
			}
			else if (c=='l')
			{
				if (conversion & CONVERT_LONG) conversion |= CONVERT_LONGLONG;
				conversion |= CONVERT_LONG;
			}
			else if (c=='+')
			{
				conversion |= CONVERT_SIGN_FORCED;
			}
			else if (c=='-')
			{
				conversion = (conversion & (~CONVERT_LEADING_ZERO)) | CONVERT_LEFT_ADJUST;
			}
			else if (c=='%')
			{
				*dest++ = '%';
				conversion = 0;
			}
			else
			{
				t_num	X;
				if (conversion&CONVERT_LONGLONG)
					X.u64 = va_arg((va_list)ap,UINT64);
				else
					X.u32 = va_arg((va_list)ap,UINT32);
				switch(c)
				{
					case 'd':	dest = __ltoa__(dest,&X,10,conversion|CONVERT_SIGN,width); break;
					case 'u':	dest = __ltoa__(dest,&X,10,conversion,width); break;
					case 'x':	dest = __ltoa16__(dest,&X,conversion,width); break;
					case 'X':   dest = __ltoa16__(dest,&X,conversion,width); break;
					case 's':
						{
							char *src = (char *)X.u32;
							while (*src)
							*dest++ = *src++;
						}
						break;
					case 'c':	*dest++ = (char) X.u32; break;
				}
				/* conversion finished */
				conversion = 0;
			}
		}
		else if (c=='%')
		{
			conversion	= CONVERT_START;
			width		= 0;
		}
		else
		{
			dest++;
		}
	}
	return 0;
}

// ****************************************************************************************
// Function name: rtl_sprintf( void)
// 
// Description: 
//   rtl_sprintf Function. 
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note: 
//   
// ****************************************************************************************
int rtl_sprintf(char *buf, const char *format, ...)
{
   	va_list valist;
	
	va_start(valist, format);
	sio_vprintf(buf, format, valist);
	va_end(valist);
	
    return  0;
}
#else
void nds_write(const unsigned char *buf, int size)
{
    while((size--) > 0)    
    {
        putchar(*buf++);
    }
}

inline int putchar(int c)
{
    debug_putc((unsigned char)c);
	
    return c;
}
#endif // } _STDIO_H_

// ****************************************************************************************
// Function name: debug_puts( void)
// 
// Description: 
//   debug_puts Function. 
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note: 
//   
// ****************************************************************************************
#ifdef  __ENABLE_DBG_MSG__  // {
void debug_puts (char *s)
{
	while(*s != '\0')
	{
		debug_putc(*s++);
	}
}

// **************************************************************************
// Function name: DLDumpMem
//
// Description:
//      Dump memory data
// Arguments:
//      Addr: data memory pointer
//      Len : totla memory length
// Return Values:
//
// Note:
//      
// **************************************************************************
void DLDumpMem(unsigned char* Addr, unsigned long Len)
{
    unsigned long i;    

    printk("DLDumpMem:: Addr = 0x%X. Len = 0x%X Bytes.\r\n", (unsigned long)Addr, Len);
    
    printk("===============================================================\r\n");
    for(i = 0; i < Len; i++)
    {
        if((i & 0xf) == 0)
        {            
            printk("0x%08X:  ", (unsigned long)Addr);
        }
        
        printk("%02X", *Addr);        
        
        if(((i & 0xf) == 0xf)||(i == Len-1))
        {
            printk("\r\n");
        }
        else if((i&0xf) == 0x7)
        {
            printk("    ");
        }
        else
            printk(" ");
        
        Addr++;
    }
    printk("===============================================================\r\n");
}
// ****************************************************************************************
// Function name: DEBUG_PORT( void)
// 
// Description: 
//   DEBUG_PORT Function. 
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note: 
//   
// ****************************************************************************************
void DEBUG_PORT(unsigned char u8Data)
{
    GPDRA = u8Data;
}

// ****************************************************************************************
// Function name: rtl_printf( void)
// 
// Description: 
//   rtl_printf Function. 
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note: 
//   
// ****************************************************************************************
int rtl_printf(const char *format, ...)
{
    char tmpBuf[0x100];
	va_list valist;

	va_start(valist, format);
    #ifndef _STDIO_H_ // {
	sio_vprintf(tmpBuf, format, valist);
    #else
	vsprintf(tmpBuf, format, valist);
    #endif // } _STDIO_H_
	va_end(valist);
	debug_puts(tmpBuf);

	return 0;
}
#endif  // } __ENABLE_DBG_MSG__
// ****************************************************************************************
void InitDebugPrint(void)
{
    #ifdef  __ENABLE_DBG_MSG__  // {
    //  Set Debug Port
    GPCRA0  = 0x40;//OUTPUT;
    GPCRA1  = 0x40;//OUTPUT;
    GPCRA2  = 0x40;//OUTPUT;
    GPCRA3  = 0x40;//OUTPUT;
    GPCRA4  = 0x40;//OUTPUT;
    GPCRA5  = 0x40;//OUTPUT;
    GPCRA6  = 0x40;//OUTPUT;
    GPCRA7  = 0x40;//OUTPUT;
    GPDRA   = 0x00;
    
    if(3 == (ECHIPVER&0xF))
    {
        GCR24 &= (~(BIT1|BIT0));    // 3.3V
    }
    #endif  // } __ENABLE_DBG_MSG__

    //  Set Uart
    Uart_Init();
}
// ****************************************************************************************
