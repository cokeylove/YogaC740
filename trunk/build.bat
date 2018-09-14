@echo off

REM ***********************************************************************************************************
REM	Setting
REM ***********************************************************************************************************
REM -----------------------------------------------------------------------------------------------------------
REM (64, 96, 128, and 160 are valid)
	SET EC_ROM_SIZE=128

	path=C:\Keil\C51\BIN;.\Tools;.\Tools\NMake;
REM -----------------------------------------------------------------------------------------------------------


REM ***********************************************************************************************************
REM	Parameter
REM ***********************************************************************************************************
REM -----------------------------------------------------------------------------------------------------------
IF  "%1" == ""     GOTO OPTIONS
IF  "%1" == "?"    GOTO OPTIONS
IF  "%1" == "/?"   GOTO OPTIONS

IF  "%1" == "clear"   GOTO clear
IF  "%1" == "CLEAR"   GOTO clear

IF  "%2" == ""	   GOTO BUILD
IF  "%2" == "all"  GOTO BUILDALL
IF  "%2" == "ALL"  GOTO BUILDALL
GOTO OPTIONS
REM -----------------------------------------------------------------------------------------------------------

REM ***********************************************************************************************************
REM	Clean build
REM ***********************************************************************************************************
:BUILDALL
del /q .\ROM\*.*
copy .\KeilMisc\SHA1.OBJ    .\KeilMisc\Obj\
REM ***********************************************************************************************************
REM	make file
REM ***********************************************************************************************************
:BUILD
cd Code
cd OEM
if not exist %1 goto NotExit
mkdir INCLUDE
copy .\%1\OEM_BANK0\*.c
copy .\%1\OEM_BANK1\*.c
copy .\%1\OEM_BANK2\*.c
copy .\%1\OEM_BANK3\*.c
cd INCLUDE
copy ..\%1\INCLUDE\*.h
cd..
cd..
cd..

cd Code
cd CORE
copy .\CORE_BANK0\*.c
copy .\CORE_COMMON\*.c
copy .\CORE_COMMON\*.a51
cd..
cd..
copy Tools\*.bin
cls

REM ***********************************************************************************************************
REM	Building
REM ***********************************************************************************************************
NMAKE ITEEC.mak
if errorlevel 2 goto errorend

::-----------------------------------------------------------
:: Common + Bank0 + Bank1 + Bank2 + Bank3 [160KB]
::-----------------------------------------------------------
FU /SIZE 64 ITEEC.b0 BANK0.BIN FF
FU /SIZE 32 ITEEC.b1 BANK1.BIN FF
FU /SIZE 32 ITEEC.b2 BANK2.BIN FF
FU /SIZE 32 ITEEC.b3 BANK3.BIN FF

COPY /b BANK0.BIN+BANK1.BIN+BANK2.BIN+BANK3.BIN ITEEC.ALL

FU /SIZE %EC_ROM_SIZE% ITEEC.ALL ITEEC.BIN FF

::-----------------------------------------------------------
:: OK
::-----------------------------------------------------------
copy ITEEC.bin .\ROM\%1.bin
REM FU /CHECKSUM .\ROM\%1.bin

FU /+BIN ITEEC.BIN ODBrom.BIN EC192.BIN 10000

REM pause
REM FU /OFFSET MAIN.BIN 2000

move ITEEC.ALL .\ROM
move %1.bin .\ROM
move EC192.BIN .\ROM
copy .\ROM\%1.bin .\ROM\7QECXXWW.bin
FUCRC /WECRC .\ROM\7QECXXWW.bin 0 1FFFF 4A
pause
FU /CHECKSUM .\ROM\EC192.BIN
FU /CHECKSUM .\ROM\%1.bin
FU /CHECKSUM .\ROM\7QECXXWW.bin
DEL /q *.BIN*
DEL /q *.H*
DEL /q *.b0*
DEL /q *.b1
DEL /q *.b2
DEL /q *.b3
DEL /q *.ABS*
move *.m51 .\KeilMisc\Map
pause
cls

ECHO    ********************************************************************
ECHO    *    ITE Embedded Controller Firmware Build Process                *
ECHO    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
ECHO    ********************************************************************
ECHO.
ECHO    Making EC bin file successfully !!!
GOTO end


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
ECHO    USAGE:  build [P1] [P2]
ECHO                  P1 = The project name of OEM folder. or [clear]
ECHO                  P2 = [all] [ALL] []
ECHO.
GOTO done

REM ***********************************************************************************************************
REM	To clear obj, lst, and bin files.
REM ***********************************************************************************************************
:clear
del /q .\KeilMisc\Obj\*.*
del /q .\KeilMisc\\Lst\*.*
del /q .\ROM\*.*
GOTO done

::-----------------------------------------------------------
:: Fail
::-----------------------------------------------------------
:errorend
cd Code
cd OEM
del *.lst
del *.obj
cd..
cd..

cd Code
cd CORE
del *.lst
del *.obj
cd..
cd..
pause

:end
cd Code
cd OEM
del *.c
rd /S /Q INCLUDE
cd..
cd..

cd Code
cd CORE
del *.c
del *.a51
REM del *.src
cd..
cd..

::-----------------------------------------------------------
:: Done
::-----------------------------------------------------------
:done

@echo on