// My library
#include "errorMetricCLP.h"
#ifdef USE_VTK_FILTER
  #include <vtkDistancePolyDataFilter.h>
#else
  #include "MeshValmet.h"
#endif
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkCleanPolyData.h>
#include <vtkTriangleFilter.h>

//class ErrorObserver copied from http://www.vtk.org/Wiki/VTK/Examples/Cxx/Utilities/ObserveError
class ErrorObserver : public vtkCommand
{
public:
  ErrorObserver():
    Error(false),
    Warning(false),
    ErrorMessage(""),
    WarningMessage("") {}
  static ErrorObserver *New()
  {
  return new ErrorObserver;
  }
  bool GetError() const
  {
  return this->Error;
  }
  bool GetWarning() const
  {
  return this->Warning;
  }
  void Clear()
  {
  this->Error = false;
  this->Warning = false;
  this->ErrorMessage = "";
  this->WarningMessage = "";
  }
  virtual void Execute(vtkObject *vtkNotUsed(caller),
                       unsigned long event,
                       void *calldata)
  {
  switch(event)
    {
    case vtkCommand::ErrorEvent:
      ErrorMessage = static_cast<char *>(calldata);
      this->Error = true;
      break;
    case vtkCommand::WarningEvent:
      WarningMessage = static_cast<char *>(calldata);
      this->Warning = true;
      break;
    }
  }
  std::string GetErrorMessage()
  {
  return ErrorMessage;
  }
std::string GetWarningMessage()
  {
  return WarningMessage;
  }
private:
  bool        Error;
  bool        Warning;
  std::string ErrorMessage;
  std::string WarningMessage;
};


int TriangulateAndClean(vtkSmartPointer<vtkPolyData> &polyData )
{
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New() ;
    Cleaner->SetInputData( polyData ) ;
    Cleaner->Update() ;
    vtkSmartPointer <vtkTriangleFilter> Triangler = vtkSmartPointer <vtkTriangleFilter>::New() ;
    Triangler->SetInputData( Cleaner->GetOutput() ) ;
    Triangler->Update() ;
    polyData = Triangler->GetOutput() ;
    return 0 ;
}


int ReadVTK( std::string input , vtkSmartPointer<vtkPolyData> &polyData )
{
  vtkSmartPointer<ErrorObserver>  errorObserver =
  vtkSmartPointer<ErrorObserver>::New();
	vtkSmartPointer<vtkPolyData> PolyData ;
	if( input.rfind( ".vtk" ) != std::string::npos )
	{
		vtkSmartPointer< vtkPolyDataReader > polyReader = vtkPolyDataReader::New() ;
    polyReader->AddObserver( vtkCommand::ErrorEvent , errorObserver ) ;
		polyReader->SetFileName( input.c_str() ) ;
		polyData = polyReader->GetOutput() ;
		polyReader->Update() ;
	}
	else if( input.rfind( ".vtp" ) != std::string::npos )
	{
		vtkSmartPointer< vtkXMLPolyDataReader > xmlReader = vtkXMLPolyDataReader::New() ;
		xmlReader->SetFileName( input.c_str() ) ;
    xmlReader->AddObserver( vtkCommand::ErrorEvent , errorObserver ) ;
		polyData = xmlReader->GetOutput() ;
		xmlReader->Update() ;
	}
  else
  {
    std::cerr << "Input file format not handled: " << input << " cannot be read" << std::endl ;
    return 1 ;
  }
  if (errorObserver->GetError())
  {
    std::cout << "Caught error opening " << input << std::endl ;
    return 1 ;
  }
  return 0 ;
}

int main( int argc , char* argv[] )
{
    PARSE_ARGS ;
#ifdef USE_VTK_FILTER
  std::cout<<"WARNING"<<std::endl ;
  std::cout<<"\'samplingStep\' and \'minSampleFrequency\' are not used when compiled with USE_VTK_FILTER" << std::endl ;
#endif
    if( vtkFile1.empty() || vtkFile2.empty() )
    {
      std::cout << "Specify 2 vtk input files" << std::endl ;
      return 1 ;
    }
    if( vtkOutput.empty() )
    {
      std::cout << "Specify an output file name" << std::endl ;
      return 1 ;
    }
    vtkSmartPointer<vtkPolyData> inPolyData1 = vtkSmartPointer<vtkPolyData>::New() ;
    if( ReadVTK( vtkFile1 , inPolyData1 ) )
    {
      return 1 ;
    }
    vtkSmartPointer<vtkPolyData> inPolyData2 = vtkSmartPointer<vtkPolyData>::New() ;
    if( ReadVTK( vtkFile2 , inPolyData2 ) )
    {
      return 1 ;
    }
    TriangulateAndClean( inPolyData1 ) ;
    TriangulateAndClean( inPolyData2 ) ;
#ifdef USE_VTK_FILTER
    vtkSmartPointer<vtkDistancePolyDataFilter> distanceFilter =
      vtkSmartPointer<vtkDistancePolyDataFilter>::New();
 
    distanceFilter->SetInputData( 0, inPolyData1 ) ;
    distanceFilter->SetInputData( 1, inPolyData2 ) ;
    distanceFilter->SetSignedDistance( signedDistance ) ;
    distanceFilter->Update();
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New() ;
    Cleaner->SetInputData( distanceFilter->GetOutput() ) ;
    Cleaner->Update() ;
#else
    meshValmet errorComputeFilter ;
    errorComputeFilter.SetData1( inPolyData1 );
    errorComputeFilter.SetData2( inPolyData2 );
    errorComputeFilter.SetSignedDistance( signedDistance ) ;
    errorComputeFilter.SetSamplingStep( samplingStep ) ;
    errorComputeFilter.SetMinSampleFrequency( minSampleFrequency ) ;
    errorComputeFilter.CalculateError() ;
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New() ;
    Cleaner->SetInputData( errorComputeFilter.GetFinalData() ) ;
    Cleaner->Update() ;
#endif



	  if (vtkOutput.rfind(".vtk") != std::string::npos )
	  {
			vtkSmartPointer<vtkPolyDataWriter> writer = vtkPolyDataWriter::New() ;
			writer->SetFileName( vtkOutput.c_str() ) ;
      writer->SetInputData( Cleaner->GetOutput() ) ;
			writer->Update();
	  }
	  else if( vtkOutput.rfind( ".vtp" ) != std::string::npos )
	  {
		  vtkSmartPointer< vtkXMLPolyDataWriter > writer = vtkXMLPolyDataWriter::New() ;
		  writer->SetFileName( vtkOutput.c_str() ) ;
		  writer->SetInputData( Cleaner->GetOutput() ) ;
		  writer->Update() ;
	  }
  return 0 ;
}
