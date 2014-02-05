cmake_minimum_required(VERSION 2.8.3)
cmake_policy(VERSION 2.8.3)

set( LOCAL_PROJECT_NAME 3DMetricTools )

set(EXTENSION_NAME ModelToModelDistance)
set(EXTENSION_HOMEPAGE "https://www.nitrc.org/projects/meshmetric3d/")
set(EXTENSION_CATEGORY "Shape Analysis")
set(EXTENSION_CONTRIBUTORS "Francois Budin (UNC), Juliette Pera (UNC), Beatriz Paniagua (UNC)")
set(EXTENSION_DESCRIPTION "This extension computes the distance between two 3D models")
set(EXTENSION_ICONURL "http://slicer.org/slicerWiki/images/4/43/Slicer4ExtensionModelToModelDistance.png")
set(EXTENSION_SCREENSHOTURLS "")
set(EXTENSION_STATUS "")
set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
set(EXTENSION_BUILD_SUBDIRECTORY .)

## NOTE THERE SHOULD BE NO PROJECT STATEMENT HERE!
## This file acts as a simple switch to initiate
## two completely independant CMake build environments.

#-----------------------------------------------------------------------------
# Superbuild Option - Enabled by default
#                   Phase I:  ${LOCAL_PROJECT_NAME}_SUPERBUILD is set to ON, and the
#                             supporting packages defined in "SuperBuild.cmake"
#                             are built.  The last package in "SuperBuild.cmake"
#                             to be built is a recursive call to this
#                             file with ${LOCAL_PROJECT_NAME}_SUPERBUILD explicitly
#                             set to "OFF" to initiate Phase II
#
#                   Phase II: Build the ${LOCAL_PROJECT_NAME}, referencing the support
#                             packages built in Phase I.
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# 3DSlicer Extension
#-----------------------------------------------------------------------------

if( ${EXTENSION_NAME}_BUILD_SLICER_EXTENSION )
  set( ${LOCAL_PROJECT_NAME}_SUPERBUILD OFF CACHE BOOL 
        "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake." FORCE )
endif()

#-----------------------------------------------------------------------------

option(${LOCAL_PROJECT_NAME}_SUPERBUILD "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake." ON)

#-----------------------------------------------------------------------------
# Superbuild script
#-----------------------------------------------------------------------------
if(${LOCAL_PROJECT_NAME}_SUPERBUILD)
  project(${LOCAL_PROJECT_NAME}_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
else()
  project(${LOCAL_PROJECT_NAME})
  include("${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_PROJECT_NAME}.cmake")
  return()
endif()

message(FATAL_ERROR "You should never reach this point !")
