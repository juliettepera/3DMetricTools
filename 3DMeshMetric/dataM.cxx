#include "dataM.h"

dataM::dataM( std::string Name )
{
    m_Name = Name;
    m_PolyData = vtkSmartPointer <vtkPolyData>::New();
    m_Mapper = vtkSmartPointer <vtkPolyDataMapper>::New();
    m_Actor = vtkSmartPointer <vtkActor>::New();

    m_Opacity = 1.0;
    m_Red = rand()/(double)RAND_MAX;
    m_Green = rand()/(double)RAND_MAX;
    m_Blue = rand()/(double)RAND_MAX;
    m_Type = 1;

    m_NameB = "";
    m_MinSamplingFrequency = 2;
    m_SamplingStep = 0.5;
    m_SignedDistance = false;
    m_DisplayError = false;

    m_Dmin = -1.0;
    m_Dmax = 1.0;
    m_ColorBar = false;
}


//*************************************************************************************************
void dataM::setName( std::string Name )
{
    m_Name = Name;
}

std::string dataM::getName()
{
    return m_Name;
}

//*************************************************************************************************
void dataM::setPolyData( vtkSmartPointer<vtkPolyData> OldData )
{
    m_PolyData = OldData;
}

vtkSmartPointer <vtkPolyData> dataM::getPolyData()
{
    return m_PolyData;
}

//*************************************************************************************************
vtkSmartPointer <vtkPolyDataMapper> dataM::getMapper()
{
    return m_Mapper;
}

vtkSmartPointer <vtkActor> dataM::getActor()
{
    return m_Actor;
}

//*************************************************************************************************
void dataM::setOpacity( double Opacity )
{
    m_Opacity = Opacity;
}

void dataM::setColor( double Red , double Green , double Blue )
{
    m_Red = Red;
    m_Green = Green;
    m_Blue = Blue;
}

void dataM::setType( int Type )
{
    m_Type = Type;
}

double dataM::getOpacity()
{
    return m_Opacity;
}

double dataM::getRed()
{
    return m_Red;
}

double dataM::getGreen()
{
    return m_Green;
}

double dataM::getBlue()
{
    return m_Blue;
}

int dataM::getType()
{
    return m_Type;
}

//*************************************************************************************************
void dataM::setNameB(std::string NameB)
{
    m_NameB = NameB;
}

void dataM::setMinSamplingFrequency( int MinSamplingFrequency )
{
    m_MinSamplingFrequency = MinSamplingFrequency;
}

void dataM::setSamplingStep( double SamplingStep )
{
    m_SamplingStep = SamplingStep;
}

void dataM::setSignedDistance( bool SignedDistance )
{
    m_SignedDistance = SignedDistance;
}

void dataM::setDisplayError( bool DisplayError )
{
    m_DisplayError = DisplayError;
}

std::string dataM::getNameB()
{
    return m_NameB;
}

int dataM::getMinSamplingFrequency()
{
    return m_MinSamplingFrequency;
}

double dataM::getSamplingStep()
{
    return m_SamplingStep;
}

bool dataM::getSignedDistance()
{
    return m_SignedDistance;
}

bool dataM::getDisplayError()
{
    return m_DisplayError;
}


//*************************************************************************************************
void dataM::setLut( vtkSmartPointer <vtkColorTransferFunction> Lut )
{
    m_Lut = vtkSmartPointer <vtkColorTransferFunction>::New();
    m_Lut = Lut;
}

void dataM::setMin( double Dmin )
{
    m_Dmin = Dmin;
}

void dataM::setMax( double Dmax )
{
    m_Dmax = Dmax;
}

void dataM::setCenter( double Center )
{
    m_Center = Center;
}

void dataM::setDelta( double Delta )
{
    m_Delta = Delta;
}

void dataM::setColorBar( bool ColorBar )
{
    m_ColorBar = ColorBar;
}

vtkSmartPointer <vtkColorTransferFunction> dataM::getLut()
{
    return m_Lut;
}

double dataM::getMin()
{
    return m_Dmin;
}

double dataM::getMax()
{
    return m_Dmax;
}

double dataM::getCenter()
{
    return m_Center;
}

double dataM::getDelta()
{
    return m_Delta;
}

bool dataM::getColorBar()
{
    return m_ColorBar;
}

//*************************************************************************************************
void dataM::initialization()
{
    vtkSmartPointer <vtkPolyDataReader> Reader = vtkSmartPointer <vtkPolyDataReader>::New();
    vtkSmartPointer <vtkTriangleFilter> Triangler = vtkSmartPointer <vtkTriangleFilter>::New();
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New();

    Reader -> SetFileName( m_Name.c_str() );
    Reader -> Update();

    Cleaner -> SetInputData( Reader -> GetOutput() );
    Cleaner -> Update();

    Triangler -> SetInputData( Cleaner -> GetOutput() );
    Triangler -> Update();

    m_PolyData = Triangler -> GetOutput();

    m_Mapper -> SetInputData( m_PolyData );

    updateActorProperties();
    updateTypeOfDisplay();

    m_Actor -> SetMapper( m_Mapper );
}

//*************************************************************************************************
void dataM::updateActorProperties()
{
    m_Actor -> GetProperty() -> SetOpacity( m_Opacity );
    m_Actor -> GetProperty() -> BackfaceCullingOn();
    m_Actor -> GetProperty() -> SetColor( m_Red , m_Green , m_Blue );
}


//*************************************************************************************************
int dataM::updateTypeOfDisplay()
{
    if( m_Type == 1 )
    {
        m_Actor -> GetProperty() -> SetRepresentationToSurface();
        return 0;
    }
    else if( m_Type == 2 )
    {
       m_Actor -> GetProperty() -> SetRepresentationToPoints();
       return 0;
    }
    else if( m_Type == 3 )
    {
       m_Actor -> GetProperty() -> SetRepresentationToWireframe();
       return 0;
    }
    else
    {
        QMessageBox MsgBox;
        MsgBox.setText( " ERROR : TYPE OF DISPLAY ");
        MsgBox.exec();
        return 1;
    }
}

//*************************************************************************************************
int dataM::changeMapperInput()
{
    m_Mapper = vtkSmartPointer <vtkPolyDataMapper>::New();
    m_Mapper -> SetInputData( m_PolyData );
    changeActivScalar();
    return 0;
}

//*************************************************************************************************
void dataM::changeActivScalar()
{
    m_Mapper -> SetLookupTable( m_Lut );
    m_Mapper -> SetScalarModeToUsePointFieldData();

    if( m_DisplayError == true && m_SignedDistance == true )
    {
        m_Mapper -> SelectColorArray( "Signed" );
        m_Mapper -> SetScalarVisibility( 1 );
    }
    else if( m_DisplayError == true && m_SignedDistance == false )
    {
        m_Mapper -> SelectColorArray( "Absolute" );
        m_Mapper -> SetScalarVisibility( 1 );
    }
    else if( m_DisplayError == false )
    {
        m_Mapper -> SelectColorArray( "Original" );
        m_Mapper -> SetScalarVisibility( 0 );
    }

    m_Mapper -> Update();
    m_Actor -> SetMapper( m_Mapper );
}
