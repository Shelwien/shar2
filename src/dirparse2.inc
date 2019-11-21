
struct dirparse2 : Coroutine {

  entry E;

  uint cur_depth;
  uint path_end; // \0 index for path_buf
  uint path2_end; // \0 index for path_buf2
  uint endpos[dirscan::MAX_DEPTH];
  uint parent[dirscan::MAX_DEPTH];

  char pathbuf[dirscan::MAX_FNAME];
  char path2buf[dirscan::MAX_FNAME];

  uint init( char* path ) {
    coro_init();
    strcpy( pathbuf, path );
    strcpy( path2buf, path );

    path2_end = path_end = strlen(pathbuf);
    cur_depth = 0;
    parent[0] = 0;
    endpos[0] = path_end;

    return 0;
  }

  void do_process( void ) {

    uint r, i_file = 0;
    path2buf[path2_end] = '/';

    for( i_file=0; /*i_file<n_files*/; /*i_file++*/ ) {

     if( E.parse(*this) ) break;

     while( (cur_depth>0) && (E.Parent<parent[cur_depth-1]) ) {
       cur_depth--;
       path_end = endpos[cur_depth];
       pathbuf[path_end] = 0;
     }

     pathbuf[path_end] = '/';
     strcpy( &pathbuf[1+path_end], E.name );

     if( E.type=='D' ) {
       fprintf(stderr, "!%c!%i!%s!\n", E.type, E.Parent, pathbuf );
       _mkdir( pathbuf );
     } else {
       sprintf( &path2buf[1+path2_end], "temp.%08X", i_file++ );

       r = rename( path2buf, pathbuf );
       if( r!=0 ) {
         remove( pathbuf );
         r = rename( path2buf, pathbuf );
       }

//fprintf(stderr, "!err=%i!\n", r );
       fprintf(stderr, "!%c!%i!%s!%s!\n", E.type, E.Parent, pathbuf, path2buf );
     }

     if( E.type=='D' ) {
       endpos[cur_depth] = path_end;
       parent[cur_depth] = 1+i_file;
       path_end = strlen(pathbuf);
       cur_depth++;
     } else {
       pathbuf[path_end] = 0;
     }

    } // for

    yield(this,0);
  }

};

