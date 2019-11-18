@echo off

del shar32.exe

set ICLCFG=icl64.cfg

set icl=C:\IntelB1054\bin\ia32\icl.bat /Qstd=c++0x /Dconstexpr=
set icl=C:\IntelH0721\bin-ia32\icl2a.bat 
set icl=C:\IntelI0124\bin-ia32\icl2a.bat 
set icl=C:\IntelJ0070\bin-ia32\icl2a.bat 
set icl=C:\IntelJ0117\bin-ia32\icl2b.bat 
set icl=C:\IntelJ2190\bin-ia32\icl2d.bat 

call %icl% shar.cpp zstd/zstd.cpp /Feshar32.exe

del *.exp *.obj
