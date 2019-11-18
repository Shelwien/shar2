
rm -v shar.exe
g++ -std=gnu++11 -O2 -s -static -fno-exceptions -fno-rtti -fpermissive -fno-stack-protector -fno-stack-check -fno-check-new -I./Lib3 -D_mkdir=mkdir shar.cpp zstd/zstd.cpp -o shar.exe 2>err
ls -al shar.exe err

