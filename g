
rm -v shar64
g++ -std=gnu++11 -O2 -s -static -fno-exceptions -fno-rtti -fpermissive -fno-stack-protector -fno-stack-check -fno-check-new -I./Lib3 -D_mkdir=mkdir src/shar.cpp zstd/zstd.cpp -o shar64 2>err
ls -al shar64 err

rm -v shar32
g++ -m32 -std=gnu++11 -O2 -s -static -fno-exceptions -fno-rtti -fpermissive -fno-stack-protector -fno-stack-check -fno-check-new -I./Lib3 -D_mkdir=mkdir src/shar.cpp zstd/zstd.cpp -o shar32 2>err32
ls -al shar32 err32

