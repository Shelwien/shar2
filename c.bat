@echo off

del shar.exe

perl txt2inc.pl src/usage.txt src/usage.inc

set ICLCFG=icl64.cfg

set icl=C:\IntelH0721\bin-intel64\icl2a.bat
set icl=C:\IntelI0124\bin-intel64\icl2a.bat 
set icl=C:\IntelJ0070\bin-intel64\icl2a.bat 
set icl=C:\IntelJ0117\bin-intel64\icl2a.bat 
set icl=C:\VC2015\bin\amd64\c2-17.bat /ILib3 /DCOMMON_SKIP_BSF setjmp.lib
set icl=C:\IntelJ2190\bin-intel64\icl2d.bat 

call %icl% src/shar.cpp zstd/zstd.cpp

del *.exp *.obj
