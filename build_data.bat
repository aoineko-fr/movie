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
if not exist %Dest%/img ( md %Dest%/img )

for %%I in (datasrc/img/*.png) do (
	%MSXtk%\MSXimg.exe datasrc/img/%%I -pos 0 0 -size 256 144 -bpc 4 -format bin -pal custom -out %Dest%/img/%%~nI.bin
)