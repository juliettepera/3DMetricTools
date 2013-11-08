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

        void initWindow();
        void updateWindow();

        void addData( vtkSmartPointer <vtkActor> Actor );
        void deleteData( int IndiceOfMesh );
        void deleteAll();

        void updatePositionCamera();

        void setCameraX( int x );
        void setCameraY( int y );
        void setCameraZ( int z );

        void setSizeH( int Height );
        void setSizeW( int Width );

        void setMeshWidget( QVTKWidget *MeshWidget );

        void setLut(vtkSmartPointer <vtkScalarsToColors> m_Lut );
        void updateLut( int Visibility );

        void changeBackgroundColor( double Red , double Green , double Blue );

    private:
        QVTKWidget *m_MeshWidget;

        vtkSmartPointer <vtkRenderer> m_Renderer;
        vtkSmartPointer <vtkRenderWindow> m_RenderWindow;
        vtkSmartPointer <vtkCamera> m_Camera;
        vtkSmartPointer <vtkAxesActor> m_Axes;
        vtkSmartPointer <vtkOrientationMarkerWidget> m_Marker;
        vtkSmartPointer <vtkScalarBarActor> m_ScalarBar;
        vtkSmartPointer <vtkScalarsToColors> m_Lut;

        int m_SizeH;
        int m_SizeW;

        int m_CameraX;
        int m_CameraY;
        int m_CameraZ;

        int m_NumberOfMesh;

        std::vector < vtkSmartPointer <vtkActor> > m_ActorList;

        double m_Red;
        double m_Green;
        double m_Blue;

};

#endif
