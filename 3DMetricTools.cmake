cmake_minimum_required(VERSION 2.8.3)

project( 3DMetricTools )

unset( USE_SYSTEM_ITK CACHE )
unset( USE_SYSTEM_SlicerExecutionModel CACHE )
unset( USE_SYSTEM_VTK CACHE )
unset( USE_GIT_PROTOCOL_3DMetricTools_SUPERBUILD CACHE )
unset( FORCE_EXTERNAL_BUILDS CACHE )

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
SETIFEMPTY( INSTALL_ARCHIVE_DESTINATION 3DMetricTools-static_lib )

if( ${EXTENSION_NAME}_BUILD_SLICER_EXTENSION )
  find_package(Subversion REQUIRED )
  find_package(Git REQUIRED )
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
  set( Use_VTK_Filter ON CACHE BOOL "Use VTK Filter" FORCE )
  set( Build_3DMeshMetric OFF CACHE BOOL "Qt GUI" FORCE )
  set( INSTALL_RUNTIME_DESTINATION ${SlicerExecutionModel_DEFAULT_CLI_INSTALL_RUNTIME_DESTINATION} )
  set( INSTALL_LIBRARY_DESTINATION ${SlicerExecutionModel_DEFAULT_CLI_INSTALL_LIBRARY_DESTINATION} )
else()
  # find the VTK headers
  find_package( VTK REQUIRED )
  if( ${VTK_MAJOR_VERSION} VERSION_LESS 6 )
    message( FATAL_ERROR "VTK version 6 or higher needed" )
  endif()
  include( ${VTK_USE_FILE} )
  # find the SlicerExecutionModel headers
  find_package(SlicerExecutionModel REQUIRED GenerateCLP)
  include( ${SlicerExecutionModel_USE_FILE} )
  include(${GenerateCLP_USE_FILE})
  # set RPATH
  if( UNIX )
    if(NOT APPLE)
      SET( CMAKE_INSTALL_RPATH "$ORIGIN/lib" )
    endif()
  endif()
  SETIFEMPTY( INSTALL_RUNTIME_DESTINATION 3DMetricTools-package )
  SETIFEMPTY( INSTALL_LIBRARY_DESTINATION ${INSTALL_RUNTIME_DESTINATION}/lib )
endif()


option( Build_Static "Static libraries and executables only" ON )
if( Build_Static )
  set( STATIC "EXECUTABLE_ONLY" )
  set( STATIC_LIB STATIC )
  set( MeshValmetLib_BINARY_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
else()
  set( STATIC_LIB SHARED )
  set( MeshValmetLib_BINARY_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
endif()
option( Use_VTK_Filter "Use VTK Filter" OFF )

# meshvalmet libraries
if( Build_3DMeshMetric OR NOT Use_VTK_Filter )
  # find the QT4 headers
  find_package( Qt4 REQUIRED )
  include( ${QT_USE_FILE} )
  set( MeshValmetLib_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries )
  add_subdirectory( Libraries )
endif()

option( Build_ModelToModelDistance "CLI" ON )
option( Build_3DMeshMetric "Qt GUI" ON )
# build errorMetric
if( Build_ModelToModelDistance )
  add_subdirectory( ModelToModelDistance )
endif()

# build meshMetric
if( Build_3DMeshMetric )
  add_subdirectory( 3DMeshMetric )
endif()

option( BUILD_TESTING "Build Testing" ON )
IF(BUILD_TESTING)
  include(CTest)
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)

if( ${EXTENSION_NAME}_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
endif()

