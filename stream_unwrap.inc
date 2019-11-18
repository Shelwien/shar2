
/*

0 = empty cache, waiting for input
1 = symbol cached, waiting for output request

2 = CHR0 detected
3 = CHR0,CHR1 detected
4 = CHR0,CHR1,CHR2 detected

13 = CHR2..CHR0 to output
12 = CHR1..CHR0 to output
11 = CHR0 to output
10 = all clear, return to 0

*/

template< class wrapper_enum >
struct stream_unwrapper : wrapper_enum {
using wrapper_enum::EOF;
using wrapper_enum::CHR0;
using wrapper_enum::CHR1;
using wrapper_enum::CHR2;
using wrapper_enum::CHR3;
using wrapper_enum::CHR4;
using wrapper_enum::CHR0123;
using wrapper_enum::CHR3210;

  uint state;
  uint count;
  uint cache;
  uint cache2;

  void init( void ) { 
    state=0; count=0;
    cache=0; cache2=0;
  }

  uint need_inp( void ) {
    return (state!=1) && (state<10);
  }

  void pass_inp( uint c ) {

    cache = c; 

    switch( state ) {
    case 0: if( c==CHR0 ) state=2-1; break;
    case 2: if( c==CHR1 ) state=3-1; else count=2,state=9,cache2=c; break;
    case 3: if( c==CHR2 ) state=4-1; else count=3,state=9,cache2=c; break;
    case 4: 
      if( c==CHR3 ) cache=EOF, state=1-1;  else
      if( c==CHR4 ) cache2=CHR2, count=3,state=9;  else  
                    cache2=c,    count=4,state=9;
      break;
    }

    state++;

  }

  uint read_out( void ) {
    uint r = cache; state--;

    if( state>=9 ) {
      r = byte( CHR3210 >> ((state-9)*8) );
      state+=2; count--;
      if( count==0 ) r=cache2, state=0; else
      if( (count==1) && (cache2==CHR0) ) state=2;
    }

    return r;
  }

};
