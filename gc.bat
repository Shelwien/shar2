@echo off

set LIB=C:\VC2015\lib\amd64;C:\VC2015\sdk\Lib\x64;C:\VC2015\ucrt\x64

set incs=-DSTRICT -DNDEBUG -DWIN32 -D_WIN32 -DBIT64 -I./Lib3 -DCOMMON_SKIP_BSF -DUNICODE -D_UNICODE ^
-D_CRT_SECURE_NO_WARNINGS ^
-D_CRT_SECURE_NO_DEPRECATE ^
-D_CRT_DISABLE_PERFCRIT_LOCKS ^
-D_CRT_NONSTDC_NO_DEPRECATE ^
-D_SECURE_SCL=0 ^
-D_ITERATOR_DEBUG_LEVEL=0 ^
-D_SECURE_SCL_THROWS=0 ^
-D_HAS_ITERATOR_DEBUGGING=0 ^
-I. -IC:\VC2019\include ^
-IC:\VC2019\ucrt\include ^
-IC:\VC2019\sdk\Include ^
-DUNICODE -D_UNICODE


rem -fstrict-aliasing 
set opts=-fomit-frame-pointer -fno-stack-protector -fno-stack-check -fgnu-keywords -Wno-invalid-token-paste

set arch=-march=k8 -mtune=k8 -fms-compatibility -fms-compatibility-version=19 -fms-extensions -Wno-ignored-attributes -m64

set gcc=C:\clang801x\bin\clang++.exe

del *.exe *.o

%gcc% -v -s %arch% -O9 -std=c++1y %incs% %opts% shar.cpp zstd/zstd.cpp C:\VC2019\lib\amd64\oldnames.lib -static -o shar.exe

del *.o



