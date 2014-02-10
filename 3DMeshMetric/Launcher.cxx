#include <stdlib.h>
#include "meshMetricCLP.h"
#include <string>
#include <itksys/SystemTools.hxx>

int main( int argc , char* argv[] )
{ 
  PARSE_ARGS;

  std::string path = argv[0];
  
  if( path.find('/' ) == std::string::npos )
  {
    //found software in PATH
    path = itksys::SystemTools::FindProgram( argv[0] ) ;
  }
  //either relative or absolute path
  if( path[0] == '/' )//absolute path
  {
    unsigned int found = path.find_last_of( "/" ) ;
    path = path.substr( 0 , found ) ;
  }
  else//relative path
  {
    char currentPath[1024] ;
    getcwd( currentPath , sizeof(currentPath) ) ;
    std::cout << "Current path: " << currentPath << std::endl ;
    if( !strcmp( currentPath , "" ) )
    {
      std::cerr << "Error: Unknown current working directory" << std::endl ;
      return -1 ;
    }
    std::string currentPathStr = currentPath ;
    unsigned int found = path.find_last_of( "/" ) ;
    path = path.substr( 0 , found ) ; 
    currentPathStr += '/' ;
    path = currentPathStr + path ;
  }
  std::cout<< "PATH: " << path << std::endl ;
  
  char* env_value ;
  env_value = getenv( "DYLD_LIBRARY_PATH" ); 
  
  std::string Value_DYLD_LIBRARY_PATH ;
  
  if( env_value )
  {
     Value_DYLD_LIBRARY_PATH = env_value ;
     Value_DYLD_LIBRARY_PATH += ":" ;
  }
  Value_DYLD_LIBRARY_PATH += path + "/lib" ;
  setenv( "DYLD_LIBRARY_PATH" , Value_DYLD_LIBRARY_PATH.c_str() , 1 ) ;
   
  std::string cmd ;
  cmd += path + "/Contents/MacOS/meshMetric" ;
  
  for( int i = 1 ; i < argc ; i++ )
  {
	  cmd += " " ;
	  cmd += argv[ i ] ;
  }

  int val = system( cmd.c_str() ) ;
  return val ;
}
