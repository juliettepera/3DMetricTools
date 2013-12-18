// My library
#include "meshMetricCLP.h"
#include "meshMetricGui.h"

int main( int argc , char* argv[] )
{
      PARSE_ARGS ;
      QApplication app( argc , argv );

      QWidget * parent = 0;
      Qt::WFlags f = 0;

      QString path = app.applicationDirPath();

      #ifdef __APPLE__
        path.remove( path.size()-5 , 5 );
        path += "Resources";
      #endif

      meshMetricGui myMainWindow( parent , f , path );

      myMainWindow.show();
      int ret = app.exec() ;
      if( ret )
      {
          return EXIT_FAILURE ;
      }
      else
      {
          return EXIT_SUCCESS ;
      }
}
