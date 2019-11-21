
struct dirstore : Coroutine {
  #define xOut pin[2]

  qword n_bytes;
  memblk* h_blk;

  entry E;

  dirscan D;

  zstd_Codec_Enc Z;

  enum{ inpbufsize=1<<16, outbufsize=1<<16,  bufsize=1<<16 };
  ALIGN(4096) memblk out2buf;
  ALIGN(4096) byte inpbuf[inpbufsize];
  ALIGN(4096) byte outbuf[outbufsize];
  ALIGN(4096) byte Zoutbuf[outbufsize];
  ALIGN(4096) byte buf[bufsize];

  CoroFileProcInp< Wrapper<0> > W;

  uint init( char* path ) {
    n_bytes = 0;
    coro_init();
    uint r = D.init(path);
    return r;
  }

  void do_process( void ) {
    uint i,l,n;
    filehandle f;

    while( D.scan() ) {

      E.init( D.f_dir, D.name, D.i_parent );

      if( E.type!='D' ) {
        // copy a file to archive
        f.open( D.pathbuf ); if( f ) {
          E.usize += W.processfile( f, this );
          f.close();
        } // if open
      } // if file

      fprintf(stderr, "!%c!%i!%s!%i!\n", E.type, D.i_parent, D.pathbuf, uint(E.usize) );

      n_bytes += E.store(xOut);
    }

    --D.i_file; // started from 1
    fprintf(stderr, "N_files=%i\n", D.i_file );
    fprintf(stderr, "n_bytes=%i\n", uint(n_bytes) );

//    l = vlen_put( xOut, D.i_file );
//    l+= vlen_put( xOut, n_bytes );
//    xOut.put( l );

    yield(this,0);
  }

  void flush_out2( filehandle& h ) {
    h_blk = &out2buf;
    while( h_blk!=0 ) {
      memblk& p = *h_blk;
      h.writ( p.data, p.len );
      h_blk = p.next;
    }
  }

  void new_memblk( memblk* p ) {
    if( h_blk!=0 ) h_blk->next = p; 
    h_blk = p;
    xOut.addout( h_blk->data, memblklen ); 
    h_blk->len=0;
    h_blk->next=0;
  }

  void processfile( filehandle g ) {
    uint l=0,r,rz;
    coro_init();
    addout( outbuf, outbufsize );
    Z.addout( Zoutbuf, outbufsize );
    h_blk=0; new_memblk( &out2buf );

    while( 1 ) {
      r = coro_call(this); 

      if( r==1 ) break; // shouldn't happen

      if( (r==2) || (r==0) ) {

        l = getoutsize();
ZRetry:
        Z.addinp( (byte*)outbeg, l ); if( r==0 ) Z.f_quit=1;

        while(1) {
          rz = Z.coro_call(&Z);
          if( rz==1 ) break;
          if( g ) g.writ( Zoutbuf, Z.getoutsize() );
          Z.addout( Zoutbuf, outbufsize );
          if( rz==0 ) break;
        }
        if( (r==0) && (rz==1) ) { l=0; Z.f_quit=1; goto ZRetry; }

        addout( outbuf, outbufsize );
      }
  
      if( (r==3) || (r==0) ) {
        h_blk->len = xOut.getoutsize();
        if( r==3 ) {
          memblk* p = new memblk; if( p==0 ) break; // memory error
          new_memblk( p );
        }
      }

      if( r==0 ) break;
    }

  } // processfile

  #undef xOut
};

