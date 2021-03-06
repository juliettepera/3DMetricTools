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

#include "dataM.h"

dataM::dataM( std::string Name )
{
    m_Name = Name;
    m_Type = 0;
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
    m_TypeDistance = 0;
    m_DisplayError = false;
    m_ColorBar = false;

    m_Dmin = -1.0;
    m_Dmax = 1.0;
    m_Delta = 0.5;
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
void dataM::setTypeFile( int Type )
{
    m_TypeFile = Type;
}

int dataM::getTypeFile()
{
    return m_TypeFile;
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

void dataM::setTypeDistance( int TypeDistance )
{
    m_TypeDistance = TypeDistance;
}

void dataM::setDisplayError( bool DisplayError )
{
    m_DisplayError = DisplayError;
}

void dataM::setColorBar( bool ColorBar )
{
    m_ColorBar = ColorBar;
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

int dataM::getTypeDistance()
{
    return m_TypeDistance;
}

bool dataM::getDisplayError()
{
    return m_DisplayError;
}

bool dataM::getColorBar()
{
    return m_ColorBar;
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

//*************************************************************************************************
int dataM::initialization()
{
    vtkSmartPointer <vtkTriangleFilter> Triangler = vtkSmartPointer <vtkTriangleFilter>::New();
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New();

    if( m_TypeFile == 0 )
    {
       return -1;
    }
    else if( m_TypeFile == 1 )
    {
       vtkSmartPointer <vtkPolyDataReader> ReaderVTK = vtkSmartPointer <vtkPolyDataReader>::New();
       ReaderVTK -> SetFileName( m_Name.c_str() );

       if( ! ReaderVTK -> IsFileStructuredPoints() )
       {
            ReaderVTK -> Update();
            Cleaner -> SetInputData( ReaderVTK -> GetOutput() );
       }
       else
       {
           QMessageBox MsgBox;
           MsgBox.setText( " Structured_points cannot be read ");
           MsgBox.exec();
           return -1;
       }
    }
    else if( m_TypeFile == 2 )
    {
       vtkSmartPointer <vtkOBJReader> ReaderOBJ = vtkSmartPointer <vtkOBJReader>::New();
       ReaderOBJ -> SetFileName( m_Name.c_str() );
       ReaderOBJ -> Update();
       ReaderOBJ -> GetOutput() -> GetPointData() -> SetNormals(NULL);
       Cleaner -> SetInputData( ReaderOBJ -> GetOutput() );
    }
    else if( m_TypeFile == 3 )
    {
       vtkSmartPointer <vtkSTLReader> ReaderSTL = vtkSmartPointer <vtkSTLReader>::New();
       ReaderSTL -> SetFileName( m_Name.c_str() );
       ReaderSTL -> Update();
       ReaderSTL -> GetOutput() -> GetPointData() -> SetNormals(NULL);
       Cleaner -> SetInputData( ReaderSTL -> GetOutput() );
    }
    else if( m_TypeFile == 4 )
    {
       vtkSmartPointer <vtkXMLPolyDataReader> ReaderVTP = vtkSmartPointer <vtkXMLPolyDataReader>::New();
       ReaderVTP -> SetFileName( m_Name.c_str() );
       ReaderVTP -> Update();
       Cleaner -> SetInputData( ReaderVTP -> GetOutput() );
    }

    Cleaner -> Update();

    Triangler -> SetInputData( Cleaner -> GetOutput() );
    Triangler -> Update();

    m_PolyData = Triangler -> GetOutput();

    m_Mapper -> SetInputData( m_PolyData );

    updateActorProperties();
    updateTypeOfDisplay();

    m_Actor -> SetMapper( m_Mapper );

    return 0;
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

    if( m_DisplayError == true && m_TypeDistance == 0 )
    {
        m_Mapper -> SelectColorArray( "Absolute" );
        m_Mapper -> SetScalarVisibility( 1 );
    }
    else if( m_DisplayError == true && m_TypeDistance == 1 )
    {
        m_Mapper -> SelectColorArray( "Signed" );
        m_Mapper -> SetScalarVisibility( 1 );
    }
    else if( m_DisplayError == true && m_TypeDistance == 2 )
    {
        m_Mapper -> SelectColorArray( "Correspondant" );
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


//*************************************************************************************************
vtkIdType dataM::getIdPointClicked( double ClickedPosition[3] )
{
    std::vector <vtkIdType> IdPicked;
    double Delta = 1;

    for( vtkIdType i = 0 ; i < m_PolyData->GetNumberOfPoints() ; i++ )
    {
      double Point[3];
      m_PolyData -> GetPoints() -> GetPoint( i , Point );

      double delta0 = fabs( Point[0]-ClickedPosition[0] );
      double delta1 = fabs( Point[1]-ClickedPosition[1] );
      double delta2 = fabs( Point[2]-ClickedPosition[2] );

      if( delta0 < Delta && delta1 < Delta && delta2 < Delta )
      {
          IdPicked.push_back( i );
      }
    }

    if( IdPicked.empty() == false )
    {
        double delta = 1;
        while( IdPicked.size() != 1 )
        {
            for( unsigned int i = 0 ; i < IdPicked.size() ; i++ )
            {
                double Point[3];
                m_PolyData -> GetPoints() -> GetPoint( IdPicked.at( i ) , Point );

                double delta0 = fabs( Point[0]-ClickedPosition[0] );
                double delta1 = fabs( Point[1]-ClickedPosition[1] );
                double delta2 = fabs( Point[2]-ClickedPosition[2] );

                if( !( delta0 < delta && delta1 < delta && delta2 < delta ) )
                {
                    IdPicked.erase( IdPicked.begin() + i );
                }
            }
            delta = delta - 0.05;
        }
        return IdPicked.at(0);
    }
    else
    {
        return -1;
    }
}


//*************************************************************************************************
QString dataM::getScalarValue( vtkIdType MyId )
{
    QString Value;

    if( MyId != -1 )
    {
        int NbArray = m_PolyData -> GetPointData() -> GetNumberOfArrays();
        if( NbArray == 0 )
        {
            Value = "No Distance Value";
        }
        else
        {
            if( strcmp( m_PolyData -> GetPointData() -> GetArray(0) -> GetName() , "Absolute" ) == 0
             || strcmp( m_PolyData -> GetPointData() -> GetArray(0) -> GetName() , "Signed" ) == 0
             || strcmp( m_PolyData -> GetPointData() -> GetArray(0) -> GetName() , "Correspondant" ) == 0 )
            {
                double value = m_PolyData -> GetPointData() -> GetArray(0) -> GetComponent( MyId , 0 );
                Value = QString::number( value );
            }
            else
            {
                Value = "No Distance Value";
            }
        }
    }
    else
    {
       Value = "Not a Point";
    }
    return Value;

}


