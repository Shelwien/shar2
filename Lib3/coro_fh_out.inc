

template < class Model > 
struct CoroFileProcOut : Model {
  using Model::f_quit;
  using Model::inpptr;
  using Model::inpend;

  enum { inpbufsize = 1<<16, outbufsize = 1<<16 };
  ALIGN(4096) byte outbuf[outbufsize];

  void init( void ) {
    coro_init();
  }

  // read from given pin of That coroutine, process, write to file
  qword processfile( filehandle g, Coroutine* _That, uint i=0 ) {
    uint l,r;
    qword c_out = 0;
    Coroutine& Q = *_That;

    addout( outbuf, outbufsize );

    while(1) {
      r = coro_call(this); 
      if( r==1 ) {
        Q.pin[i].chkinp(); l = Q.pin[i].getinplen();
        if( l==0 ) f_quit=1;
        addinp( (byte*)Q.pin[i].ptr, l ); Q.pin[i].ptr+=l;
      } else { // r0=quit, r3=error
        l = getoutsize();
        if( l>0 ) g.writ( outbuf, l ); 
        c_out += l;
        if( r!=2 ) break;
        addout( outbuf, outbufsize );
      } // if
    } // while

    Q.pin[i].ptr -= inpend-inpptr;
    addinp( 0, 0 ); // get it to redo chkinp

    return c_out;
  } // func

};

