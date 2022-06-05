@echo off
setlocal enableDelayedExpansion

:find_folder_name
set temp=%cd%
set folder=
:loop
    if not "x%temp:~-1%"=="x\" (
        set folder=!temp:~-1!!folder!
        set temp=!temp:~0,-1!
        goto :loop
    )
    echo.folder   = %folder%

echo %folder%
if exist game\stdafx.h goto start
echo Run this script from the project root folder (where *.SLN file is stored)
goto abort

:start
echo This script will clean your solution by removing all non-essential files
echo and prepare a packaged ZIP file for submission.
choice /M "Do you want to continue"
if errorlevel 2 goto eof

if exist *.suo del /A:H *.suo 
if exist *.sdf del *.sdf 
if exist *.opendb del *.opendb
if exist *.VC.db del *.VC.db
if exist .vs\*.* rmdir /S /Q .vs
if exist Debug\*.* rmdir /S /Q Debug 
if exist Release\*.* rmdir /S /Q Release 
if exist game\Debug\*.* rmdir /S /Q game\Debug 
if exist game\Release\*.* rmdir /S /Q game\Release 
if exist ipch\*.* rmdir /S /Q ipch 
if exist %folder%.zip del %folder%.zip

echo src = "game"> __tmpJustZipCI4510.vbs
echo sln = "*.sln">> __tmpJustZipCI4510.vbs
echo zipname = "%folder%">> __tmpJustZipCI4510.vbs
echo bat1 = "_clean.bat">> __tmpJustZipCI4510.vbs
echo bat2 = "_pack.bat">> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs
echo 'Get current directory>> __tmpJustZipCI4510.vbs
echo Set fso = CreateObject("Scripting.FileSystemObject")>> __tmpJustZipCI4510.vbs
echo Dim curdir>> __tmpJustZipCI4510.vbs
echo curdir = fso.GetAbsolutePathName(".")>> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs
echo 'FNames>> __tmpJustZipCI4510.vbs
set "Line1=srcPath = curdir & "\" & src"
set "Line2=slnPath = curdir & "\" & sln"
set "Line3=zipFile = curdir & "\" & zipname & ".zip""
set "Line4=bat1File = curdir & "\" & bat1"
set "Line5=bat2File = curdir & "\" & bat2"
echo !Line1!>> __tmpJustZipCI4510.vbs
echo !Line2!>> __tmpJustZipCI4510.vbs
echo !Line3!>> __tmpJustZipCI4510.vbs
echo !Line4!>> __tmpJustZipCI4510.vbs
echo !Line5!>> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs
echo 'Create empty ZIP file.>> __tmpJustZipCI4510.vbs
echo Set fso = CreateObject("Scripting.FileSystemObject")>> __tmpJustZipCI4510.vbs
set "Line=fso.CreateTextFile(zipFile, True).Write "PK" & Chr(5) & Chr(6) & String(18, vbNullChar)"
echo !Line!>> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs
echo 'Copy files to ZIP file>> __tmpJustZipCI4510.vbs
echo WScript.Echo("Compressing project files...")>> __tmpJustZipCI4510.vbs
echo WScript.Echo("It can take a few minutes.")>> __tmpJustZipCI4510.vbs
echo Set objShell = CreateObject("Shell.Application")>> __tmpJustZipCI4510.vbs

echo objShell.NameSpace(zipFile).CopyHere(srcPath)>> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs
echo 'Keep script waiting until compression is done>> __tmpJustZipCI4510.vbs
echo Do Until objShell.NameSpace(zipFile).Items.Count = 1 >> __tmpJustZipCI4510.vbs
echo     wScript.Sleep 200>> __tmpJustZipCI4510.vbs
echo Loop>> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs
echo 'Copy sln files>> __tmpJustZipCI4510.vbs
echo For Each f In fso.GetFolder(".").Files>> __tmpJustZipCI4510.vbs
echo 	If UCase(fso.GetExtensionName(f.Name)) = Ucase("SLN") Then >> __tmpJustZipCI4510.vbs
echo                 WScript.Echo("Compressing SLN file...")>> __tmpJustZipCI4510.vbs
echo 		objShell.NameSpace(zipFile).CopyHere(f.Path)>> __tmpJustZipCI4510.vbs
echo 		WScript.Sleep 250>> __tmpJustZipCI4510.vbs
echo 		'Exit For>> __tmpJustZipCI4510.vbs
echo 	End If>> __tmpJustZipCI4510.vbs
echo Next>> __tmpJustZipCI4510.vbs
echo  WScript.Sleep 500>> __tmpJustZipCI4510.vbs
echo objShell.NameSpace(zipFile).CopyHere(bat1File)>> __tmpJustZipCI4510.vbs
echo WScript.Sleep 500>> __tmpJustZipCI4510.vbs
echo objShell.NameSpace(zipFile).CopyHere(bat2File)>> __tmpJustZipCI4510.vbs
echo WScript.Sleep 500>> __tmpJustZipCI4510.vbs
echo WScript.Echo("Finalising...")>> __tmpJustZipCI4510.vbs
echo WScript.Sleep 1000>> __tmpJustZipCI4510.vbs
echo.>> __tmpJustZipCI4510.vbs

CScript  __tmpJustZipCI4510.vbs //Nologo
if exist __tmpJustZipCI4510.vbs del __tmpJustZipCI4510.vbs

if exist %folder%.zip goto success

echo Submission file not created.
goto abort

:success
echo.
echo Your files have been successfully packed into the following file: %folder%.zip
echo Please submit this file only.
pause
goto eof

:abort
echo Aborting...
if exist __tmpJustZipCI4510.vbs del __tmpJustZipCI4510.vbs
if exist %folder%.zip del %folder%.zip
dir /s > dump.txt
echo PROCESS ABORTED. Report stored in file: dump.txt
pause
goto eof

:eof