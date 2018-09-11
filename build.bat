@echo off

REM ***********************************************************************************************************
REM Setting
REM ***********************************************************************************************************
REM -----------------------------------------------------------------------------------------------------------
    SET EC_ROM_SIZE=190

    REM 2.0.0
    REM path=C:\BSPv410\toolchains\nds32le-elf-mculib-v3m\bin;.\Tools;.\Tools\NMake;C:\Windows\System32;

    REM 2.0.1 beta
    REM path=C:\BSPv410\toolchains\nds32le-elf-mculib-v3m\bin;.\Tools;.\Tools\NMake;C:\Windows\System32;

    REM 2.0.1
    REM path=C:\BSPv410\toolchains\nds32le-elf-mculib-v3m\bin;.\Tools;.\Tools\NMake;C:\Windows\System32;
    
    REM 3.2.1
    REM path=C:\BSPv410\toolchains\nds32le-elf-mculib-v3m\bin;.\Tools;.\Tools\NMake;C:\Windows\System32;

    REM 4.1.0
    path=C:\BSPv410\toolchains\nds32le-elf-mculib-v3m\bin;C:\BSPv410\cygwin\bin;.\Tools;.\Tools\NMake;C:\Windows\System32;


    REM -----------------------------------------------------------------------------------------------------------
    REM     0 : EC firmware only
    REM     1 : RTOS ( EC+Fusion+HID )
    REM     2 : RTOS ( Fusion+HID )
    REM     3 : Non-RTOS ( EC+Fusion+HID )
    REM -----------------------------------------------------------------------------------------------------------
    SET FIRMWARE_FEATURE_SELECTION=0

    SET SOURCE_BUILD=0

    SET SOURCE_VER=0

REM -----------------------------------------------------------------------------------------------------------
REM
REM INIT_BUILD_CONFIG
REM
REM -----------------------------------------------------------------------------------------------------------
IF  "%FIRMWARE_FEATURE_SELECTION%" == "0"  GOTO  BUILD_CONFIG_EC_ONLY
IF  "%FIRMWARE_FEATURE_SELECTION%" == "1"  GOTO  BUILD_CONFIG_RTOS_EC_FUSION_HID
IF  "%FIRMWARE_FEATURE_SELECTION%" == "2"  GOTO  BUILD_CONFIG_RTOS_FUSION_HID
IF  "%FIRMWARE_FEATURE_SELECTION%" == "3"  GOTO  BUILD_CONFIG_EC_FUSION_HID
GOTO BUILD_CONFIG_ERROR

:BUILD_CONFIG_EC_ONLY
    SET RTOS_BUILD=0
    SET EC_NULL=0
    SET EC_PURE=1
    GOTO BUILD_CONFIG_END

:BUILD_CONFIG_RTOS_EC_FUSION_HID
    SET RTOS_BUILD=1
    SET EC_NULL=0
    SET EC_PURE=0
    GOTO BUILD_CONFIG_END

:BUILD_CONFIG_RTOS_FUSION_HID
    SET RTOS_BUILD=1
    SET EC_NULL=1
    SET EC_PURE=0
    GOTO BUILD_CONFIG_END

:BUILD_CONFIG_EC_FUSION_HID
    SET RTOS_BUILD=0
    SET EC_NULL=0
    SET EC_PURE=0
    GOTO BUILD_CONFIG_END

:BUILD_CONFIG_END

REM ***********************************************************************************************************
REM Parameter
REM ***********************************************************************************************************
REM -----------------------------------------------------------------------------------------------------------
IF  "%1" == ""     GOTO OPTIONS
IF  "%1" == "?"    GOTO OPTIONS
IF  "%1" == "/?"   GOTO OPTIONS

IF  "%1" == "clear"   GOTO clear
IF  "%1" == "CLEAR"   GOTO clear

IF  "%2" == "ite"    GOTO BUILD
IF  "%2" == "akm"    GOTO BUILD
IF  "%2" == "akm2"   GOTO BUILD
IF  "%2" == "magna"  GOTO BUILD

GOTO OPTIONS
REM -----------------------------------------------------------------------------------------------------------

REM ***********************************************************************************************************
REM make file
REM ***********************************************************************************************************
:BUILD
cd Code
cd OEM
md Project_H
if not exist %1 goto NotExit
copy .\%1\*.c
copy .\%1\*.h
xcopy .\%1\*.h .\Project_H /S /Y
cd..
cd..

IF "%SOURCE_VER%" == "0" (
GOTO SrcVerEND
)ELSE IF "%SOURCE_VER%" == "1" (
xcopy ..\..\ver\cx\api .\code\api /S /Y
xcopy ..\..\ver\cx\nds .\code\nds /S /Y
)ELSE IF "%SOURCE_VER%" == "2" (
xcopy ..\..\ver\dx\api .\code\api /S /Y
xcopy ..\..\ver\dx\nds .\code\nds /S /Y
)ELSE IF "%SOURCE_VER%" == "3" (
xcopy ..\..\ver\it8390_v0\code\api .\code\api /S /Y
xcopy ..\..\ver\it8390_v0\code\nds .\code\nds /S /Y
xcopy ..\..\ver\it8390_v0\code\chip .\code\chip /S /Y
xcopy ..\..\ver\it8390_v0\code\core .\code\core /S /Y
xcopy ..\..\ver\it8390_v0\ITEEC.mak .\ITEEC.mak /Y
)ELSE IF "%SOURCE_VER%" == "4" (
xcopy ..\..\ver\it8353_v0\code\api .\code\api /S /Y
xcopy ..\..\ver\it8353_v0\code\nds .\code\nds /S /Y
xcopy ..\..\ver\it8353_v0\code\chip .\code\chip /S /Y
xcopy ..\..\ver\it8353_v0\ITEEC.mak .\ITEEC.mak /Y
)
:SrcVerEND

cls

IF  "%3" == "all"  GOTO CleanBuild
IF  "%3" == "ALL"  GOTO CleanBuild
goto NoCleanBuild

REM ***********************************************************************************************************
REM Clean build
REM ***********************************************************************************************************
:CleanBuild
del /q .\Misc\Obj\*.*

REM ***********************************************************************************************************
REM Building
REM ***********************************************************************************************************
:NoCleanBuild
IF  "%2" == "ite"    SET MAG_FUNC=0
IF  "%2" == "akm"    SET MAG_FUNC=1
IF  "%2" == "akm2"   SET MAG_FUNC=2
IF  "%2" == "magna"  SET MAG_FUNC=3

NMAKE ITEEC.mak FUSION_SRC=%SOURCE_BUILD% MAG_FUNC=%MAG_FUNC% FREE_RTOS=%RTOS_BUILD% NO_EC_FEATURE=%EC_NULL% Firmware_Feature_Selection=%FIRMWARE_FEATURE_SELECTION% ONLY_EC_FEATURE=%EC_PURE% OEM_PROJECT=%1
if errorlevel 1 goto errorend
GOTO OK

::-----------------------------------------------------------
:OK
::-----------------------------------------------------------
del /q .\code\oem\*.c
del /q .\code\oem\*.h
rmdir /s /q .\code\oem\Project_H
move iteec.adx .\misc\Obj
FU /SIZE %EC_ROM_SIZE% iteec.bin %1.bin FF

::-----------------------------------------------------------
:: To add checksum information
::-----------------------------------------------------------
IF  "%4" == "+checksum"  GOTO addchksum
IF  "%3" == "+checksum"  GOTO addchksum
GOTO :bbkcmd

:addchksum
copy %1.bin chksum_temp.bin
fu /wcsarm chksum_temp.bin 2000 1F7FF 1F7FE
rename chksum_temp.bin %1_c.bin

GOTO :Nomergebbk

::-----------------------------------------------------------
:: To merge bbk bin file
::-----------------------------------------------------------
:bbkcmd
IF  "%4" == "+bbk"  GOTO mergebbk
IF  "%4" == "+BBK"  GOTO mergebbk
IF  "%3" == "+bbk"  GOTO mergebbk
IF  "%3" == "+BBK"  GOTO mergebbk
GOTO :Nomergebbk

::-----------------------------------------------------------
:: To merge boot code or bbk bin file at 0x800 ~ 0x1FFF 6K bytes 
:: and modifying entry point of firmware
::-----------------------------------------------------------
:mergebbk
copy %1.bin bbk_temp.bin
fu /ow bbk_temp.bin .\code\bbk\bbk.bin with_bbk.bin 0x800
fu /binmodify with_bbk.bin 0x02 0x04
del /q bbk_temp.bin

::-----------------------------------------------------------
:: To caculate word checksum [start ~ end at location ]
:: checksum :           [start] [end] [word checksum location]
::-----------------------------------------------------------
fu /wcsarm with_bbk.bin 2000 1F7FF 1F7FE
rename with_bbk.bin %1_b.bin


::-----------------------------------------------------------
:: No merge bbk bin file
::-----------------------------------------------------------
:Nomergebbk
del /q iteec.bin
move *.bin .\rom\%1

REMIF  "%4" == "notsign" GOTO NOTSIGN
REMdel /q unsigned.zip
copy .\rom\%1\%1.bin 
rename %1.bin ec.bin
.\tools\7za.exe a -y -plenovo .\rom\%1\unsigned.zip .\code\oem\%1\Signkey\SctKeyRsa2048Public.bin .\code\oem\%1\Signkey\LnvEcSig.exe .\code\oem\%1\Signkey\ecfwsig.bat .\ec.bin
del .\signed.zip
del /q ec.bin
REM:NOTSIGN

cls
ECHO    ********************************************************************
ECHO    *    ITE Embedded Controller Firmware Build Process                *
ECHO    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
ECHO    ********************************************************************
ECHO.  
ECHO    Making EC bin file successfully !!!
GOTO done

:BUILD_CONFIG_ERROR
cls
ECHO    ********************************************************************
ECHO    *    ITE Embedded Controller Firmware Build Process                *
ECHO    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
ECHO    ********************************************************************
ECHO.  
ECHO    FIRMWARE_FEATURE_SELECTION(%FIRMWARE_FEATURE_SELECTION%) is error configure setting.(build.bat)
ECHO            0 : EC firmware only
ECHO            1 : RTOS ( EC+Fusion+HID )
ECHO            2 : RTOS (Fusion+HID)
ECHO            3 : Non-RTOS ( EC+Fusion+HID )
GOTO done

:NotExit
cls
ECHO    ********************************************************************
ECHO    *    ITE Embedded Controller Firmware Build Process                *
ECHO    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
ECHO    ********************************************************************
ECHO.  
ECHO    Project folder isn't exit.
cd ..
cd ..
GOTO done


:OPTIONS
cls
ECHO    ********************************************************************
ECHO    *    ITE Embedded Controller Firmware Build Process                *
ECHO    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
ECHO    ********************************************************************
ECHO.    
ECHO    USAGE:  build [P1] [P2] [P3] [P4]
ECHO                  P1 = The project name of OEM folder. or [clear] [CLEAR]
ECHO                  P2 = "akm": for ak8963, "akm2": for ak09911, "magna": for mxg1300, "ite": for ITE algorithm.
ECHO                  P3 = [all] [ALL] [+bbk] [+BBK]
ECHO                  P4 = [+bbk] [+BBK]
ECHO.
GOTO done

:clear
del /q .\rom\%2\*.*
del /q .\misc\lst\*.*
del /q .\misc\map\*.*
del /q .\misc\obj\*.*
if not exist .\code\oem\Project_H goto done
rmdir /s /q .\code\oem\Project_H
GOTO done

::-----------------------------------------------------------
:: Fail
::-----------------------------------------------------------
:errorend
del /q .\code\oem\*.c
del /q .\code\oem\*.h

::-----------------------------------------------------------
:: Done
::-----------------------------------------------------------
:done

@echo on