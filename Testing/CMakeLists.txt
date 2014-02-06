cmake_minimum_required(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)

if( NOT ${EXTENSION_NAME}_BUILD_SLICER_EXTENSION )
  find_package(ITK REQUIRED )
  include( ${ITK_USE_FILE} )
endif()

# Testing for 

set(TestingBinarydirectory ${CMAKE_CURRENT_BINARY_DIR}/Temporary)
set(TestDataDirectory ${CMAKE_CURRENT_SOURCE_DIR}/Data)


## Test 1 : ModelToModelDistance.cxx with no inputs
add_test(NAME ModelToModelDistanceHelp COMMAND ${Slicer_LAUNCH_COMMAND} $<TARGET_FILE:ModelToModelDistance> --help) # Should just display the Usage:

## Test 2 : ModelToModelDistance.cxx with inputs
add_executable(ModelToModelDistanceTest vtkTest.cxx)
target_link_libraries(ModelToModelDistanceTest ModelToModelDistanceLib ${ITK_LIBRARIES} )

set(ReferenceVTK ${TestDataDirectory}/Baseline/sphere_distance.vtk)
set(OutputVTK ${TestingBinarydirectory}/sphere_distance_output.vtk)
set(InputVTK ${TestDataDirectory}/Input/sphere.vtk)

if( NOT VTK_DEBUG_LEAKS )
add_test(NAME ModelToModelDistanceTest COMMAND ${Slicer_LAUNCH_COMMAND} $<TARGET_FILE:ModelToModelDistanceTest>
  --compare
    ${ReferenceVTK}
    ${OutputVTK}
  ModuleEntryPoint
    -i ${InputVTK}
    -j ${InputVTK} 
    -o ${OutputVTK}
 )
endif()
