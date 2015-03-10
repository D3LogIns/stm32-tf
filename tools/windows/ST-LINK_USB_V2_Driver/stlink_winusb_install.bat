@echo off
ver | find "6.2" > nul
if %ERRORLEVEL% == 0 goto WIN8
start "" ST-Link_V2_USBdriver.exe
goto END
:WIN8
if ("%PROCESSOR_ARCHITEW6432%" == "AMD64") OR ("%PROCESSOR_ARCHITECTURE%" == "AMD64") 
goto X64
start "" dpinst_x86.exe
goto END
:X64
start "" dpinst_amd64.exe
:END

