@echo off
REM Build wrapper for hash-bench-3ds (devkitARM + libctru).

setlocal enableextensions enabledelayedexpansion

set DEVKITPRO=C:/devkitPro
set DEVKITARM=C:/devkitPro/devkitARM
set PATH=C:\devkitPro\msys2\usr\bin;C:\devkitPro\devkitARM\bin;C:\devkitPro\tools\bin;%PATH%

if not exist "C:\devkitPro\devkitARM\bin\arm-none-eabi-gcc.exe" (
    echo ERROR: devkitARM not found under C:\devkitPro\devkitARM.
    exit /b 1
)
if not exist "C:\devkitPro\libctru\include\3ds.h" (
    echo ERROR: libctru not found - install devkitPro 3ds-dev meta-package.
    exit /b 1
)

cd /d "%~dp0"

if "%1"=="clean" (
    make clean 2>nul
    exit /b 0
)

make 2>nul

set OUTNAME=hash-bench-3ds.3dsx

if not exist "%OUTNAME%" (
    echo.
    echo Build FAILED - no .3dsx produced.
    exit /b 1
)

if not exist "%~dp0artifacts" mkdir "%~dp0artifacts"
copy /Y "%OUTNAME%" "%~dp0artifacts\%OUTNAME%" >nul
if exist hash-bench-3ds.smdh copy /Y hash-bench-3ds.smdh "%~dp0artifacts\" >nul

echo.
echo Build OK. Fresh ROM at:
echo     %~dp0%OUTNAME%
echo     %~dp0artifacts\%OUTNAME%

endlocal
