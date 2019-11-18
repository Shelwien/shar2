
template < class Model > 
struct CoroFileProcInp : Model {
  using Model::f_quit;

  enum { inpbufsize = 1<<16, outbufsize = 1<<16 };
  ALIGN(4096) byte inpbuf[inpbufsize];

  // read from file, process with coroutine, write using That coroutine
  qword processfile( filehandle f, Coroutine* _That ) {
    uint l,r;
    qword c_out = 0;
    Coroutine& Q = *_That;

    coro_init();
    goto AddOut;

    while(1) {
      r = coro_call(this); 
      if( r==1 ) {
        l = f.sread( inpbuf, inpbufsize );
        if( l==0 ) f_quit=1; // get/put don't support repeated failures
        addinp( inpbuf, l ); 
      } else { // r0=quit, r3=error
        l = getoutsize();
        Q.outptr += l; c_out += l;
        if( r!=2 ) break;
AddOut: Q.chkout(); addout( (byte*)Q.outptr, Q.getoutlen() );
      } // if
    } // while

    return c_out;
  } // func

};

