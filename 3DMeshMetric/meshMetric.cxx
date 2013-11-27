// My library
#include "meshMetricGui.h"

int main( int argc , char* argv[] )
{

      QApplication app( argc , argv );

      QWidget * parent = 0;
      Qt::WFlags f = 0;
      QString path = app.applicationDirPath();

      meshMetricGui myMainWindow( parent , f , path );

      myMainWindow.show();

      return app.exec();

}
