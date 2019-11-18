
/*

0 = empty cache, waiting for input
1 = symbol cached, waiting for output request

7 = CHR0 ready for output (after EOF)
6 = CHR1 ready for output 
5 = CHR2 ready for output
4 = CHR3 ready for output
3 = EOF reached

*/

template< class wrapper_enum >
struct stream_wrapper : wrapper_enum {
using wrapper_enum::EOF;
using wrapper_enum::CHR0;
using wrapper_enum::CHR1;
using wrapper_enum::CHR2;
using wrapper_enum::CHR3;
using wrapper_enum::CHR4;
using wrapper_enum::CHR0123;
using wrapper_enum::CHR3210;

  uint state;
  uint cache;

  void init( void ) { 
    state=0; 
    cache=0;
  }

  uint need_inp( void ) {
    return state==0;
  }

  void pass_inp( uint c ) {
    if( c!=EOF ) {
      cache = (cache<<8) | c;
      state = 1;
      if( (cache|1)==CHR0123 ) {
        cache = (cache<<8) | CHR4;
        state = 2;
      }
    } else {
      cache = CHR3210;
      state = 7;
    }
  }

  uint read_out( void ) {
    uint r = (state!=3) ? byte(cache) : EOF;
    state--; if( state>0 ) cache>>=8;
    return r;
  }

};
