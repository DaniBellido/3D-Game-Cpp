@echo off
setlocal enableDelayedExpansion

:start
echo This script will clean your solution by removing all non-essential files.
choice /M "Continue"
if errorlevel 2 goto eof

if exist *.suo del /A:H *.suo 
if exist *.sdf del *.sdf 
if exist *.opendb del *.opendb
if exist *.VC.db del *.VC.db
if exist .vs\*.* rmdir /S /Q .vs
if exist Debug\*.* rmdir /S /Q Debug
if exist Release\*.* rmdir /S /Q Release
if exist GTEC3Dlib\Debug\*.* rmdir /S /Q GTEC3Dlib\Debug
if exist game\Release\*.* rmdir /S /Q game\Release
if exist ipch\*.* rmdir /S /Q ipch
echo.
echo All non-essential files have been removed.
echo.

:eof

