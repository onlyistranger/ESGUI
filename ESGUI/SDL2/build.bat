@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul 2>&1

echo ============================================
echo   ESGUI SDL2 Simulator - One-Click Build
echo ============================================
echo.

cd /d "%~dp0"

:: ============================================
::  1. Check CMake
:: ============================================
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found in PATH.
    echo Please install CMake from: https://cmake.org/download/
    echo Or: winget install Kitware.CMake
    pause
    exit /b 1
)
for /f "tokens=3" %%v in ('cmake --version ^| findstr /c:"cmake version"') do (
    echo [INFO] CMake version: %%v
)

:: ============================================
::  2. MinGW Detection (default generator)
:: ============================================
set MINGW_PATH=
set SDL2_ARGS=-G "MinGW Makefiles"

:: 2a. MSYS2 MinGW64
if exist "C:\msys64\mingw64\bin\gcc.exe" (
    set "MINGW_PATH=C:\msys64\mingw64\bin"
) else if exist "D:\msys64\mingw64\bin\gcc.exe" (
    set "MINGW_PATH=D:\msys64\mingw64\bin"
) else if exist "E:\msys64\mingw64\bin\gcc.exe" (
    set "MINGW_PATH=E:\msys64\mingw64\bin"
)

:: 2b. Standalone MinGW-w64
if not defined MINGW_PATH (
    for %%d in ("mingw64" "mingw32") do (
        for %%r in ("C:\%%~d" "D:\%%~d" "E:\%%~d" "C:\Program Files\%%~d" "C:\Program Files (x86)\%%~d") do (
            if exist "%%~r\bin\gcc.exe" (
                set "MINGW_PATH=%%~r\bin"
            )
        )
    )
)

:: 2c. PATH 中查找 gcc (MinGW 版本)
if not defined MINGW_PATH (
    for /f "delims=" %%p in ('where gcc 2^>nul') do (
        echo %%p | findstr /i "mingw" >nul
        if !errorlevel! equ 0 (
            for %%d in ("%%~dp.") do set "MINGW_PATH=%%~dp."
        )
    )
)

if defined MINGW_PATH (
    echo [INFO] MinGW found: !MINGW_PATH!
    set "PATH=!MINGW_PATH!;%PATH%"
    set "SDL2_ARGS=!SDL2_ARGS! -DCMAKE_C_COMPILER=!MINGW_PATH!\gcc.exe -DCMAKE_MAKE_PROGRAM=!MINGW_PATH!\mingw32-make.exe"
) else (
    echo [WARN] MinGW not found, using default CMake generator.
    set SDL2_ARGS=
)

:: ============================================
::  3. SDL2 Detection
:: ============================================

:: 3a. Try vcpkg (works with MinGW triplets)
if defined VCPKG_ROOT (
    set "VCPKG_TOOLCHAIN=!VCPKG_ROOT!\scripts\buildsystems\vcpkg.cmake"
) else if exist "%USERPROFILE%\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    set "VCPKG_TOOLCHAIN=%USERPROFILE%\vcpkg\scripts\buildsystems\vcpkg.cmake"
) else if exist "C:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    set "VCPKG_TOOLCHAIN=C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
) else if exist "D:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    set "VCPKG_TOOLCHAIN=D:\vcpkg\scripts\buildsystems\vcpkg.cmake"
)

if exist "!VCPKG_TOOLCHAIN!" (
    echo [INFO] vcpkg toolchain: !VCPKG_TOOLCHAIN!
    set "SDL2_ARGS=!SDL2_ARGS! -DCMAKE_TOOLCHAIN_FILE=!VCPKG_TOOLCHAIN!"
)

:: 3b. Standalone SDL2 (e.g. D:\SDL2)
if not defined SDL2_DIR (
    for %%d in ("C:\SDL2\cmake" "D:\SDL2\cmake" "E:\SDL2\cmake") do (
        if exist "%%~d\SDL2Config.cmake" (
            set "SDL2_ARGS=!SDL2_ARGS! -DSDL2_DIR=%%~d"
            echo [INFO] Standalone SDL2 found: %%~d
        )
    )
)

:: 3c. Check if SDL2 is available via pkg-config (MSYS2)
if defined MINGW_PATH (
    if exist "!MINGW_PATH!\pkg-config.exe" (
        "!MINGW_PATH!\pkg-config.exe" --exists sdl2 2>nul
        if !errorlevel! equ 0 (
            echo [INFO] SDL2 found via pkg-config
        )
    )
)

:configure
:: ============================================
::  3. CMake Configure
:: ============================================
if exist build (
    echo [INFO] Cleaning existing build directory...
    rmdir /s /q build
)

echo.
echo [INFO] Configuring...
cmake -S . -B build %SDL2_ARGS%
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] CMake configure failed.
    echo.
    echo Troubleshooting:
    echo   1. Install MSYS2:  https://www.msys2.org/
    echo      Then:  pacman -S mingw-w64-x86_64-{gcc,cmake,SDL2}
    echo   2. Or via vcpkg:   vcpkg install sdl2:x64-mingw-static
    echo   3. Or set SDL2_DIR manually:
    echo      cmake -S . -B build -DSDL2_DIR=C:/path/to/SDL2/cmake
    pause
    exit /b 1
)

:: ============================================
::  4. Build
:: ============================================
echo.
echo [INFO] Building...
cmake --build build --config Release
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed. Check errors above.
    pause
    exit /b 1
)

:: ============================================
::  5. Done
:: ============================================
echo.
echo ============================================
echo   Build successful!
echo.
echo   Binary: build\Release\esgui_sdl2.exe
echo           (or build\esgui_sdl2.exe)
echo ============================================
echo.
echo [INFO] Run: build\esgui_sdl2.exe
echo.

:: Copy SDL2.dll next to the executable for convenience
for /r build %%f in (esgui_sdl2.exe) do (
    set EXE_DIR=%%~dpf
    goto :found_exe
)
:found_exe

if defined VCPKG_TOOLCHAIN (
    echo [INFO] vcpkg users: SDL2.dll should be auto-found via PATH.
    echo [INFO] Run vcpkg integrate install to set up PATH automatically.
) else (
    echo [INFO] Make sure SDL2.dll is in PATH or next to the executable.
)

pause
endlocal
