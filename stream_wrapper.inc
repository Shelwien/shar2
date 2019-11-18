
#undef EOF

template< int _CHR0=0x99, int _CHR1=0x19, int _CHR2=0xD3, int _CHR3=0xFF, int _CHR4=0xFE >
struct wrapper_enum {
  enum { EOF=0x100 };

  //99 19 D3
  enum { 
    CHR0=_CHR0, CHR1=_CHR1, CHR2=_CHR2, CHR3=_CHR3, CHR4=_CHR4,
    CHR0123=CHR0*0x1000000U+CHR1*0x10000+CHR2*0x100+CHR3, 
    CHR3210=CHR3*0x1000000U+CHR2*0x10000+CHR1*0x100+CHR0
  };
};

#include "stream_wrap.inc"
#include "stream_unwrap.inc"

template< int MODE, int _CHR0=0x99 > struct wrapper_base;
template< int _CHR0 > struct wrapper_base<0,_CHR0> : stream_wrapper< wrapper_enum<_CHR0> > {};
template< int _CHR0 > struct wrapper_base<1,_CHR0> : stream_unwrapper< wrapper_enum<_CHR0> > {};

template <int ProcMode, int _CHR0=0x99>
struct Wrapper : Coroutine {

  enum{ DECODE=ProcMode };

  wrapper_base<DECODE,_CHR0> W;

  qword c_out; // number of input bytes

  void do_process( void ) {

    uint c;
   
    c_out = 0; 

    W.init();

    while(1) {

      while( W.need_inp() ) {
        c = get(); c_out+=1-f_quit;
        W.pass_inp( f_quit ? W.EOF : c );
      }

      c = W.read_out(); if( c==W.EOF ) break;

      put( c );

    }

//printf( "c_out=%i f_quit=%i\n", uint(c_out), f_quit );

    yield(this,0);
  }

};

