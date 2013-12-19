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

#ifndef DISPLAY_H
#define DISPLAY_H

// VTK libraries
#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkLookupTable.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>
#include <vtkPointPicker.h>

class display
{
    public:
        display();

        /* Set the camera positions
         */
        void setCameraX( int x );
        void setCameraY( int y );
        void setCameraZ( int z );

        /* Set the widget
         */
        void setMeshWidget( QVTKWidget *MeshWidget );

        /* Set the size of the screen = widget
         */
        void setSizeH( int Height );
        void setSizeW( int Width );

        /* Change the background color
         */
        void changeBackgroundColor( double Red , double Green , double Blue );

        /* Set the look up table
         */
        void setLut(vtkSmartPointer <vtkScalarsToColors> m_Lut );

        /* Init the display
         * add all the actors to the renderer
         * connect renderer with renderer window and rw with widget
         * set the captions ( orientation and color bar )
         */
        void initWindow();

        /* update the window
         * render and show
         */
        void updateWindow();

        /* add a file to display
         */
        void addData( vtkSmartPointer <vtkActor> Actor );

        /* Delete one file or all the files
         */
        void deleteData( int IndiceOfMesh );
        void deleteAll();

        /* Change the position of the camera
         */
        void updatePositionCamera();

        /* Change the LUT
         */
        void updateLut( int Visibility );

        /*virtual void OnLeftButtonDown()
        {
            std::cout << "Pressed left mouse button." << std::endl;

            vtkSmartPointer <vtkPicker> picker = vtkSmartPointer <vtkPicker>::New();

            int pickPosition[2];
            m_RenderWindow->GetInteractor()->GetEventPosition( pickPosition );

            int xP = pickPosition[0];
            int yP = pickPosition[1];
            std::cout << "x : " << xP << " , y : " << yP << std::endl;

            picker->Pick( xP , yP , 0 , m_Renderer );

            double* worldPosition;
            worldPosition = picker->GetPickPosition();

            double xW = worldPosition[0];
            double yW = worldPosition[1];
            double zW = worldPosition[2];
            std::cout << "x : " << xW << " , y : " << yW << " , z : " << zW << std::endl;

            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }*/

    private:
        int m_SizeH;
        int m_SizeW;

        double m_Red;
        double m_Green;
        double m_Blue;

        int m_CameraX;
        int m_CameraY;
        int m_CameraZ;

        int m_NumberOfMesh;

        std::vector < vtkSmartPointer <vtkActor> > m_ActorList;
        vtkSmartPointer <vtkRenderer> m_Renderer;
        vtkSmartPointer <vtkRenderWindow> m_RenderWindow;
        QVTKWidget *m_MeshWidget;

        vtkSmartPointer <vtkCamera> m_Camera;
        vtkSmartPointer <vtkAxesActor> m_Axes;
        vtkSmartPointer <vtkOrientationMarkerWidget> m_Marker;
        vtkSmartPointer <vtkScalarBarActor> m_ScalarBar;
        vtkSmartPointer <vtkScalarsToColors> m_Lut;
};

#endif
