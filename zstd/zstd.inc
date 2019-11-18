
#if 1

#define ZSTD_STATIC_LINKING_ONLY
#include "zstd.h"

#else

#include "error_private.inc"
#include "entropy_common.inc"

#include "fse_compress.inc"
#include "fse_decompress.inc"

#include "huf_compress.inc"
#include "huf_decompress.inc"

#include "zstd_common.inc"

#include "zbuff_compress.inc"
#include "zbuff_decompress.inc"

#include "zstd_compress.inc"
#include "zstd_decompress.inc"

#endif

template< int f_DEC > 
struct zstd_Codec : Coroutine {

  enum{ ID='Z' };

  ZSTD_CStream* cstream;
  ZSTD_DStream* dstream;

  uint Init( uint cLevel=0, uint winlog=-1 ) {
    coro_init(); 
//    D.C=this; D.total=0;
    if( f_DEC==0 ) return cInit(cLevel,winlog); else return dInit();
  }

  void Quit( void ) {
    if( f_DEC==0 ) cQuit(); else cQuit();
  }

  void do_process( void ) {
    if( f_DEC==0 ) compressFile(); else decompressFile();
    yield(this,0);
  }


  uint _initCStream( ZSTD_CStream* zcs, int compressionLevel, uint winlog ) {
    ZSTD_parameters params = ZSTD_getParams( compressionLevel, 0, 0 );
    if( winlog!=-1 ) {
      params.cParams.windowLog=winlog;
      params.cParams.chainLog = Min<uint>(winlog,params.cParams.chainLog);
      params.cParams.hashLog = Min<uint>(winlog,params.cParams.hashLog);
    }
    return ZSTD_initCStream_advanced(zcs, 0, 0, params, 0);
  }

  // r0 on success
  uint cInit( uint cLevel, uint winlog ) {
    uint r;
    //ZSTD_customMem dummy = { dum_malloc, dum_free, &D }; cstream = ZSTD_createCStream_advanced( dummy ); 
    cstream = ZSTD_createCStream(); 
    if( cstream==0 ) r=10; else r = _initCStream(cstream, cLevel, winlog); 
    return r;
  }

  void cQuit( void ) {
    ZSTD_freeCStream(cstream);
  }

  uint dInit( void ) {
    uint r;
    dstream = ZSTD_createDStream(); 
    if( dstream==0 ) r=10; else {
      r = ZSTD_initDStream(dstream); if( r==5 ) r=0;
    }
    return r;
  }

  void dQuit( void ) {
    ZSTD_freeDStream(dstream);
  }



  void compressFile(void) {

    size_t read, toRead;
    byte* buffIn;

    while(1) {
      chkinp(); read=getinplen(); buffIn=(byte*)inpptr; inpptr+=read;
      if( read==0 ) break;

      ZSTD_inBuffer input = { buffIn, read, 0 };
      while( input.pos < input.size ) {
        //ZSTD_outBuffer output = { buffOut, buffOutSize, 0 };
        chkout(); ZSTD_outBuffer output = { (byte*)outptr, getoutlen(), 0 };
        toRead = ZSTD_compressStream( cstream, &output, &input );
//printf( "%i/%i %i/%i %i\n", input.pos,input.size, output.pos,output.size, toRead );
        if( ZSTD_isError(toRead) ) yield(this,3);
        outptr+=output.pos;
      }
    }

    while(1) {
      chkout(); ZSTD_outBuffer output = { (byte*)outptr, getoutlen(), 0 };
      toRead = ZSTD_endStream( cstream, &output );
      outptr+=output.pos;
      if( toRead==0 ) break;
    }

  }

  void decompressFile(void) {

    uint read, toRead=-1;
    byte* buffIn;

    while(1) {
      chkinp(); read=Min<uint>(toRead,getinplen()); buffIn=(byte*)inpptr; inpptr+=read;
      if( read==0 ) break;

      ZSTD_inBuffer input = { buffIn, read, 0 };
      while( input.pos<input.size ) {
        chkout(); ZSTD_outBuffer output = { (byte*)outptr, getoutlen(), 0 };

        // toRead : size of next compressed block
        toRead = ZSTD_decompressStream( dstream, &output , &input );  
        if( ZSTD_isError(toRead) ) yield(this,3);

        outptr+=output.pos;
      }
    }

  }

};
