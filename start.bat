:: Copyright Mac Clayton 2017

@echo off
set PROJECT_ROOT=%~dp0

:: Libraries

:: qt5
set QT5_ROOT=C:\Qt\5.10.0\msvc2017_64
set QT5_BIN=%QT5_ROOT%\bin
set QT5_INCLUDES=%QT5_ROOT%\include
set QT5_LIBDIR=%QT5_ROOT%\lib
set QT5_XCOMPILE=1

:: FreeGLUT
set FREEGLUT_ROOT=C:\Developer\libs\freeglut
set FREEGLUT_INCLUDES=%FREEGLUT_ROOT%\include
set FREEGLUT_LIBDIR=%FREEGLUT_ROOT%\lib\x64
set FREEGLUT_BIN=%FREEGLUT_ROOT%\bin\x64

:: Waf
set WAF_EXECUTABLE=%PROJECT_ROOT%\crank\build\waf

:: Path
set PATH=%QT5_BIN%;%QT5_LIBDIR%;%FREEGLUT_BIN%;%PATH%

:: Python Path
set PYTHONPATH=%PROJECT_ROOT%\crank;%PROJECT_ROOT%\crank\libraries;%PYTHONPATH%

:: Shortcuts
doskey waf=python %WAF_EXECUTABLE% $*
