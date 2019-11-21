@echo off

del shar.exe

set icl=C:\VC2015\bin\amd64\c2-17.bat /ILib3 /DCOMMON_SKIP_BSF src/setjmp.lib

call %icl% src/shar.cpp zstd/zstd.cpp

del *.exp *.obj
