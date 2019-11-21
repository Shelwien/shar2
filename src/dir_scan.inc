
#ifdef _WIN32

#define AreFileApisANSI(x) 0
#define GetACP(x) CP_UTF8
#define GetOEMCP(x) CP_UTF8
#include "../zstd/dirent.h"
#undef AreFileApisANSI
#undef GetACP
#undef GetOEMCP

#else

#include <dirent.h>
#include <sys/stat.h>
#define _mkdir(X) mkdir(X,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)

#endif

struct dirscan {

  typedef DIR* pDIR;
  typedef dirent* pdirent;

  enum{ MAX_DEPTH=256, MAX_FNAME=4096 };

  char pathbuf[MAX_FNAME];

  pDIR dir0[MAX_DEPTH];
  uint endpos[MAX_DEPTH];
  uint parent[MAX_DEPTH];
  pdirent ent;

  uint state; // 0=after init, 1=after r1
  uint cur_depth;
  uint path_end; // \0 index for path_buf
  uint f_dir; // 1==dir, 0=file?
  uint i_file; // file index // number of files
  uint i_parent; // file's parent index
  char* name; // current filename;
  char* path; // original path

  uint init( char* path ) {
    uint c,i,j,k;

    //coro_init();
    state=0;

    // copy the whole path, remember last slash pos
    for( i=0,j=0; (i+1<MAX_FNAME) && ((c=path[i])!=0); i++ ) {
      if( c=='\\' ) c='/';
      pathbuf[i]=c;
      if( c=='/' ) j=i;
#ifdef _WIN32
      if( c==':' ) j=i+1;
#endif
    }
    // cut off last name/mask
    if( j==0 ) {
      strcpy( pathbuf, "." ); j++;
    } else {
      pathbuf[j]=0;
    }

    cur_depth = 0;
    path_end = j;
    i_file = 1;
    i_parent = 0;

    return 0;
  }

  uint scan( void ) {
    uint c,i,j; char* s;

    if( state==1 ) {
      i_file++; // file counter
      if( f_dir && (cur_depth+1<MAX_DEPTH) ) {
        parent[cur_depth]=i_parent;
        endpos[cur_depth]=path_end;
        path_end = strlen(pathbuf);
        //pathbuf[path_end++]='/';
        //for( i=0; (path_end+1<MAX_FNAME) && ((c=name[i])!=0); i++ ) pathbuf[path_end++]=c;
        //pathbuf[path_end]=0;
        cur_depth++; 
        i_parent = i_file-1;
        state=0; //goto Recurse;
      }
    }

    if( state==0 ) {
      dir0[cur_depth] = opendir(pathbuf);
      //printf( "!%s!dir=%I64X!\n", pathbuf, dir0[cur_depth] );
    }

    while(1) {
      ent = 0; if( dir0[cur_depth]!=0 ) ent = readdir(dir0[cur_depth]);
      if( ent==0 ) {
        if( cur_depth==0 ) break;
        closedir( dir0[cur_depth] ); dir0[cur_depth]=0;
        cur_depth--;
        path_end = endpos[cur_depth];
        i_parent = parent[cur_depth];
        pathbuf[path_end]=0;
        continue;
      }

      s = ent->d_name;
      c = (uint&)s[0];
      if( ((c&0xFFFF)=='.') || ((c&0xFFFFFF)=='..') ) continue;

      i=path_end;
      pathbuf[i++]='/';
      for( i=i,j=0; (i+1<MAX_FNAME) && ((c=s[j])!=0); j++ ) pathbuf[i++]=c;
      pathbuf[i++]=0;

#ifdef _DIRENT_HAVE_D_TYPE
      if( (ent->d_type!=DT_UNKNOWN) && (ent->d_type!=DT_LNK) ) {
         // don't have to stat if we have d_type info, unless it's a symlink (since we stat, not lstat)
        f_dir = (ent->d_type==DT_DIR);
      } else 
#endif
      {
        struct stat stbuf;
        stat(pathbuf, &stbuf); // stat follows symlinks, lstat doesn't.
        f_dir = S_ISDIR(stbuf.st_mode);
      }

      //f_dir = (ent->d_type==DT_DIR);
      name = s;

      //printf( "!%c!%s/%s!\n", f_dir?'D':'F', pathbuf, s );
      //yield(this,1);
      state=1; return 1;
    } // while

    //yield(this,0);
    return 0;
  }

};
