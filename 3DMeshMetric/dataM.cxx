#include "dataM.h"

dataM::dataM( std::string Name )
{
    // init the basic data
    m_Name = Name;
    m_Mapper = vtkSmartPointer <vtkPolyDataMapper>::New();
    m_Actor = vtkSmartPointer <vtkActor>::New();
    m_PolyData = vtkSmartPointer <vtkPolyData>::New();

    // init the file properties
    m_Opacity = 1.0;
    m_Red = rand()/(double)RAND_MAX;
    m_Green = rand()/(double)RAND_MAX;
    m_Blue = rand()/(double)RAND_MAX;
    m_Type = 1;

    // init the error parameters
    m_MinSamplingFrequency = 2;
    m_SamplingStep = 0.5;
    m_SignedDistance = false;
    m_DisplayError = false;
    m_ColorBar = false;
    //m_Lut = vtkSmartPointer <vtkColorTransferFunction>::New();
    m_Dmin = -1.0;
    m_Dmax = 1.0;
    m_NameB = "";
}

/* initialize the reading of the file, create the polydata, update the properties of the
*  file, the type of display, connect the mapper to the actor
*/
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

/* update the properties of the file ( opacity and color )
 */
void dataM::updateActorProperties()
{
    m_Actor -> GetProperty() -> SetOpacity( m_Opacity );
    m_Actor -> GetProperty() -> SetColor( m_Red , m_Green , m_Blue );
}

/* update the type of display of the file
 * type = 1 : surface | 2 : points | 3 : wireframe
 * return 0 in case of success and 1 in case of error
 **/
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

/* change the input of the mapper in function of the choice
 * choice = 1 : data ( with or without error ) | 2 : data smoothed | 3 : data downsampled
 * return 0 in case of success and 1 in case of error
 */
int dataM::changeMapperInput()
{
    m_Mapper = vtkSmartPointer <vtkPolyDataMapper>::New();
    m_Mapper -> SetInputData( m_PolyData );
    changeActivScalar();
    return 0;
}

void dataM::changeActivScalar()
{
    m_Mapper -> SetLookupTable( m_Lut );
    m_Mapper -> SetScalarModeToUsePointFieldData();

    if( m_DisplayError == true )
    {
        m_Mapper -> SelectColorArray( "Error" );
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

// Set the basic data
void dataM::setName( std::string Name )
{
    m_Name = Name;
}

void dataM::setPolyData( vtkSmartPointer<vtkPolyData> OldData )
{
    m_PolyData = OldData;
}

// Get the basic data
std::string dataM::getName()
{
    return m_Name;
}

vtkSmartPointer <vtkPolyDataMapper> dataM::getMapper()
{
    return m_Mapper;
}

vtkSmartPointer <vtkActor> dataM::getActor()
{
    return m_Actor;
}

vtkSmartPointer <vtkPolyData> dataM::getPolyData()
{
    return m_PolyData;
}

// Set the File properties
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

// Get the file properties
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

// Set the error parameters
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

void dataM::setColorBar( bool ColorBar )
{
    m_ColorBar = ColorBar;
}

void dataM::setDisplayError( bool DisplayError )
{
    m_DisplayError = DisplayError;
}

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

void dataM::setNameB(std::string NameB)
{
    m_NameB = NameB;
}

// Get the error parameters
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

bool dataM::getColorBar()
{
    return m_ColorBar;
}

bool dataM::getDisplayError()
{
    return m_DisplayError;
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

std::string dataM::getNameB()
{
    return m_NameB;
}



















