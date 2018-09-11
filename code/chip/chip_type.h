/* 
 * ****************************************************************************
 * chip_type.h
 * ****************************************************************************
 */

#ifndef CHIP_TYPE_H_
#define CHIP_TYPE_H_

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define BIT(X)	(1 << (X))

#define BIT0 	0x00000001
#define BIT1 	0x00000002
#define BIT2 	0x00000004
#define BIT3   	0x00000008
#define BIT4   	0x00000010
#define BIT5  	0x00000020
#define BIT6   	0x00000040
#define BIT7 	0x00000080

#define BIT8 	0x00000100
#define BIT9 	0x00000200
#define BIT10 	0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13  	0x00002000
#define BIT14   0x00004000
#define BIT15 	0x00008000

#define BIT16 	0x00010000
#define BIT17	0x00020000
#define BIT18 	0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21  	0x00200000
#define BIT22   0x00400000
#define BIT23 	0x00800000

#define BIT24 	0x01000000
#define BIT25 	0x02000000
#define BIT26 	0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29  	0x20000000
#define BIT30   0x40000000
#define BIT31 	0x80000000

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
/*
 * TRUE can be defined as !FALSE but defining
 * it as 1 allows it to fit into a bitfield.
 */
#define TRUE    1
#endif

#define MASK_bit0 (1 << 0)
#define MASK_bit1 (1 << 1)
#define MASK_bit2 (1 << 2)
#define MASK_bit3 (1 << 3)
#define MASK_bit4 (1 << 4)
#define MASK_bit5 (1 << 5)
#define MASK_bit6 (1 << 6)
#define MASK_bit7 (1 << 7)
#define CONCAT(a, b) a ## b
#define MASK(x) CONCAT(MASK_, x)

/* 
 * ****************************************************************************
 * Define memory data type.
 * ****************************************************************************
 */

/* with volatile */
typedef volatile unsigned char uchar_8;
typedef volatile unsigned short uint_16;
typedef volatile unsigned long ulong_32;

/* no volatile */
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

/* For const table at ILM */
typedef unsigned char CBYTE;
typedef unsigned short CWORD;
typedef unsigned long CDWORD;

typedef volatile unsigned char FLAG;

typedef void* HANDLE;
typedef unsigned char BOOL;

typedef signed char SINT8;
typedef signed short SINT16;
typedef signed long SINT32;
typedef signed int SINT;

typedef SINT8* LPSINT8;
typedef SINT16* LPSINT16;
typedef SINT32* LPSINT32;
typedef SINT* LPSINT;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef unsigned long long UINT64;
typedef unsigned int UINT;

typedef UINT8* LPUINT8;
typedef UINT16* LPUINT16;
typedef UINT32* LPUINT32;
typedef UINT* LPUINT;

typedef	char INT8;
typedef	short INT16;
typedef	long INT32;
typedef long long INT64;

/* Macros for hardware registers acess. */
#define ECReg(x) (*((volatile unsigned char *)(x)))
#define ECReg32(x) (*((volatile unsigned long *)(x)))

/* Macros for SRAM data acess. */
#define SD_uchar_8(x) (*((unsigned char *)(x)))
#define SD_uint_16(x) (*((unsigned short *)(x)))
#define SD_ulong_32(x) (*((unsigned long *)(x)))

#define SD_Ptr_uchar_8(x) ((unsigned char *)(x))
#define SD_Ptr_uint_16(x) ((unsigned short *)(x))
#define SD_Ptr_ulong_32(x) ((unsigned long *)(x))

/* MASK operation macros */
#define SET_MASK(reg, bit_mask) 	    ((reg)|=(bit_mask))
#define CLEAR_MASK(reg, bit_mask) 	    ((reg)&=(~(bit_mask)))
#define IS_MASK_SET(reg, bit_mask) 	    (((reg)&(bit_mask))!=0)
#define IS_MASK_CLEAR(reg, bit_mask)    (((reg)&(bit_mask))==0)
#define INVERSE_REG(reg, bit)           ((reg)^=(0x1<<(bit)))
#define INVERSE_REG_MASK(reg, bit_mask) ((reg)^=(bit_mask))

/*
 * Name: Byte_Mask, Word_Mask, Cpl_Byte_Mask, Cpl_Word_Mask
 *
 * Used to create a one-bit mask from a bit number.
 *
 * C prototypes:   BYTE Byte_Mask(BYTE bit_number);
 *                 WORD Word_Mask(BYTE bit_number);
 *                 BYTE Cpl_Byte_Mask(BYTE bit_number);
 *                 WORD Cpl_Word_Mask(BYTE bit_number);
 */
#define Byte_Mask(x)        (1 << (x))
#define Word_Mask(x)        (1 << (x))
#define Cpl_Byte_Mask(x)    (~(1 << (x)))
#define Cpl_Word_Mask(x)    (~(1 << (x)))

/* */
#define COUNT_OF_ARRAY(p)   (sizeof(p)/sizeof(p[0]))

/* FIELD is used when defining bit-fields within a structure (or union). */
#define FIELD   unsigned char

typedef volatile union _BITS_8
{
    unsigned char byte;

    struct
    {
        FIELD bit0: 1;
        FIELD bit1: 1;
        FIELD bit2: 1;
        FIELD bit3: 1;
        FIELD bit4: 1;
        FIELD bit5: 1;
        FIELD bit6: 1;
        FIELD bit7: 1;
    } fbit;
}BITS_8;

typedef volatile union _BITS_32
{
    unsigned long lword;

    struct
    {
        FIELD bit0: 1;
        FIELD bit1: 1;
        FIELD bit2: 1;
        FIELD bit3: 1;
        FIELD bit4: 1;
        FIELD bit5: 1;
        FIELD bit6: 1;
        FIELD bit7: 1;
        FIELD bit8: 1;
        FIELD bit9: 1;
        FIELD bit10: 1;
        FIELD bit11: 1;
        FIELD bit12: 1;
        FIELD bit13: 1;
        FIELD bit14: 1;
        FIELD bit15: 1;
        FIELD bit16: 1;
        FIELD bit17: 1;
        FIELD bit18: 1;
        FIELD bit19: 1;
        FIELD bit20: 1;
        FIELD bit21: 1;
        FIELD bit22: 1;
        FIELD bit23: 1;     
        FIELD bit24: 1;
        FIELD bit25: 1;
        FIELD bit26: 1;
        FIELD bit27: 1; 
        FIELD bit28: 1;
        FIELD bit29: 1;
        FIELD bit30: 1;
        FIELD bit31: 1; 
    } fbit;
}BITS_32;

typedef volatile union _TIMER
{
    unsigned char byte;
    struct
    {
        FIELD count: 4; /* bit[0:3] */
        FIELD bit4:  1;
        FIELD bit5:  1;
        FIELD bit6:  1;
        FIELD bit7:  1;
    } fbit;
}TIMER;

typedef volatile union Unew_keyl
{
    unsigned char byte;
    struct
    {
        FIELD state: 1; /* Bit[0]   1 means debounce counted out. */
        FIELD trans: 1; /* Bit[1]   1 for BREAK. 0 for MAKE. */
        FIELD intm:  1; /* Bit[2] 	*/
        FIELD ghost: 1; /* Bit[3]   Ghost key indicator. */
        FIELD same:  1; /*
                         * Bit[4]
                         * 1 = key activity last time was SAME as this time.
                         */
        FIELD count: 3; /* Bit[5:7] Debounce counter. */
    }field;
}UUnew_keyl;

typedef volatile union KEY
{
   unsigned char byte;
   struct
   {
      FIELD input:            3; /* Bit 0 - 2. Bit address of the KI bit. */
      FIELD debounce_status:  1; /* Bit 3.     Debounce status. */
      FIELD output:           4; /* Bit 4 - 7. Bit address of the KO bit. */
   } field;
}uKEY;

typedef volatile struct Sscan
{
    FIELD kbf_head:  4; /* Keyboard buffer head (0-15). */
    FIELD kbf_tail:  4; /* Keyboard buffer tail (0-15). */
    FIELD kbf_mark:  4; /* Keyboard buffer overflow marker (0-15). */
    FIELD TMscale:   4; /* Typematic scale. */
    FIELD scan_line: 4;
    FIELD saf_make:  1; /* Scan activity flag - make. */
    FIELD saf_break: 1; /* Scan activity flag - break. */
    FIELD saf_keys:  1; /* Scan activity flag - keys active. */
}sSscan;

typedef volatile union _key_state
{
    unsigned char byte;
    struct
    {
       FIELD tNUM_LOCK:  1;  /* Bit[0] */
       FIELD tLEFT:      1;  /* Bit[1] */
       FIELD tRIGHT:     1;  /* Bit[2] */
       FIELD tALT:       1;  /* Bit[3] */
       FIELD tCONTROL:   1;  /* Bit[4] */
       FIELD tOVERLAY:   1;  /* Bit[5] */
       FIELD tFN:        1;  /* Bit[6] */
       FIELD bit7: 		 1;  /* Bit[7] */
    } field;
}key_state;

typedef volatile struct _INT_VAR
{
    FIELD Active_Aux_Port:      2;  /* bit[0:1] */
    FIELD Ticker_10:            4;  /* bit[2:5] */
    FIELD Low_Power_Mode:       1;  /* bit[6] */
    FIELD Scan_Lock:            1;  /* bit[7] */
    FIELD SMB_Service_Request:  1;  /* bit[8] */
 	FIELD Ticker_100:           7;  /* bit[9:15] or bit[8:14] */
}INT_VAR;

typedef volatile struct _FLAGS
{
    FIELD bit0:  1;
    FIELD bit1:  1;
    FIELD bit2:  1;
    FIELD bit3:  1;
    FIELD bit4:  1;
    FIELD bit5:  1;
    FIELD bit6:  1;
    FIELD bit7:  1;
    FIELD bit8:  1;
    FIELD bit9:  1;
    FIELD bit10: 1;
    FIELD bit11: 1;
    FIELD bit12: 1;
    FIELD bit13: 1;
    FIELD bit14: 1;
    FIELD bit15: 1;
}FLAGS;

typedef union _EXT_CB2
{
    unsigned char byte;
    struct
    {
        FIELD bit0 :       1;
        FIELD Break_Count: 3;   /* bit[1:3] */
        FIELD bit4 :       1;
        FIELD Make_Count:  3;   /* bit[5:7] */
    } field;
}EXT_CB2;

/* Function Pointers */
/*
 * FUNCT_PTR_V_V is a pointer to a function that returns nothing
 * (V for void) and takes nothing for a parameter (V for void).
 */
typedef void (*FUNCT_PTR_V_V)(void);

/*
 * FUNCT_PTR_V_W is a pointer to a function that returns nothing
 * (V for void) and takes a WORD for a parameter (W for WORD).
 */
typedef void (*FUNCT_PTR_V_W)(WORD);

/*
 * FUNCT_PTR_W_V is a pointer to a function that returns a WORD
 * (W for WORD) and takes nothing for a parameter (V for void).
 */
typedef WORD (*FUNCT_PTR_W_V)(void);

/*
 * FUNCT_PTR_W_W is a pointer to a function that returns a WORD
 * (W for WORD) and takes a WORD for a parameter (W for WORD).
 */
typedef WORD (*FUNCT_PTR_W_W)(WORD);

/*
 * FUNCT_PTR_V_B8 is a pointer to a function that returns nothing
 * (V for void) and takes a BITS_8 for a parameter (B8 for BITS_8).
 */
typedef void (*FUNCT_PTR_V_B8)(BITS_8);

/*
 * FUNCT_PTR_W_B is a pointer to a function that returns a WORD
 * (W for WORD) and takes a BYTE for a parameter (B for BYTE).
 */
typedef WORD (*FUNCT_PTR_W_B)(BYTE);

/*
 * FUNCT_PTR_B_B is a pointer to a function that returns a WORD
 * (W for WORD) and takes a BYTE for a parameter (B for BYTE).
 */
typedef BYTE (*FUNCT_PTR_B_B)(BYTE);

/*
 * FUNCT_PTR_B_V is a pointer to a function that returns a WORD
 * (W for WORD) and takes a BYTE for a parameter (B for BYTE).
 */
typedef BYTE (*FUNCT_PTR_B_V)(void);

/*
 * FUNCT_PTR_S_V is a pointer to a function that returns a BYTE
 * (S for BYTE) and takes nothing for a parameter (V for void).
 */
typedef BYTE (*FUNCT_PTR_S_V)(void);

typedef void (*FUNCT_PTR_V_B)(BYTE);

typedef void (*FUNCT_PTR_V_BS)(BYTE,BYTE);

typedef void (*FUN_PTR_V_PV)(void*);


#endif /* CHIP_TYPE_H_ */

