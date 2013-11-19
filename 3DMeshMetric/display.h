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
