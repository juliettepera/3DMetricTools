// My library
#include "ModelToModelDistanceCLP.h"
#ifdef USE_VTK_FILTER
  #include <vtkDistancePolyDataFilter.h>
#else
  #include "MeshValmet.h"
#endif
#include <vtkVersion.h>
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkCleanPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkCellData.h>

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
  #if VTK_MAJOR_VERSION > 5
    Cleaner->SetInputData( polyData ) ;
  #else
    Cleaner->SetInput( polyData ) ;
  #endif
    Cleaner->Update() ;
    vtkSmartPointer <vtkTriangleFilter> Triangler = vtkSmartPointer <vtkTriangleFilter>::New() ;
  #if VTK_MAJOR_VERSION > 5
    Triangler->SetInputData( Cleaner->GetOutput() ) ;
  #else
    Triangler->SetInput( Cleaner->GetOutput() ) ;
  #endif
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
  #if VTK_MAJOR_VERSION > 5
    distanceFilter->SetInputData( 0, inPolyData1 ) ;
    distanceFilter->SetInputData( 1, inPolyData2 ) ;
  #else
    distanceFilter->SetInput( 0, inPolyData1 ) ;
    distanceFilter->SetInput( 1, inPolyData2 ) ;
  #endif
    distanceFilter->SetSignedDistance( signedDistance ) ;
    distanceFilter->Update();
    //We are only interested in the point distance, not in the cell distance, so we remove the cell distance
    distanceFilter->GetOutput()->GetCellData()->RemoveArray("Distance") ;
    vtkSmartPointer <vtkCleanPolyData> Cleaner = vtkSmartPointer <vtkCleanPolyData>::New() ;
  #if VTK_MAJOR_VERSION > 5
    Cleaner->SetInputData( distanceFilter->GetOutput() ) ;
  #else
    Cleaner->SetInput( distanceFilter->GetOutput() ) ;
  #endif
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
  #if VTK_MAJOR_VERSION > 5
    Cleaner->SetInputData( errorComputeFilter.GetFinalData() ) ;
  #else
    Cleaner->SetInput( errorComputeFilter.GetFinalData() ) ;
  #endif
    Cleaner->Update() ;
#endif
	  if (vtkOutput.rfind(".vtk") != std::string::npos )
	  {
			vtkSmartPointer<vtkPolyDataWriter> writer = vtkPolyDataWriter::New() ;
			writer->SetFileName( vtkOutput.c_str() ) ;
    #if VTK_MAJOR_VERSION > 5
      writer->SetInputData( Cleaner->GetOutput() ) ;
    #else
      writer->SetInput( Cleaner->GetOutput() ) ;
    #endif
			writer->Update();
	  }
	  else if( vtkOutput.rfind( ".vtp" ) != std::string::npos )
	  {
		  vtkSmartPointer< vtkXMLPolyDataWriter > writer = vtkXMLPolyDataWriter::New() ;
		  writer->SetFileName( vtkOutput.c_str() ) ;
    #if VTK_MAJOR_VERSION > 5
		  writer->SetInputData( Cleaner->GetOutput() ) ;
    #else
		  writer->SetInput( Cleaner->GetOutput() ) ;
    #endif
		  writer->Update() ;
	  }
  return 0 ;
}
