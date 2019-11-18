
template < class Model1, class Model2 > 
struct CoroutinePair : Coroutine {

  ALIGN(4096) Model1 M1;
  ALIGN(4096) Model2 M2;

  enum { midbufsize = 1<<16 };
  ALIGN(4096) byte midbuf[midbufsize+16];

  void init( void ) {
//printf( "%I64X: coropair init\n", this );
    coro_init();
    M1.coro_init();
    M2.coro_init();
  }

  void do_process( void ) {
    uint r1,r2,l;
    static byte* sEOF = (byte*)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";

//printf( "%I64X: inpptr=%I64X outptr=%I64X il=%i ol=%i \n", this, inpptr,outptr, getinplen(), getoutleft() ); fflush(stdout);

    M1.coro_init();
    M2.coro_init();

    M1.addout( midbuf, midbufsize );
    M2.addout( (byte*)outptr, getoutleft() );
    M2.coro_call(&M2); // should be at r1

//printf( "outptr=%I64X M2.outptr=%I64X\n", outptr, M2.outptr );

    while( 1 ) {
      r1 = M1.coro_call(&M1); 
//printf( "!r1=%i f_quit=%i l=%i!\n", r1, M1.f_quit, M1.getoutsize() );

      if( r1==1 ) {

        //inpptr=inpend; 
        chkinp(); // yield(this,1);
        l = getinpleft();
        if( l==0 ) { M1.f_quit=1; M1.addinp( sEOF, 0 ); } else
        M1.addinp( (byte*)inpptr, l ); 
        inpptr+=l; 

      } else if( (r1==2) || (r1==0) ) {

        if( r1==0 ) inpptr=M1.inpptr;

        l = M1.getoutsize(); //M1.outptr-M1.outbeg;

        Retry:
        if( l==0 ) { M2.f_quit=1; M2.addinp( sEOF, 0 ); } else
        M2.addinp( (byte*)M1.outbeg, l );

        while(1) {
//printf( "{\n" ); fflush(stdout);
          r2 = M2.coro_call(&M2); 
//printf( "}r2=%i f_quit=%i l=%i!\n", r2, M2.f_quit, M2.getoutsize() );

          if( r2==2 ) {
            outend = outptr = M2.outptr; chkout(); //yield(this,2);
            M2.addout( (byte*)outptr, getoutleft() );
            continue;
          }
          if( (r2==1) && (r1==0) ) { l=0; goto Retry; }
          break;
        }

        if( (r1==0) || (r2==0) ) break;

        M1.addout( midbuf, midbufsize );
      }
    }

    outbeg = M2.outbeg; outend = outptr = M2.outptr;

    yield(this,0);
  }

};
