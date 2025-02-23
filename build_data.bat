@echo off
:: ____________________________
:: ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
:: ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
:: █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
:: ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
::  Data cooking
::─────────────────────────────────────────────────────────────────────────────

:: Path
set Tools=..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set Dest=content

if not exist %Dest% ( md %Dest% )
@REM if not exist %Dest%/img ( md %Dest%/img )

@REM for %%I in (datasrc/img/*.png) do (
@REM 	%MSXtk%\MSXimg.exe datasrc/img/%%I -pos 0 0 -size 256 144 -bpc 4 -format bin -pal custom -out %Dest%/img/%%~nI.bin
@REM )

%MSXtk%\MSXimg.exe datasrc/img -out content/JoyAndHeron.h -mode mglv -format c -size 256 144 -bpc 4 -pal msx1 -mglvSeg 16 -mglvTime 6 -mglvLoop -mglvMinSkip 8 -mglvMinFill 8
%MSXtk%\MSXimg.exe datasrc/img -out content/JoyAndHeron.mglv -mode mglv -format bin -size 256 144 -bpc 4 -pal msx1 -mglvSeg 16 -mglvTime 6 -mglvLoop -mglvMinSkip 8 -mglvMinFill 8
