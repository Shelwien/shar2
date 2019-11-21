@echo off

del *.exe

set incs=-DNDEBUG -DSTRICT -DNDEBUG -DWIN32 -I./Lib3 -D_mkdir=mkdir
rem -march=sse2 -mtune=sse2 -msse2

set opts=-fstrict-aliasing -fomit-frame-pointer -ffast-math ^
-fno-exceptions -fno-rtti -fpermissive  ^
-fno-stack-protector -fno-stack-check -fno-check-new -ftree-vectorize


rem -fprofile-use -fprofile-correction 

rem -flto -ffat-lto-objects -Wl,-flto -fuse-linker-plugin -Wl,-O -Wl,--sort-common -Wl,--as-needed -ffunction-sections

:set gcc=C:\MinGW710\bin\g++.exe -m32 
set gcc=C:\MinGW810\bin\g++.exe -m32
set gcc=C:\MinGW810x\bin\g++.exe 
set gcc=C:\MinGW820x\bin\g++.exe -march=k8
set gcc=C:\MinGW820\bin\g++.exe -march=pentium2 -m32
set path=%gcc%\..\

del *.exe *.o

%gcc% -std=gnu++11 -O9 -s %incs% %opts% -static src/shar.cpp zstd/zstd.cpp -o shar32.exe

