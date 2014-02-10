/* 3DMeshMetric is a visualization tool used to measures and display
 * surface-to-surface distance between two triangle meshes.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors : Juliette Pera, Francois Budin, Beatriz Paniagua
 * Web site : http://www.nitrc.org/projects/meshmetric3d/
 *
 * Reference : MeshValmet, Validation Metric for Meshes
 * http://www.nitrc.org/projects/meshvalmet/
 *
 * This program also uses Qt libraries
 * http://www.qt-project.org/
 */

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

      myMainWindow.setWindowState( Qt::WindowMaximized );

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
