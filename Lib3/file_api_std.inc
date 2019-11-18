
#if defined(_MSC_VER)
#define fseeko64 _fseeki64 
#define ftello64 _ftelli64
#endif

struct filehandle0 {

  FILE* f;

  operator int( void ) {
    return ((byte*)f)-((byte*)0);
  }

  template< typename CHAR >
  int open( const CHAR* name ) {
    if( strcmp(name,"-")==0 ) f = stdin; else
    f = fopen( name, "rb" );
    return ((byte*)f)-((byte*)0);
  }

  template< typename CHAR >
  int make( const CHAR* name ) {
    if( strcmp(name,"-")==0 ) f = stdout; else
    f = fopen( name, "wb" ); 
    return ((byte*)f)-((byte*)0);
  }

  int close( void ) {
    return fclose(f);
  }

  qword size( void ) { 
    qword p = tell();
    seek( 0, SEEK_END );
    qword s = tell();
    seek( p, SEEK_SET );
    return s; 
  }

  void seek( qword ofs, uint typ=SEEK_SET ) {
    fseeko64( f, ofs, typ );
  }

  qword tell( void ) {
    return ftello64(f);
  }

  template< typename BUF >
  uint read( BUF& buf ) {
    return read( &buf, sizeof(buf) )!=sizeof(buf);
  }

  uint read( void* _buf, uint len ) {
    return fread(_buf,1,len,f);
  }

  template< typename BUF >
  uint sread( BUF& buf ) { return sread( &buf, sizeof(buf) )!=sizeof(buf); }

  uint sread( void* _buf, uint len ) { 
    byte* buf = (byte*)_buf;
    uint l=0, r;

    do {
      r = read( buf+l, len-l );
      l += r;
    } while( (r>0) && (l<len) );

    return l;
  }

  int Getc( void ) {
    byte c;
    uint l = read(c);
    return l ? -1 : c;
  }

  uint writ( void* _buf, uint len ) {
    return fwrite( _buf, 1,len, f );
  }

  template< typename BUF >
  uint writ( BUF& buf ) {
    return writ( &buf, sizeof(buf) )!=sizeof(buf);
  }

};


struct filehandle : filehandle0 {
  filehandle() { f=0; }

  filehandle( int ) { f=0; }

  template< typename CHAR >
  filehandle( const CHAR* name, uint f_wr=0 ) {
    f_wr ? make(name) : open(name);
  }
};


