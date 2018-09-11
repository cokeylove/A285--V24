#//****************************************************************************************
#
#//****************************************************************************************
#
# Compiler
#
#CC=nds32le-elf-gcc
CC=nds32le-elf-gcc-4.9.3


#
# Assembler
#
#AS=nds32le-elf-gcc
AS=nds32le-elf-gcc-4.9.3


#
# Linker
#
#Linker=nds32le-elf-gcc
Linker=nds32le-elf-gcc-4.9.3

# library maker
#Libmaker=nds32le-elf-ar


#
# Parameter of compiler
#
CDirectives=-Os -Wall -Wno-strict-aliasing -Wno-uninitialized -Wno-unused-but-set-variable -Werror -c -fmessage-length=0
#CDirectives=-Os -Wall -Wno-strict-aliasing -Wno-uninitialized -Werror -c -fmessage-length=0 -ffunction-sections
#
# Parameter of linker
#
LDirectives=-nostartfiles -static -T LDS\ite8380.lds -Wl,-Map -Wl,Misc\Map\map.txt -Os -o
#LDirectives=-nostartfiles -static -T LDS\ite8380.lds -Wl,-Map -Wl,Misc\Map\map.txt -Wl,--gc-sections,--print-gc-sections -Os -o
#
# LDS file
#
LDS_File=LDS\ite8380.lds

#
# path of all header files
#
All_Include=code\*.h\
            code\chip\*.h\
!IF $(NO_EC_FEATURE)
            code\api\ec_null\*.h\
!ELSE
            code\core\*.h\
!ENDIF 
            code\nds\include\*.h\
            code\nds\include\*.inc\
            code\oem\LCFC_COMMON\INCLUDE\*.h\
            code\oem\$(OEM_PROJECT)\INCLUDE\*.h\
            code\api\*.h\
            code\api\debug\*.h\
            code\api\osc\*.h\
            code\api\direct_map\*.h\
#            code\api\usbpd\*.h\
            code\api\ext_timer\*.h\
!IF $(ONLY_EC_FEATURE)
!ELSE
            code\api\hid\*.h\
            code\api\i2c\*.h\
            code\api\sensor_fusion\*.h\
!IF $(FUSION_SRC)
            code\api\usb\*.h\
!ENDIF            
!IF $(FREE_RTOS)
            code\api\free_rtos\rtos-freertos\freertos\*.h\
            code\api\free_rtos\rtos-freertos\freertos\hal\*.h\
            code\api\free_rtos\rtos-freertos\freertos\kernel\Source\include\*.h\
!ELSE
            code\api\rtos_null\hal.h\
!ENDIF
!ENDIF

#
# path of all obj files
#
OBJ_PATH=Misc\Obj
            
#
# all oem obj files
#
OEMFiles=\
!IF $(NO_EC_FEATURE)
		$(OBJ_PATH)\oem_ver.o
!ELSE
        $(OBJ_PATH)\oem_6064.o\
        $(OBJ_PATH)\oem_acpi.o\
        $(OBJ_PATH)\oem_adc.o\
        $(OBJ_PATH)\oem_asm.o\
        $(OBJ_PATH)\oem_battery.o\
        $(OBJ_PATH)\oem_cir.o\
        $(OBJ_PATH)\oem_debug.o\
        $(OBJ_PATH)\oem_ecpdm.o\
        $(OBJ_PATH)\oem_event.o\
        $(OBJ_PATH)\oem_fan.o\
        $(OBJ_PATH)\oem_flash.o\
        $(OBJ_PATH)\oem_gpio.o\
        $(OBJ_PATH)\oem_hostif.o\
        $(OBJ_PATH)\oem_hspi.o\
        $(OBJ_PATH)\oem_init.o\
        $(OBJ_PATH)\oem_irq.o\
        $(OBJ_PATH)\oem_lcd.o\
        $(OBJ_PATH)\oem_led.o\
        $(OBJ_PATH)\oem_lpc.o\
        $(OBJ_PATH)\oem_mailbox.o\
        $(OBJ_PATH)\oem_main.o\
        $(OBJ_PATH)\oem_memory.o\
        $(OBJ_PATH)\oem_mmb.o\
        $(OBJ_PATH)\oem_peci.o\
        $(OBJ_PATH)\oem_pm2.o\
        $(OBJ_PATH)\oem_pm3.o\
        $(OBJ_PATH)\oem_port686c.o\
        $(OBJ_PATH)\oem_power.o\
        $(OBJ_PATH)\oem_ps2.o\
        $(OBJ_PATH)\oem_scantabs.o\
        $(OBJ_PATH)\oem_smbus.o\
        $(OBJ_PATH)\oem_spi.o\
        $(OBJ_PATH)\oem_timer.o\
        $(OBJ_PATH)\oem_ver.o\
        $(OBJ_PATH)\oem_beep.o\
        $(OBJ_PATH)\oem_fantbl.o\
        $(OBJ_PATH)\oem_powerseq.o\
        $(OBJ_PATH)\oem_hook.o\
        $(OBJ_PATH)\oem_gsns.o\
        $(OBJ_PATH)\oem_vga_amd.o\
        $(OBJ_PATH)\oem_vga_nv.o\
        $(OBJ_PATH)\oem_AOU.o\
        $(OBJ_PATH)\oem_edge.o\
        $(OBJ_PATH)\oem_ccg2.o\
        $(OBJ_PATH)\oem_ccg3.o\
        $(OBJ_PATH)\oem_EEPROM.o\
        $(OBJ_PATH)\oem_CHGIC_25700.o\
        $(OBJ_PATH)\oem_Dash.o\
        $(OBJ_PATH)\origa_ecc.o\
        $(OBJ_PATH)\Origa_Gf2nMath.o\
        $(OBJ_PATH)\oem_TAMPER.o\
        $(OBJ_PATH)\OEM_TPS65988.o\
        $(OBJ_PATH)\OEM_EcCmd.o \
        $(OBJ_PATH)\OEM_ucsi.o\
        $(OBJ_PATH)\OEM_BMA255.o\
        $(OBJ_PATH)\OEM_EnhancedPcBeep.o\
        $(OBJ_PATH)\oem_BottomTamper.o\
        $(OBJ_PATH)\oem_RFID.o
!ENDIF

#
# ec api obj files
#
OBJ_API_EC=\
!IF $(NO_EC_FEATURE)
!ELSE
	$(OBJ_PATH)\api_gpio.o\
	$(OBJ_PATH)\api_peci.o\
	$(OBJ_PATH)\api_wuc.o\
	$(OBJ_PATH)\api_sha1.o\
	$(OBJ_PATH)\api_ow.o\
	$(OBJ_PATH)\api_sspi.o\
	$(OBJ_PATH)\api_intc.o\
	$(OBJ_PATH)\api_etwd.o\
 	$(OBJ_PATH)\api_adc.o\
	$(OBJ_PATH)\api_dac.o\
	$(OBJ_PATH)\api_pwm.o\
	$(OBJ_PATH)\api_i2c_slave.o
!ENDIF		

#
# To link [api_xxx.o] if related api function be used.
#
OBJ_API_EC_Link=\
!IF $(NO_EC_FEATURE)
!ELSE
	$(OBJ_PATH)\api_wuc.o\
	$(OBJ_PATH)\api_etwd.o\
	$(OBJ_PATH)\api_pwm.o\
	$(OBJ_PATH)\api_adc.o
!ENDIF
# ---------------------------
# $(OBJ_PATH)\api_gpio.o\
# $(OBJ_PATH)\api_peci.o\
# $(OBJ_PATH)\api_wuc.o\
# $(OBJ_PATH)\api_sha1.o\
# $(OBJ_PATH)\api_ow.o\
# $(OBJ_PATH)\api_sspi.o\
# $(OBJ_PATH)\api_intc.o\
# $(OBJ_PATH)\api_etwd.o\
# $(OBJ_PATH)\api_adc.o\
# $(OBJ_PATH)\api_dac.o\
# $(OBJ_PATH)\api_pwm.o\
# $(OBJ_PATH)\api_i2c_slave.o\
# ---------------------------

#
# all core obj files
#
COREFiles=\
!IF $(NO_EC_FEATURE)
        $(OBJ_PATH)\core_gpio.o\
        $(OBJ_PATH)\core_timer.o\
        $(OBJ_PATH)\core_sync.o\
        $(OBJ_PATH)\core_irq.o\
        $(OBJ_PATH)\core_power.o
!ELSE
        $(OBJ_PATH)\core_acpi.o\
        $(OBJ_PATH)\core_asm.o\
        $(OBJ_PATH)\core_flash.o\
        $(OBJ_PATH)\core_gpio.o\
        $(OBJ_PATH)\core_hostif.o\
        $(OBJ_PATH)\core_init.o\
        $(OBJ_PATH)\core_irq.o\
        $(OBJ_PATH)\core_main.o\
        $(OBJ_PATH)\core_memory.o\
        $(OBJ_PATH)\core_port6064.o\
        $(OBJ_PATH)\core_ps2.o\
        $(OBJ_PATH)\core_scan.o\
        $(OBJ_PATH)\core_smbus.o\
        $(OBJ_PATH)\core_timer.o\
        $(OBJ_PATH)\core_xlt.o\
        $(OBJ_PATH)\core_power.o
!ENDIF

#
# all nds obj files
#
NDSFiles=\
        $(OBJ_PATH)\crt0.o\
        $(OBJ_PATH)\os_cpu_a.o

#
# osc obj file
#
APIFiles_OSC=\
        $(OBJ_PATH)\osc.o

#
# debug obj files
#
APIFiles_Debug=\
        $(OBJ_PATH)\debug_print.o\
        $(OBJ_PATH)\debug_ns16550.o

#
# direct_map obj files
#
APIFiles_DirectMap=\
        $(OBJ_PATH)\direct_map.o

#
# usbpd obj files
#
#APIFiles_UsbPD=\
#        $(OBJ_PATH)\usbpd_hal.o\
#        $(OBJ_PATH)\usbpd.o\
#        $(OBJ_PATH)\usbpd_proto.o\
#        $(OBJ_PATH)\usbpd_vdm.o\
#        $(OBJ_PATH)\usbpd_dp.o\
#        $(OBJ_PATH)\power.o

#
# ext_timer obj files
#
APIFiles_ExtTimer=\
        $(OBJ_PATH)\ext_timer.o

#
# i2c obj files
#
APIFiles_I2C=\
!IF $(ONLY_EC_FEATURE)
		$(OBJ_PATH)\i2c.o\
        $(OBJ_PATH)\i2c_drv.o\
        $(OBJ_PATH)\i2c_pio.o\
        $(OBJ_PATH)\i2c_cmd_q.o
!ELSE
        $(OBJ_PATH)\i2c.o\
        $(OBJ_PATH)\i2c_drv.o\
        $(OBJ_PATH)\i2c_pio.o\
        $(OBJ_PATH)\i2c_cmd_q.o
!ENDIF

#
# hid obj file
#
APIFiles_HID=\
!IF $(ONLY_EC_FEATURE)
!ELSE
        $(OBJ_PATH)\hid_hal.o\
!IF $(FUSION_SRC)
        $(OBJ_PATH)\hid_custom_cmd.o\
        $(OBJ_PATH)\hid_common.o
!ENDIF
!ENDIF

#
# free rtos obj files
#
APIFiles_freertos=\
!IF $(ONLY_EC_FEATURE)
!ELSE
!IF $(FREE_RTOS)
        $(OBJ_PATH)\port.o\
        $(OBJ_PATH)\portISR.o\
        $(OBJ_PATH)\debug.o\
        $(OBJ_PATH)\vApplication.o\
        $(OBJ_PATH)\hal.o\
        $(OBJ_PATH)\list.o\
        $(OBJ_PATH)\queue.o\
        $(OBJ_PATH)\tasks.o\
        $(OBJ_PATH)\heap_2.o\
        $(OBJ_PATH)\task_main.o
!ELSE
        $(OBJ_PATH)\hal.o\
!ENDIF
!ENDIF

#
# sensor obj files
#
APIFiles_sensor=\
!IF $(ONLY_EC_FEATURE)
!ELSE
        $(OBJ_PATH)\sensor.o\
        $(OBJ_PATH)\sensor_fusion.o
!ENDIF

#
# fusion
#
APIFiles_fusion=\
!IF $(ONLY_EC_FEATURE)
!ELSE
!IF $(FUSION_SRC)
        $(OBJ_PATH)\compass_calibrate.o\
        $(OBJ_PATH)\fusion_common.o\
        $(OBJ_PATH)\fusion.o
!ENDIF
!ENDIF

OBJS=$(OEMFiles)\
     $(COREFiles)\
     $(NDSFiles)\
     $(APIFiles_OSC)\
     $(APIFiles_Debug)\
     $(APIFiles_DirectMap)\
     $(APIFiles_ExtTimer)\
     $(APIFiles_I2C)\
     $(APIFiles_HID)\
     $(APIFiles_freertos)\
     $(APIFiles_sensor)\
     $(APIFiles_fusion)\

#//****************************************************************************************

#//****************************************************************************************
# LIB setting for linker
#//****************************************************************************************
LIB_DIR=-Lcode\lib\
!IF "$(MAG_FUNC)" == "1"
        -Lcode\lib\ak8963  
!ELSEIF "$(MAG_FUNC)" == "2"
        -Lcode\lib\ak09911
!ENDIF        

LIB_NAME=\
!IF !$(FUSION_SRC)
         -lFusion\
         -lHID\
!ENDIF
!IF "$(MAG_FUNC)" == "1"
         -lAKMApi\
         -lAKMCore\
!ELSEIF "$(MAG_FUNC)" == "2"
         -lAKMApi\
         -lAKMCore\
!ELSEIF "$(MAG_FUNC)" == "3"
         -lMXApi\
!ENDIF
    	 #-lMemsicAccAlgo\
    	 #-lm\

#//****************************************************************************************
#
#//****************************************************************************************
ITE_DEFS=-D__DISABLE_UART_AUTO_CLK_GATING__\
!IF "$(MAG_FUNC)" == "1"
         -D__ENABLE_AKM_LIB__\
!ELSEIF "$(MAG_FUNC)" == "2"
         -D__ENABLE_AKM2_LIB__\
!ELSEIF "$(MAG_FUNC)" == "3"
         -D__ENABLE_MAGNA_LIB__\
!ENDIF
         -DFirmware_Feature_Selection=$(FIRMWARE_FEATURE_SELECTION)\
         -DENABLE_IVT_MAP_TO_FLASH\
         #-D__ENABLE_DBG_MSG__\
         #-D__ENABLE_TASK_PERF__\

#//****************************************************************************************
#
#//****************************************************************************************
ITE_DIR=-Icode\nds\boot\
        -Icode\nds\include\
        -Icode\chip\
!IF $(NO_EC_FEATURE)
        -Icode\api\ec_null\
!ELSE        
        -Icode\core\
!ENDIF          
		-Icode\api\i2c\
#        -Icode\oem\
		-Icode\oem\LCFC_COMMON\
		-Icode\oem\$(OEM_PROJECT)\
        -Icode\api\
        -Icode\api\debug\
        -Icode\api\direct_map\
#        -Icode\api\usbpd\
        -Icode\api\ext_timer\
        -Icode\api\hid\
#        -Icode\api\i2c\
        -Icode\api\osc\
!IF $(FREE_RTOS)
        -Icode\api\free_rtos\rtos-freertos\freertos\
        -Icode\api\free_rtos\rtos-freertos\freertos\debug\
        -Icode\api\free_rtos\rtos-freertos\freertos\hal\
        -Icode\api\free_rtos\rtos-freertos\freertos\Hook\
        -Icode\api\free_rtos\rtos-freertos\freertos\kernel\Source\include\
!ELSE
        -Icode\api\rtos_null\
!ENDIF      
        -Icode\api\sensor_fusion\
!IF $(FUSION_SRC)
        -Icode\api\usb\
!ENDIF        

#//****************************************************************************************
# Target : 
#//****************************************************************************************
ALL:iteec.bin

#//****************************************************************************************
# Hex file to bin file and obj dump.
# Syntax : 
#//****************************************************************************************
iteec.bin:iteec.adx
    nds32le-elf-objcopy -S -O binary iteec.adx iteec.bin
    nds32le-elf-nm -n -l -C iteec.adx > misc/map/symbol.txt
    nds32le-elf-readelf -a iteec.adx > misc/map/readelf.txt
    nds32le-elf-objdump -x -d -C iteec.adx > misc/map/objdump.txt
    
#//****************************************************************************************
# Link all obj fils and lib
#//****************************************************************************************
iteec.adx:$(OBJS)\
        $(OBJ_API_EC)\
        $(LDS_File)
        $(Linker) $(LDirectives) iteec.adx\
        $(OBJS)\
        $(OBJ_API_EC_Link)\
        $(LIB_DIR)\
        $(LIB_NAME)\

#//****************************************************************************************
#
#//****************************************************************************************
$(OBJS):$(All_Include)
!IF $(NO_EC_FEATURE)
!ELSE
$(OBJ_API_EC):$(All_Include)
!ENDIF

!IF $(FUSION_SRC)
!IF $(ONLY_EC_FEATURE)
!ELSE
$(OBJ_PATH)\hid_common.o:code\api\hid\i2c_hid.c code\api\hid\usb_slave_hid.c code\api\usb\usb_slave.c code\api\usb\usb_slave_const.c
!ENDIF
!ENDIF

#------------------------------------------------------------------
# Compile oem file
#------------------------------------------------------------------
!IF $(NO_EC_FEATURE)
{code\oem\LCFC_COMMON\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) -o $*.o $<
{code\oem\$(OEM_PROJECT)\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) -o $*.o $<
!ELSE
{code\oem\LCFC_COMMON\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) -o $*.o $<
{code\oem\$(OEM_PROJECT)\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) -o $*.o $<
!ENDIF    

#------------------------------------------------------------------
# Compile core file
#------------------------------------------------------------------
!IF $(NO_EC_FEATURE)
{code\api\ec_null\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) -o $*.o $<
!ELSE
{code\core\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) -o $*.o $<
!ENDIF
    
#------------------------------------------------------------------
# Compile nds file
#------------------------------------------------------------------
$(OBJ_PATH)\crt0.o:code\nds\boot\crt0.s $(All_Include)
    $(CC) $(CDirectives) $(ITE_DEFS) -o $(OBJ_PATH)\crt0.o code\nds\boot\crt0.S

$(OBJ_PATH)\os_cpu_a.o:code\nds\boot\os_cpu_a.s $(All_Include)
    $(CC) $(CDirectives) $(ITE_DEFS) -o $(OBJ_PATH)\os_cpu_a.o code\nds\boot\os_cpu_a.S

#------------------------------------------------------------------
# Compile api file
#------------------------------------------------------------------
{code\api\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\debug\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\direct_map\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<
    
#{code\api\usbpd\}.c{$(OBJ_PATH)\}.o:
#    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<    

{code\api\ext_timer\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

!IF $(FREE_RTOS)
{code\api\free_rtos\rtos-freertos\freertos\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\free_rtos\rtos-freertos\freertos\debug\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\free_rtos\rtos-freertos\freertos\Hook\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\free_rtos\rtos-freertos\freertos\hal\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\free_rtos\rtos-freertos\freertos\kernel\Source\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\free_rtos\rtos-freertos\freertos\kernel\Source\portable\MemMang\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<
!ELSE
{code\api\rtos_null\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<
!ENDIF

{code\api\hid\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\i2c\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\sensor_fusion\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\sensor_fusion\sensor\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

{code\api\osc\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<

!IF $(FUSION_SRC)
{code\api\sensor_fusion\fusion\}.c{$(OBJ_PATH)\}.o:
    $(CC) $(CDirectives) $(ITE_DEFS) $(ITE_DIR) -o $*.o $<
!ENDIF

#------------------------------------------------------------------
