
struct memblk;

enum{ 
  memblksize=1<<13,
  memblklen = memblksize - sizeof(memblk*) - sizeof(uint)
};

struct memblk {
  memblk* next;
  uint len;
  byte data[memblklen];
};

