
// from paq8px_v183.cpp

//#include <windows.h>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

int main( int argc, char** argv ) {

#ifdef _WIN32
  // On Windows, argv is encoded in the effective codepage, therefore unsuitable 
  // for acquiring command line arguments (file names
  // in our case) not representable in that particular codepage.
  // -> We will recreate argv as UTF8 (like in Linux)
  uint i, /*oldcp = GetConsoleOutputCP(),*/ buffersize, retval;
//  SetConsoleOutputCP(CP_UTF8);
  int argc_utf8 = 0;
  char** argv_utf8 = 0 ;
  #pragma comment(lib,"shell32.lib")
  wchar_t** szArglist = CommandLineToArgvW( GetCommandLineW(), &argc_utf8 );

  if( szArglist==0 ) {
    fprintf(stderr, "CommandLineToArgvW failed\n" ); return 1;
  } else {
    if( argc!=argc_utf8 ) { fprintf(stderr, "Error preparing command line arguments." ); return 1; }
    argv_utf8 = new char*[argc_utf8+1];
    for( i=0; i<argc_utf8; i++ ) { 
      wchar_t* s = szArglist[i];
      buffersize = WideCharToMultiByte(CP_UTF8,0,s,-1,NULL,0,NULL,NULL);
      argv_utf8[i] = new char[buffersize];
      WideCharToMultiByte(CP_UTF8,0,s,-1,argv_utf8[i],buffersize,NULL,NULL);
      //printf("%d: %s\n", i, argv_utf8[i]); //debug: see if conversion is successful
    }
    argv_utf8[argc_utf8]=0;

    setlocale(LC_ALL, ".UTF8");
    setmode( fileno(stdin), O_BINARY );
    setmode( fileno(stdout), O_BINARY );

    retval = main_utf8(argc_utf8, argv_utf8);
    //for( i=0; i<argc_utf8; i++ ) delete[] argv_utf8[i]; 
    //delete[] argv_utf8;
    //SetConsoleOutputCP(oldcp);
    return retval;
  }

#else

  return main_utf8(argc, argv);

#endif

}
