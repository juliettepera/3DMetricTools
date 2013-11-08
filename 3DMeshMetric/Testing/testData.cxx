#include "../dataM.h"
#include "vtkCellArray.h"
#include "vtkProperty.h"

int testPolyData(vtkSmartPointer<vtkPolyData> inData , vtkSmartPointer<vtkPolyData> outData );

int main( int argc , char* argv[] )
{
    std::cout << " BEGIN TEST  ----> DATAM.H " << std::endl;

    dataM dataTest;

    //**************************************************************************************
    std::cout << " Test : Name " << std::endl;
    std::string inName = "/work/jpera/Data/Data_Slicer3/Registered_condyle_V2.vtk" ;
    std::string outName = "out";
    dataTest.setName( inName );
    outName = dataTest.getName();
    if( outName != inName )
    {
        std::cout << " ERROR : name " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : Opacity " << std::endl;
    double inOpacity = 0.95;
    double outOpacity = 0.0;
    dataTest.setOpacity( inOpacity );
    outOpacity = dataTest.getOpacity();
    if( outOpacity != inOpacity )
    {
        std::cout << " ERROR : opacity " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : Color " << std::endl;
    double inRed = 0.5;
    double outRed = 0.0;
    double inBlue = 0.3;
    double outBlue = 0.0;
    double inGreen = 0.654;
    double outGreen = 0.0;
    dataTest.setColor( inRed , inGreen , inBlue );
    outRed = dataTest.getRed();
    outGreen = dataTest.getGreen();
    outBlue = dataTest.getBlue();
    if( outRed != inRed || outBlue != inBlue || outGreen != inGreen )
    {
        std::cout << " ERROR : color " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : Type " << std::endl;
    int inType = 2;
    int outType = 1;
    dataTest.setType( inType );
    outType = dataTest.getType();
    if( outType != inType )
    {
        std::cout << " ERROR : type " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : getActor | updateproperties " << std::endl;
    vtkSmartPointer <vtkActor> outActor = vtkSmartPointer <vtkActor>::New();

    dataTest.initialization();
    outActor = dataTest.getActor();

    outOpacity = outActor -> GetProperty() -> GetOpacity();
    outActor -> GetProperty() -> GetColor( outRed , outGreen , outBlue );
    if( outOpacity != inOpacity || outRed != inRed || outBlue != inBlue || outGreen != inGreen )
    {
        std::cout << " ERROR : actor properties " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : update type display " << std::endl;
    outType = outActor -> GetProperty() -> GetRepresentation();
    if( outType != 0 )
    {
        std::cout << " ERROR : type display " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    /*std::cout << " Test : NumberIteration " << std::endl;
    int inNumberIteration = 500;
    int outNumberIteration = 0;
    dataTest.setNumberOfIterationSmooth( inNumberIteration );
    outNumberIteration = dataTest.getNumberOfIterationSmooth();
    if( outNumberIteration != inNumberIteration )
    {
        std::cout << " ERROR : number iteration " << std::endl;
        return EXIT_FAILURE;
    }*/

    //**************************************************************************************
   /* std::cout << " Test : Decimate " << std::endl;
    double inDecimate = 0.14;
    double outDecimate = 0.0;
    dataTest.setDecimate( inDecimate );
    outDecimate = dataTest.getDecimate();
    if( outDecimate != inDecimate )
    {
        std::cout << " ERROR : decimate " << std::endl;
        return EXIT_FAILURE;
    }*/

    //**************************************************************************************
    std::cout << " Test : MisSampFreq " << std::endl;
    double inMinSampFreq = 2.0;
    double outMinSampFreq = 0.0;
    dataTest.setMinSamplingFrequency( inMinSampFreq );
    outMinSampFreq = dataTest.getMinSamplingFrequency();
    if( outMinSampFreq != inMinSampFreq )
    {
        std::cout << " ERROR : min samp freq " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : SampStep " << std::endl;
    double inSampStep = 0.02;
    double outSampStep = 0.0;
    dataTest.setSamplingStep( inSampStep );
    outSampStep = dataTest.getSamplingStep();
    if( outSampStep != inSampStep )
    {
        std::cout << " ERROR : samp step " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : SignedDist " << std::endl;
    bool inSignedDist = true;
    bool outSignedDist = false;
    dataTest.setSignedDistance( inSignedDist );
    outSignedDist = dataTest.getSignedDistance();
    if( outSignedDist != inSignedDist )
    {
        std::cout << " ERROR : signed dist " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : DisplayError " << std::endl;
    bool inDisplayError = true;
    bool outDisplayError = false;
    dataTest.setDisplayError( inDisplayError );
    outDisplayError = dataTest.getDisplayError();
    if( outDisplayError != inDisplayError )
    {
        std::cout << " ERROR : display error " << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " Test : GetPolyData " << std::endl;

    vtkSmartPointer <vtkPolyData> refData = vtkSmartPointer <vtkPolyData>::New();
    vtkSmartPointer <vtkPolyDataReader> Reader = vtkSmartPointer <vtkPolyDataReader>::New();
    vtkSmartPointer <vtkTriangleFilter> Triangler = vtkSmartPointer <vtkTriangleFilter>::New();

    Reader -> SetFileName( "/work/jpera/Data/Data_Slicer3/Registered_condyle_V2.vtk" );
    Reader -> Update();
    Triangler -> SetInputData( Reader -> GetOutput() );
    Triangler -> Update();
    refData = Triangler -> GetOutput();

    vtkSmartPointer <vtkPolyData> outData = vtkSmartPointer <vtkPolyData>::New();

    outData = dataTest.getPolyData();

    if( testPolyData( refData , outData ) == 1 )
    {
        std::cout << " ERROR : getPolyData" << std::endl;
        return EXIT_FAILURE;
    }


    //**************************************************************************************
    std::cout << " Test : getMapper " << std::endl;
    vtkSmartPointer <vtkPolyDataMapper> outMapper = vtkSmartPointer <vtkPolyDataMapper>::New();

    outMapper = dataTest.getMapper();

    if( testPolyData( outMapper -> GetInput() , outData ) == 1 )
    {
        std::cout << " ERROR : getMapper " << std::endl;
        return EXIT_FAILURE;
    }


    //**************************************************************************************
    std::cout << " Test : SetPolyData " << std::endl;

    Reader -> SetFileName( "/work/jpera/Data/Data_Slicer3/Registered_condyle_V8.vtk" );
    Reader -> Update();
    Triangler -> SetInputData( Reader -> GetOutput() );
    Triangler -> Update();
    refData = Triangler -> GetOutput();

    dataTest.setPolyData( refData );

    outData = dataTest.getPolyData();

    if( testPolyData( refData , outData ) == 1 )
    {
        std::cout << " ERROR : setPolyData" << std::endl;
        return EXIT_FAILURE;
    }

    //**************************************************************************************
    std::cout << " END TEST " << std::endl;
    return EXIT_SUCCESS;
}

//**********************************************************************************************
int testPolyData( vtkSmartPointer <vtkPolyData> inData , vtkSmartPointer <vtkPolyData> outData )
{
    // comparer pointeurs
    std::cout << "          -number of points  " << std::endl;
    if( outData -> GetNumberOfPoints() != inData -> GetNumberOfPoints() )
    {
        std::cout << " ERROR : number of points " << std::endl;
        return 1;
    }

    std::cout << "          -number of cells  " << std::endl;
    if( outData -> GetNumberOfCells() != inData -> GetNumberOfCells() )
    {
        std::cout << " ERROR : number of cells " << std::endl;
        return 1;
    }
    vtkIdType b = outData -> GetNumberOfCells();


    std::cout << "          -get points  " << std::endl;
    vtkIdType Id = 0;
    double inV[3];
    double outV[3];

    for( Id = 0 ; Id < inData -> GetNumberOfPoints() ; Id++ )
    {
        inData -> GetPoints() -> GetPoint( Id , inV );
        outData -> GetPoints() -> GetPoint( Id , outV );
        if( inV[0] != outV[0] || inV[1] != outV[1] || inV[2] != outV[2] )
        {
            std::cout << " ERROR :  points " << std::endl;
            return 1;
        }

    }

    std::cout << "          -get cells  " << std::endl;
    vtkSmartPointer <vtkIdList> inF = vtkSmartPointer <vtkIdList>::New();
    vtkSmartPointer <vtkIdList> outF = vtkSmartPointer <vtkIdList>::New();
    vtkSmartPointer <vtkCellArray> inPolys = vtkSmartPointer <vtkCellArray> ::New();
    vtkSmartPointer <vtkCellArray> outPolys = vtkSmartPointer <vtkCellArray> ::New();
    inPolys = inData -> GetPolys();
    outPolys = outData -> GetPolys();

    for( Id = 0 ; Id < b ; Id++ )
    {
        inPolys -> GetCell( Id , inF );
        outPolys -> GetCell( Id , outF );

        vtkIdType c = inF->GetNumberOfIds();
        vtkIdType d = outF->GetNumberOfIds();
        if( c!= d && c != 3 )
        {
            return 1 ;
        }
        for( vtkIdType i = 0 ; i < c ; i++ )
        {
            if( inF->GetId(i) != outF->GetId(i) )
            {
                std::cout << " ERROR :  cells " << std::endl;
                return 1;
            }
        }
    }
    return 0;
}
