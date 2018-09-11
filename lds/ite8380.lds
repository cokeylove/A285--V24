/* -----------------------------------------------*/
/* ROM code linker script, for normal executables */
/* -----------------------------------------------*/

OUTPUT_FORMAT("elf32-nds32", "elf32-nds32", "elf32-nds32")
OUTPUT_ARCH(nds32)

MEMORY
{
	ILM_BASE_ISR_IVT (RX)   : ORIGIN = 0x00000000, LENGTH = 512
	ILM_BASE_OSC (RX)       : ORIGIN = 0x00000200, LENGTH = 512
	ILM_BASE_ISR (RX)       : ORIGIN = 0x00000400, LENGTH = 1K
	ILM_BASE_BBK (RX)       : ORIGIN = 0x00000800, LENGTH = 6K
	ILM_BASE_RAM_CODE (RX)  : ORIGIN = 0x00002000, LENGTH = 3K
	ILM_BASE_IRQ (RX)       : ORIGIN = 0x00002C00, LENGTH = 1K
	ILM_BASE_MAIN (RX)      : ORIGIN = 0x00003000, LENGTH = 256
	ILM_BASE_OEM_VER (RX) 	: ORIGIN = 0x00003100, LENGTH = 256
	ILM_BASE_TEXT_ISR (RX)  : ORIGIN = 0x00003200, LENGTH = 512
	ILM_BASE_TEXT (RX)      : ORIGIN = 0x00003400, LENGTH = 177K

	/* ---------------------------------------------------  */
	/* 0x00080000 ~ 0x000801FF, 512 bytes for CORE feature  */
	/* 0x00080200 ~ 0x000813FF, 4.5K for OEM feature        */
	/* ---------------------------------------------------  */
	DLM_BASE (ARW)          : ORIGIN = 0x00081400, LENGTH = 7K
}

/* -----------------------------------------------*/
/* define entry point of the program              */
/* -----------------------------------------------*/
ENTRY(_code_start)

SECTIONS
{
	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._isr_ivt ORIGIN(ILM_BASE_ISR_IVT) : 
	{	
		PROVIDE (__isr_ivt_begin = .);
		KEEP(*(.ite8380_init))
		*(.oem_recover)								/******Alex modify for oem version ********/
		PROVIDE (__isr_ivt_end = .);
	} > ILM_BASE_ISR_IVT

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._osc ORIGIN(ILM_BASE_OSC) : 
	{	
		PROVIDE (__osc_begin = .);
		KEEP(*(.OSC_CODE))
		PROVIDE (__osc_end = .);
	} > ILM_BASE_OSC

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._isr ORIGIN(ILM_BASE_ISR) : 
	{	
		PROVIDE (__isr_begin = .);
		*(.ite8380_isr)
		*(._Interrupt_HW0)
		*(._Interrupt_HW1)
		*(._Interrupt_HW2)
		*(._Interrupt_HW3)
		*(._Interrupt_HW4)
		*(._Interrupt_HW5)
		*(._Interrupt_HW6)
		*(._Interrupt_HW7)
		*(._Interrupt_HW8)
		*(._Interrupt_HW9)
		*(._Interrupt_HW10)
		*(._Interrupt_HW11)
		*(._Interrupt_HW12)
		*(._Interrupt_HW13)
		*(._Interrupt_HW14)
		*(._Interrupt_HW15)
		*(._Interrupt_SW0)
		PROVIDE (__isr_end = .);
	} > ILM_BASE_ISR

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._bbk ORIGIN(ILM_BASE_BBK) : 
	{	
		PROVIDE (__bbk_begin = .);
		*(.OEM_VERSION)					/******Alex + for oem version ********/
		*(.FUSION_VER_RODATA)
		/* *core_init.o */
		*core_acpi.o
		*core_port6064.o
		PROVIDE (__bbk_end = .);
	} > ILM_BASE_BBK

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._ramcode ORIGIN(ILM_BASE_RAM_CODE) : 
	{	
		PROVIDE (__ramcode_begin = .);
		*(.core_flash_reset_immu)
		*(.core_flash)
		*core_flash.o
	} > ILM_BASE_RAM_CODE

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._irq ORIGIN(ILM_BASE_IRQ) :
	{
		*(._IRQ_ISR_HND)
		PROVIDE (__ramcode_end = .);
	}

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._main ORIGIN(ILM_BASE_MAIN) :
	{
		PROVIDE (__main_func_begin = .);
		*(.func__main)					/* main function fix address */
		PROVIDE (__main_func_end = .);
	} > ILM_BASE_MAIN

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._oem_version ORIGIN(ILM_BASE_OEM_VER) :
	{
		*(._OEM_VERSION_RODATA)
		*(.HID_VER_RODATA)
		*(.FUSION_VER_RODATA)
	} > ILM_BASE_OEM_VER

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	._text_isr ORIGIN(ILM_BASE_TEXT_ISR):
	{	
		PROVIDE (__text_isr_begin = .);
		*(._Isr_Int1)
		*(._General_Exception)
		PROVIDE (__text_isr_end = .);
	} > ILM_BASE_TEXT_ISR

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	.text ORIGIN(ILM_BASE_TEXT):
	{	
		PROVIDE (__text_begin = .);
		*(._code_start)					/* code start fix address */
		PROVIDE (__text_end = .);

		. = ALIGN(0x200);
		*core_smbus.o (.text .text.*)

		/* -----------------------------------------------*/
		/* ram code section start, (direct map)
		/* -----------------------------------------------*/
		/* -----------------------------------------------*/
		/* For EC
		/* -----------------------------------------------*/
		/* . = ALIGN(0x1000); */
		/* EC ram code section always at boundary 4K */

		/* -----------------------------------------------*/
		/* For RTOS
		/* -----------------------------------------------*/
		. = ALIGN(0x1000);
		PROVIDE (_vma_dm_s = .);
		*( ._ISR_RODATA )
		*( ._ISR_CODE )
		*( ._Isr_Int1 )
		*( ._ISR_CODE_OEM )
		*( .RTOS_CODE )
		*libc.a:*memcpy.o (.text .text.*)
		*libc.a:*memset.o (.text .text.*)
		*libc.a:*memcmp.o (.text .text.*)
		*libc.a:*abs.o (.text .text.*)
		*libc.a:*strncpy.o (.text .text.*)
		_text_hid_s = .;
		*( .HID_CODE )
		*( .I2C_CODE )
		*( .USB_CODE )
		*( .HID_RODATA )
		_text_sensor_s = .;
		*( .SENSOR_CODE )
		*( .SENSOR_RODATA )
		PROVIDE (_vma_dm_e = .);
		/* -----------------------------------------------*/
		/* ram code section end, (direct map)
		/* -----------------------------------------------*/
		. = ALIGN(0x1000);
		_text_hid_l_s = .;
		*( .HID_RODATA_L )
		*( .HID_CODE_L )
		*( .I2C_CODE_L )
		*( .USB_CODE_L )
		_text_sensor_l_s = .;
		*( .SENSOR_CODE_L )
		_text_rtos_l_s = .;
		*( .RTOS_CODE_L )
		PROVIDE (_text_s = .);
		*(.text .text.*)
	} > ILM_BASE_TEXT
	_dm_size = (_vma_dm_e - _vma_dm_s);

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	.rodata : 
	{ 
		PROVIDE (__rodata_begin = .);
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		PROVIDE (__rodata_end = .);
	} > ILM_BASE_TEXT

	PROVIDE (__lma_data_start = .);

	/* -----------------------------------------------*/
	/* -----------------------------------------------*/
	.data ORIGIN(DLM_BASE) : 
	{
		*( .RTOS_DATA_H )
		*os_cpu_a.o(.data)
		*( .RTOS_DATA )
		. = ALIGN(4);
		_hid_data_s = .;
		*( .HID_DATA )
		*( .USB_DATA )
		*( .I2C_DATA )
		. = ALIGN(4);
		_sensor_data_s = .;
		*( .SENSOR_DATA ) 
		. = ALIGN(4);
		_data_s = .;
		*( .sdata_d .sdata_d.* )
		*( .sdata_w .sdata_w.* )
		*( .sdata_h .sdata_h.* )
		*( .sdata_b .sdata_b.* )
		. = ALIGN(4);
		*( .data .data.* .gnu.linkonce.d.* )
		. = ALIGN(4);
	} > DLM_BASE AT>ILM_BASE_TEXT
	PROVIDE (__vma_data_start = ADDR(.data));
	PROVIDE (__data_size = SIZEOF(.data));
	. = ALIGN(4);

	/* We want the small data sections together, so single-instruction offsets
	can access them all, and initialized data all before uninitialized, so
	we can shorten the on-disk segment size. */

	.bss (NOLOAD):
	{
		_rtos_bss_s = .;
		*heap_2.o (.bss)
		_hid_bss_s = .;
		*( .HID_BSS )
		*( .USB_BSS )
		*( .I2C_BSS )
		*hid_common.o (.bss .scommon_w .scommon_h .scommon_b COMMON)
		*hid_custom_cmd.o (.bss .scommon_w .scommon_h .scommon_b COMMON)
		_sensor_bss_s = .;
		*( .SENSOR_BSS )
		*fusion.o (.bss .scommon_w .scommon_h .scommon_b COMMON)
		*sensor_fusion.o (.bss .scommon_w .scommon_h .scommon_b COMMON)
		*compass_calibrate.o (.bss .scommon_w .scommon_h .scommon_b COMMON)
		*libAKMApi.a:*.* (.bss .scommon_w .scommon_h .scommon_b COMMON)
		*libAKMCore.a:*. (.bss .scommon_w .scommon_h .scommon_b COMMON)
		_bss_s = .;
		*( .sbss_d .sbss_d.* .scommon_d .scommon_d.*)
		*( .sbss_w .sbss_w.* .scommon_w .scommon_w.*)
		*( .sbss_h .sbss_h.* .scommon_h .scommon_h.*)
		*( .sbss_b .sbss_b.* .scommon_b .scommon_b.*)
		. = ALIGN(4);
		*( .bss .bss.* .scommon .scommon.* COMMON .gnu.linkonce.b.* )
		. = ALIGN(4);
		/* Align here to ensure that the .bss section occupies space up to
		_end.  Align after .bss to ensure correct alignment even if the
		.bss section disappears because there are no input sections.  */
	} > DLM_BASE
	PROVIDE (__vma_bss_end = .);
	PROVIDE (__vma_bss_start = ADDR(.bss));
	PROVIDE (__bss_size = SIZEOF(.bss));
	PROVIDE (_end = .);

	/* ----------------------------------------------- */
	/* ----------------------------------------------- */
	/DISCARD/ (NOLOAD):
	{
		*(.comment)
		*(.debug_aranges)
		*(.debug_ranges)
		*(.debug_pubnames)
		*(.debug_info .gnu.linkonce.wi.*)
		*(.debug_abbrev)
		*(.debug_line)
		*(.debug_frame)
		*(.debug_str)
		*(.debug_loc)
		*(.debug_macinfo)
		*(.note.nds32)
		*(.note.nds32.ld)
		*(.note.GNU-stack)
		*(.ex9.itable)
	}

	/* -----------------------------------------------*/
	/*  Symbol Address                                */
	/* -----------------------------------------------*/
	PROVIDE (_stack_ec = 0x00083000);
	PROVIDE (_stack = ORIGIN(DLM_BASE));
	PROVIDE (_storage = ORIGIN(ILM_BASE_TEXT)+LENGTH(ILM_BASE_TEXT));
	PROVIDE (_hid_fusion_buf = ORIGIN(DLM_BASE) - 0x200);
	
    PROVIDE (_SDA_BASE_ = 0x00080000);
    PROVIDE (_FP_BASE_  = 0x00080000);

	/* -----------------------------------------------*/
	/*  RTOS/FUSION/HID Code Size                     */
	/* -----------------------------------------------*/
	_rtos_size   = (_text_hid_l_s - _text_rtos_l_s) + (_text_hid_s - _vma_dm_s);
	_hid_size    = (_text_sensor_l_s - _text_hid_l_s)  + (_text_sensor_s - _text_hid_s);
	_sensor_size = (_vma_dm_e - _text_sensor_s) + (_text_s - _text_sensor_l_s);

	/* -----------------------------------------------*/
	/*  RTOS/FUSION/HID data Size                     */
	/* -----------------------------------------------*/	
	_rtos_data_size   = _hid_data_s - __vma_data_start;
	_hid_data_size    = _sensor_data_s - _hid_data_s;
	_sensor_data_size = _data_s - _sensor_data_s;

	/* -----------------------------------------------*/
	/*  RTOS/FUSION/HID bss Size                      */
	/* -----------------------------------------------*/
	_rtos_bss_size   = _hid_bss_s - _rtos_bss_s;
	_hid_bss_size    = _sensor_bss_s - _hid_bss_s;
	_sensor_bss_size = _bss_s - _sensor_bss_s;
}
