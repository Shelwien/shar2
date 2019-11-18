@echo off

del shar.exe

perl txt2inc.pl usage.txt usage.inc

set ICLCFG=icl64.cfg

set icl=C:\IntelH0721\bin-intel64\icl2a.bat
set icl=C:\IntelI0124\bin-intel64\icl2a.bat 
set icl=C:\IntelJ0070\bin-intel64\icl2a.bat 
set icl=C:\IntelJ0117\bin-intel64\icl2a.bat 
set icl=C:\IntelJ2190\bin-intel64\icl2d.bat 

call %icl% shar.cpp zstd/zstd.cpp

del *.exp *.obj
