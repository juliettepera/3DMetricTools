cmake_minimum_required(VERSION 2.8)
# name of the project
project( 3DMetricTools )

## A simple macro to set variables ONLY if it has not been set
## This is needed when stand-alone packages are combined into
## a larger package, and the desired behavior is that all the
## binary results end up in the combined directory.
if(NOT SETIFEMPTY)
macro(SETIFEMPTY)
  set(KEY ${ARGV0})
  set(VALUE ${ARGV1})
  if(NOT ${KEY})
    set(${KEY} ${VALUE})
  endif(NOT ${KEY})
endmacro(SETIFEMPTY KEY VALUE)
endif(NOT SETIFEMPTY)

# sub-directory of the executable
## A simple macro to set variables ONLY if it has not been set
## This is needed when stand-alone packages are combined into
## a larger package, and the desired behavior is that all the
## binary results end up in the combined directory.
if(NOT SETIFEMPTY)
macro(SETIFEMPTY)
  set(KEY ${ARGV0})
  set(VALUE ${ARGV1})
  if(NOT ${KEY})
    set(${KEY} ${VALUE})
  endif(NOT ${KEY})
endmacro(SETIFEMPTY KEY VALUE)
endif(NOT SETIFEMPTY)
SETIFEMPTY( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
SETIFEMPTY( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib )
SETIFEMPTY( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/static )
SETIFEMPTY( INSTALL_RUNTIME_DESTINATION bin )
SETIFEMPTY( INSTALL_LIBRARY_DESTINATION bin )
SETIFEMPTY( INSTALL_ARCHIVE_DESTINATION lib )

# find the VTK headers
find_package( VTK 6 REQUIRED )
include( ${VTK_USE_FILE} )

option( Build_Static "Static libraries and executables only" ON )
if( Build_Static )
  set( STATIC "EXECUTABLE_ONLY" )
  set( STATIC_LIB STATIC )
  set( MeshValmetLib_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
else()
  set( STATIC_LIB SHARED )
  set( MeshValmetLib_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
endif()
option( Use_VTK_Filter "Use VTK Filter" OFF )
set( MeshValmetLib_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries )
add_subdirectory( Libraries )
option( Build_3DErrorMetric "CLI" ON )
option( Build_3DMeshMetric "Qt GUI" ON )

if( Build_3DErrorMetric )
  add_subdirectory( 3DErrorMetric )
endif()

if( Build_3DMeshMetric )
  add_subdirectory( 3DMeshMetric )
endif()



