#include "display.h"

display::display()
{
    m_SizeH = 550 ; m_SizeW = 550;

    m_Red = 1.0 ; m_Green = 0.898 ; m_Blue = 0.8;

    m_CameraX = 0 ; m_CameraY = 1 ; m_CameraZ = 0;

    m_NumberOfMesh = 0;

    m_Renderer = vtkSmartPointer <vtkRenderer>::New();
    m_RenderWindow = vtkSmartPointer <vtkRenderWindow>::New();

    m_Camera = vtkSmartPointer <vtkCamera>::New();
    m_Axes = vtkSmartPointer <vtkAxesActor>::New();
    m_Marker = vtkSmartPointer <vtkOrientationMarkerWidget>::New();
    m_ScalarBar = vtkSmartPointer <vtkScalarBarActor>::New();
    m_Lut = vtkSmartPointer <vtkScalarsToColors>::New();    
}


//*************************************************************************************************
void display::setCameraX( int x )
{
    m_CameraX = x;
}

void display::setCameraY( int y )
{
    m_CameraY = y;
}

void display::setCameraZ( int z )
{
    m_CameraZ = z;
}


//*************************************************************************************************
void display::setMeshWidget( QVTKWidget *MeshWidget )
{
    m_MeshWidget = MeshWidget;
}


//*************************************************************************************************
void display::setSizeH( int Height )
{
    m_SizeH = Height;
}

void display::setSizeW( int Width )
{
    m_SizeW = Width;
}


//*************************************************************************************************
void display::changeBackgroundColor( double Red , double Green , double Blue )
{
    m_Red = Red;
    m_Green = Green;
    m_Blue = Blue;
    m_Renderer -> SetBackground( m_Red , m_Green , m_Blue );

}


//*************************************************************************************************
void display::setLut( vtkSmartPointer <vtkScalarsToColors> Lut )
{
    m_Lut = vtkSmartPointer <vtkScalarsToColors>::New();
    m_Lut = Lut;
    m_ScalarBar -> SetLookupTable( m_Lut );
}


//*************************************************************************************************
void display::initWindow()
{
    m_Renderer -> SetBackground( m_Red , m_Green , m_Blue );
    m_Renderer -> SetActiveCamera( m_Camera );

    for( int IndiceOfMesh = 0 ; IndiceOfMesh < m_NumberOfMesh ; IndiceOfMesh++ )
    {
        m_Renderer -> AddActor( m_ActorList[ IndiceOfMesh ] );
    }

    m_RenderWindow -> AddRenderer( m_Renderer );
    m_MeshWidget -> SetRenderWindow( m_RenderWindow );
    m_RenderWindow -> SetSize( m_SizeW , m_SizeH );

    m_Renderer -> ResetCamera();
    updatePositionCamera();

    m_Marker -> SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    m_Marker -> SetOrientationMarker( m_Axes );
    m_Marker -> SetInteractor( m_RenderWindow -> GetInteractor() );
    m_Marker -> SetViewport( 0.0, 0.0, 0.2, 0.2 );
    m_Marker -> SetEnabled( 1 );
    m_Marker -> InteractiveOff();

    m_ScalarBar -> SetTitle( "" );
    m_ScalarBar -> SetVisibility( 0 );
    m_ScalarBar -> SetLookupTable( m_Lut );
    m_ScalarBar -> SetNumberOfLabels( 5 );
    m_ScalarBar -> SetTextPositionToPrecedeScalarBar();
    m_ScalarBar -> SetHeight( 0.55 );
    m_ScalarBar -> SetWidth( 0.09 );
    m_ScalarBar -> SetPosition( 0.85 , 0.4 );
    m_ScalarBar -> GetLabelTextProperty() -> SetColor( 0 , 0 , 0 );
    m_ScalarBar -> GetLabelTextProperty() -> SetFontFamilyToTimes();

    m_Renderer -> AddActor2D( m_ScalarBar );

}


//*************************************************************************************************
void display::updateWindow()
{
    m_RenderWindow -> Render();
    m_MeshWidget -> show();
}


//*************************************************************************************************
void display::updateLut( int Visibility )
{
    m_ScalarBar -> SetVisibility( Visibility );
}


//*************************************************************************************************
void display::addData( vtkSmartPointer <vtkActor> Actor )
{
    m_ActorList.push_back( Actor );
    m_NumberOfMesh++;
}


//*************************************************************************************************
void display::deleteData( int IndiceOfMesh )
{
    m_Renderer -> RemoveActor( m_ActorList[ IndiceOfMesh ] );
    m_ActorList.erase( m_ActorList.begin() + IndiceOfMesh );
    m_NumberOfMesh--;
    updateWindow();
}


//*************************************************************************************************
void display::deleteAll()
{
    int IndiceOfMesh;
    for( IndiceOfMesh = 0 ; IndiceOfMesh < m_NumberOfMesh ; IndiceOfMesh++ )
    {
        m_Renderer -> RemoveActor( m_ActorList[ IndiceOfMesh ] );
    }

    m_ActorList.clear();
    m_NumberOfMesh = 0;
    updateWindow();
}


//*************************************************************************************************
void display::updatePositionCamera()
{
    m_Renderer -> ResetCamera();

    double *focalPoint  = m_Camera -> GetFocalPoint();
    double distance = m_Camera -> GetDistance();

    m_Camera -> SetPosition( focalPoint[0] + m_CameraX*distance , focalPoint[1] + m_CameraY*distance , focalPoint[2] + m_CameraZ*distance );

    if( m_CameraZ == 0 )
    {
        m_Camera -> SetViewUp( 0 , 0 , 1 );
    }
    else
    {
        m_Camera -> SetViewUp( 0 , -1 , 0 );
    }
}
