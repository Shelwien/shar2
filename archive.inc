
template< template< int f_DEC > class zstd_Codec >
struct Archive {

  enum{ pathbufsize=1<<16 };
  char pathbuf[pathbufsize];

  struct zstd_Codec_Enc : CoroutinePair< zstd_Codec<0>, Wrapper<0,0x9A> > {
    uint Init( uint cLevel=0, uint winlog=-1 ) { init(); return this->M1.Init(cLevel,winlog); }
    void Quit( void ) { return this->M1.Quit(); }
  };


  struct zstd_Codec_Dec : CoroutinePair< Wrapper<1,0x9A>, zstd_Codec<1> > {
    uint Init( uint cLevel=0, uint winlog=-1 ) { init(); return this->M2.Init(cLevel,winlog); }
    void Quit( void ) { return this->M2.Quit(); }
  };

  #include "dirstore.inc"
  #include "dirparse.inc"
  #include "dirparse2.inc"

  ALIGN(4096) union {
    dirstore A;
    CoroFileProc< CoroutinePair<zstd_Codec_Dec,dirparse> > X;
    CoroFileProc< dirparse2 > X2;
  };

  NOINLINE
  uint Shar_Extract( filehandle& f, char* path, uint _r=0 ) {
    uint i,c, r=0, cfg_level=0, cfg_memory=0;

    if( _r!=0 ) r=_r; else {
      char chksign[sign_len];
      f.read( chksign, sign_len ); chksign[sign_len-1]=0;
      if( strcmp(chksign,signature)!=0 ) return 3;
      f.read( &r, 1 ); // codec type
    }

    if( r!=zstd_Codec<0>::ID ) return r;

    // create target path
    strcpy( pathbuf, path );
    for( i=0;; i++ ) {
      c = pathbuf[i];
      if( (i>0) && ((c=='/')||(c=='\\')||(c==0)) ) {
        if( pathbuf[i-1]!=':' ) {
          pathbuf[i]=0;
          _mkdir( pathbuf );
          pathbuf[i]=c;
        }
      }
      if( c==0 ) break;
    }

    X.M1.Init( cfg_level, cfg_memory ); // zstd
    X.M2.init( pathbuf );
    X.processfile( f, 0 );
    X.M1.Quit();

    // ren/move dumped files to proper subdirs and names
    X2.init( pathbuf );
    X2.processfile( f, 0, X.M1.inpptr, X.M1.inpend );

    return 0;
  }

  NOINLINE
  uint Shar_Create( filehandle& f, char* path, uint cfg_level, uint cfg_memory ) {
    uint r;
    filehandle& g = f;

    A.Z.Init( cfg_level, cfg_memory );

    g.writ( (void*)signature, sign_len );

    r = zstd_Codec<0>::ID;
    g.writ( &r, 1 );

    A.init( path );
    A.processfile( g );
    A.flush_out2( g );

    A.Z.Quit();
  }

};
