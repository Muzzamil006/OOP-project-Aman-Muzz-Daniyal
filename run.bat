@echo off
setlocal
cd /d "%~dp0"
title Teen Talwar

set "TOOLS=%LOCALAPPDATA%\TeenTalwar"
set "SFML_URL=https://github.com/SFML/SFML/releases/download/2.6.1/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip"
set "GCC_URL=https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-msvcrt-r5/winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64msvcrt-11.0.0-r5.zip"

call :findSFML
if defined SFML_VC if not defined SFML echo Found SFML at %SFML_VC% but it is the Visual Studio version. Getting the MinGW version instead.
if not defined SFML call :download "%SFML_URL%" "%TOOLS%\sfml.zip" "SFML 2.6.1, about 20 MB"
if not defined SFML call :findSFML
if not defined SFML goto noSFML

call :findCompiler
if not defined GPP call :download "%GCC_URL%" "%TOOLS%\gcc.zip" "the g++ 13.1.0 compiler, about 220 MB, this can take a few minutes"
if not defined GPP call :findCompiler
if not defined GPP goto noCompiler
set "PATH=%GPP%;%PATH%"

if not exist "sfml-graphics-2.dll" copy /y "%SFML%\bin\*.dll" . >nul

powershell -NoProfile -Command "$exe = Get-Item 'TeenTalwar.exe' -ErrorAction SilentlyContinue; if (-not $exe) { exit 1 }; $changed = Get-ChildItem src, include -File | Where-Object { $_.LastWriteTime -gt $exe.LastWriteTime }; if ($changed) { exit 1 } else { exit 0 }"
if errorlevel 1 goto compile
goto run

:compile
echo SFML:     %SFML%
echo Compiler: %GPP%
echo Compiling Teen Talwar, please wait...
g++ -std=c++17 -Iinclude -I"%SFML%\include" src\*.cpp -o TeenTalwar.exe -L"%SFML%\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
if errorlevel 1 goto compileFailed
echo Done.

:run
TeenTalwar.exe
exit /b 0

:findSFML
set "SFML="
set "SFML_VC="
for %%R in ("%~dp0." "%~dp0.." "%~dp0..\.." "%~dp0..\..\.." "%~dp0..\..\..\.." "C:" "%USERPROFILE%" "%USERPROFILE%\Downloads" "%USERPROFILE%\Desktop" "%USERPROFILE%\Documents" "%OneDrive%" "%OneDrive%\Desktop" "%OneDrive%\Documents" "%TOOLS%") do call :scanSFML "%%~R"
if not defined SFML call :deepSearchSFML
exit /b 0

:scanSFML
if defined SFML exit /b 0
for /f "delims=" %%A in ('dir /b /ad "%~1\SFML*" 2^>nul') do call :scanOne "%~1\%%A"
exit /b 0

:scanOne
call :trySFML "%~1"
for /f "delims=" %%B in ('dir /b /ad "%~1\SFML*" 2^>nul') do call :trySFML "%~1\%%B"
exit /b 0

:deepSearchSFML
echo Searching your user folder for SFML, this can take a moment...
for /f "usebackq delims=" %%S in (`powershell -NoProfile -Command "Get-ChildItem -Path $env:USERPROFILE -Recurse -Depth 6 -Directory -Filter 'SFML*' -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }"`) do call :trySFML "%%S"
exit /b 0

:trySFML
if defined SFML exit /b 0
if not exist "%~1\include\SFML\Graphics.hpp" exit /b 0
findstr /c:"SFML_VERSION_MAJOR 2" "%~1\include\SFML\Config.hpp" >nul 2>nul
if errorlevel 1 exit /b 0
if not exist "%~1\lib\libsfml-graphics.a" set "SFML_VC=%~1"
if not exist "%~1\lib\libsfml-graphics.a" exit /b 0
set "SFML=%~1"
exit /b 0

:findCompiler
set "GPP="
for %%D in ("C:\mingw64\bin" "%~dp0mingw64\bin" "%TOOLS%\mingw64\bin") do call :tryGpp "%%~D"
if defined GPP exit /b 0
for /f "delims=" %%P in ('where g++ 2^>nul') do call :tryGpp "%%~dpP."
exit /b 0

:tryGpp
if defined GPP exit /b 0
if not exist "%~1\g++.exe" exit /b 0
"%~1\g++.exe" --version 2>nul | findstr /c:"13.1.0" >nul
if errorlevel 1 exit /b 0
set "GPP=%~1"
exit /b 0

:download
echo.
echo Downloading %~3 ...
if not exist "%TOOLS%" mkdir "%TOOLS%"
curl.exe -L --fail -o "%~2" "%~1"
if errorlevel 1 powershell -NoProfile -Command "$ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest -Uri '%~1' -OutFile '%~2'"
if not exist "%~2" exit /b 1
echo Extracting...
tar -xf "%~2" -C "%TOOLS%"
if errorlevel 1 powershell -NoProfile -Command "Expand-Archive -Path '%~2' -DestinationPath '%TOOLS%' -Force"
del "%~2" >nul 2>nul
exit /b 0

:noSFML
echo.
echo Could not find or download SFML 2.6.
echo Copy your SFML-2.6.x folder - the one that contains include, lib and bin -
echo into this game folder, next to run.bat, and run it again.
pause
exit /b 1

:noCompiler
echo.
echo Could not find or download the g++ 13.1.0 compiler.
echo Download WinLibs MSVCRT 13.1.0 64-bit from the SFML 2.6.1 download page
echo and put the mingw64 folder at C:\mingw64 so this file exists:
echo C:\mingw64\bin\g++.exe
pause
exit /b 1

:compileFailed
echo.
echo Compiling failed. Read the errors above.
pause
exit /b 1
