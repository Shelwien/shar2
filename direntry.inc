
const char signature[] = "Shar2\n\x1A";
const uint sign_len = sizeof(signature);

template< class Coroutine >
uint vlen_put( Coroutine& C, qword x ) {
  uint len = 0;
  while(1) {
    C.put( (x&0x7F) | ((x>0x7F)<<7) ); len++;
    x>>=7;
    if( x==0 ) break;
  }
  return len;
}

template< class Coroutine >
qword vlen_get( Coroutine& C ) {  
  uint c,i=0;
  qword x=0;
  while(1) {
    c = byte(C.get());
    x |= qword(c&0x7F)<<i; i+=7;
    if( (c&0x80)==0 ) break;
  }
  return x;
}

struct entry {

  uint Parent; // directory index
  qword usize; // unpacked size
//  qword csize; // packed size

  char type; // entry type: 'F'=file, 'D'=dir

//  char method; // compression method: 'c'=copy, 'z'=zstd

  char* name;

  char namebuf[dirscan::MAX_FNAME];

  void init( uint f_dir=0, char* fnam=0, uint _parent=0 ) {
    Parent = _parent;
    usize = 0; 
//    csize = 0;
    type = f_dir?'D':'F'; 
//    method = 'c';
    name = (fnam==0) ? namebuf : fnam;
  }

  // return the number of written bytes
  template< class Coroutine >
  uint store( Coroutine& C ) {
    uint i,l=0;
    l += vlen_put( C, Parent );
    l++; C.put( type );
//    C.put( method );
//    if( type!='D' ) l += vlen_put( C, usize );

    for( i=0;; i++ ) { l++; C.put( name[i] ); if( name[i]==0 ) break; }
    return l;
  }

  template< class Coroutine >
  uint parse( Coroutine& C ) {
    uint i;
    C.chkinp(); if( C.getinplen()==0 ) return 1;
    name = namebuf;
    Parent = vlen_get(C);
    type = C.get();
//    if( type!='D' ) usize=vlen_get(C); else 
    usize=0;
//    method = C.get();
    for( i=0; i<sizeof(namebuf); i++ ) { name[i]=C.get(); if( name[i]==0 ) break; }
    return 0;
  }

};
