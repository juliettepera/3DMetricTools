#include "../processing.h"

int main( int argc , char* argv[] )
{
    /*displayTools testTool(1);

    // test Name
    std::string inputName = "/work/jpera/Data/Data_Slicer3/Registered_condyle_V2.vtk";
    std::string outputName;

    testTool.setName( inputName );
    outputName = testTool.getName();

    if( inputName != outputName )
    {
        std::cout << " name failure" << std::endl;
        return EXIT_FAILURE;
    }

    // test Reader
    vtkSmartPointer <vtkPolyDataReader> inputReader = vtkSmartPointer <vtkPolyDataReader>::New();
    vtkSmartPointer <vtkPolyDataReader> outputReader = vtkSmartPointer <vtkPolyDataReader>::New();

    //testTool.setReader( inputReader );
    //outputReader = testTool.getReader();

    if( inputReader != outputReader )
    {
       std::cout << " reader failure" << std::endl;
       return EXIT_FAILURE;
    }

    // test PolyData
    vtkSmartPointer <vtkPolyData> inputData = vtkSmartPointer <vtkPolyData>::New();
    vtkSmartPointer <vtkPolyData> outputData = vtkSmartPointer <vtkPolyData>::New();

    //testTool.setPolyData( inputData );
    //outputData = testTool.getPolyData();

    if( inputData != outputData )
    {
       std::cout << " Data failure" << std::endl;
       return EXIT_FAILURE;
    }

    // test Mapper
    vtkSmartPointer <vtkPolyDataMapper> inputMapper = vtkSmartPointer <vtkPolyDataMapper>::New();
    vtkSmartPointer <vtkPolyDataMapper> outputMapper = vtkSmartPointer <vtkPolyDataMapper>::New();

    testTool.setMapper( inputMapper );
    outputMapper = testTool.getMapper();

    if( inputMapper != outputMapper )
    {
       std::cout << " Mapper failure" << std::endl;
       return EXIT_FAILURE;
    }

    // test Reader
    vtkSmartPointer <vtkActor> inputActor = vtkSmartPointer <vtkActor>::New();
    vtkSmartPointer <vtkActor> outputActor = vtkSmartPointer <vtkActor>::New();

    testTool.setActor( inputActor );
    outputActor = testTool.getActor();

    if( inputActor != outputActor )
    {
       std::cout << " Actor failure" << std::endl;
       return EXIT_FAILURE;
    }

    // test Opacity
    double inputOpacity = 0.5;
    double outputOpacity;

    testTool.setOpacity( inputOpacity );
    outputOpacity = testTool.getOpacity();

    if( inputOpacity != outputOpacity )
    {
        std::cout << " Opacity failure" << std::endl;
        return EXIT_FAILURE;
    }

    // test Color
    double inputRed = 0.5;
    double outputRed;

    double inputGreen = 0.6;
    double outputGreen;

    double inputBlue = 0.5;
    double outputBlue;

    testTool.setColor( inputRed , inputGreen , inputBlue );
    outputRed = testTool.getRed();
    outputGreen = testTool.getGreen();
    outputBlue = testTool.getBlue();

    if( inputRed != outputRed )
    {
        std::cout << " Red failure" << std::endl;
        return EXIT_FAILURE;
    }
    if( inputBlue != outputBlue )
    {
        std::cout << " Blue failure" << std::endl;
        return EXIT_FAILURE;
    }
    if( inputGreen != outputGreen )
    {
        std::cout << " Green failure" << std::endl;
        return EXIT_FAILURE;
    }

    // test Smoothing
    bool inputSmoothing = false;
    bool outputSmoothing;

    testTool.setSmoothing( inputSmoothing );
    outputSmoothing = testTool.getSmoothing();

    if( inputSmoothing != outputSmoothing )
    {
        std::cout << " Smoothing failure" << std::endl;
        return EXIT_FAILURE;
    }

    // test iteration smoothing
    int inputNumber = 100;
    int outputNumber;

    testTool.setNumberOfIterationSmooth( inputNumber );
    outputNumber = testTool.getNumberOfIterationSmooth();

    if( inputNumber != outputNumber )
    {
        std::cout << " Number failure" << std::endl;
        return EXIT_FAILURE;
    }

    // test initialization
    testTool.initialization();

    outputData = testTool.getPolyData();
    //outputReader = testTool.getReader();
    outputMapper = testTool.getMapper();
    outputActor = testTool.getActor();

    int outputNumberOfPoints = outputData -> GetNumberOfPoints();

    if( outputNumberOfPoints != 8468 )
    {
        std::cout << " during initialization : number of points failure " << std::endl;
        return EXIT_FAILURE;
    }

    if( outputMapper -> GetInputConnection( 0 , 0 ) != outputReader -> GetOutputPort() )
    {
        std::cout << " during initialization : Mapper Failure " << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << " No Failure !!! " << std::endl;
    return EXIT_SUCCESS;*/

}
