
struct dirparse : Coroutine {

  uint path_end; // \0 index for path_buf

  char pathbuf[dirscan::MAX_FNAME];

  CoroFileProcOut< Wrapper<1> > W;

  uint init( char* path ) {
    coro_init();
    strcpy( pathbuf, path );

    path_end = strlen(pathbuf);

    return 0;
  }

  void do_process( void ) {

    W.init();

    filehandle g;

    uint i_file = 0;
    for( i_file=0; /*i_file<n_files*/; /*i_file++*/ ) {

      while( (getinplen()==0) ) { chkinp(); if( f_quit ) goto Done; }

      pathbuf[path_end] = '/';
      sprintf( &pathbuf[1+path_end], "temp.%08X", i_file++ );

      //printf( "!%c!%i!%s!%i!\n", E.type, E.Parent, pathbuf, uint(E.usize) );

      g.make( pathbuf ); if( g ) {
        W.processfile( g, this, /*pin*/0 );
        g.close();
      } // if open

    } // for

Done:

    yield(this,0);
  }

};

