
#ifdef _WIN32
#include <windows.h>
#include <locale.h>
#endif

#include "common.inc"
#include <io.h>
#include <direct.h>

#ifdef _WIN32
//#include "file_api_win.inc"
#include "file_api_std.inc"
#else
#include "file_api_std.inc"
#endif

#include "coro3b.inc"

#include "stream_wrapper.inc"
#include "coro2pair.inc"

#include "coro_fhp2.inc"
#include "coro_fh_inp.inc"
#include "coro_fh_out.inc"

#include "memblk.inc"

#include "dir_scan.inc"
#include "direntry.inc"

#define FILE BUGG // have to use file_api

#include "archive.inc"

#include "../zstd/zstd.inc"

template< int f_DEC >
struct copy_Codec : Coroutine {

  enum{ ID='C' };

  uint Init( uint cLevel=0, uint winlog=-1 ) {
    coro_init(); 
    return 0;
  }

  void Quit( void ) {
  }

  void do_process( void ) {
    uint c;

    while(1) {
      c = get(); if( c==-1 ) break; 
      put( c );
    }

    yield(this,0);
  }

};

ALIGN(4096) 
static union {
  Archive< zstd_Codec > A_zstd;
  Archive< copy_Codec > A_copy;
};

#include "usage.inc"

// shar a|x archive path

int main_utf8( int argc, char** argv ) {

  uint cfg_level = 3;
  uint cfg_memory = 23; // 1<<23

  if( argc<3 ) {
    printf( msg_logo );
    printf( msg_usage );
    return 1;
  }

  uint r, f_UNP = (argv[1][0]=='x');
  char* path = (char*)( argc<4 ? f_UNP? "" : "./" : argv[3] );

  if( strlen(argv[1])>=2 ) cfg_level=atoi(&argv[1][1]);

  if( f_UNP ) {
    filehandle f( argv[2] ); if( f==0 ) return 2; // for actual data

    r = A_zstd.Shar_Extract( f, path, 0 );

    if( r==3 ) {
      fprintf(stderr,"Signature mismatch!\n" );
      return 3;
    }

    switch( r ) {
    case 0: break; // done
    case zstd_Codec<0>::ID: 
      r = A_zstd.Shar_Extract( f, path, r );
      break;
    case copy_Codec<0>::ID: 
      r = A_copy.Shar_Extract( f, path, r );
      break;
    default:
      fprintf(stderr,"Unknown compression method!\n" );
      return 3;
    };

  } else {

    filehandle g( argv[2], 1 ); if( g==0 ) return 2;
  
    if( cfg_level==0 ) {
      r=A_copy.Shar_Create( g, path, 0, cfg_memory );
    } else {
      r=A_zstd.Shar_Create( g, path, cfg_level, cfg_memory );
    }
  }

  return 0;
}

#include "main_utf8_wrap.inc"

