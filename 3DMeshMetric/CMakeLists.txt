#/tools/Qt/Qt4.8.2/Qt4.8.2_linux64_THL/bin/qmake                                                                                                                                                           
#/tools/VTK/VTK_6.0.0/VTK_6.0.0_linux64_dyn_release_Qt4.8.2_Python2.7.5/lib/cmake/vtk-6.0   

# name of the project
project( meshMetric )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

# find the VTK headers
find_package( VTK 6 REQUIRED )
include( ${VTK_USE_FILE} )

# find the QT4 headers
find_package( Qt4 REQUIRED ) 
include( ${QT_USE_FILE} )

# include the directories for the binary and sources files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
${QT_INCLUDE_DIR} ${MeshValmetLib_SOURCE_DIRECTORY} )

# ?????
add_definitions( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )

# generate moc and ui files
QT4_WRAP_CPP( QtProject_HEADERS_MOC meshMetricGui.h ColormapWidget.h )
QT4_WRAP_UI( UI_FILES meshMetricGui.ui )

# set the sources files
set( Dsources 
main.cxx 
meshMetricGui.cxx
dataM.cxx
processing.cxx
display.cxx
meshValmet.cxx
ColormapWidget.cxx
${QtProject_HEADERS_MOC}
${UI_FILES}
)

# generate the executable
add_executable( meshMetric ${Dsources} )

# link the libraries to the executable
target_link_libraries( meshMetric ${QT_LIBRARIES} ${VTK_LIBRARIES} MeshValmetLib )
link_directories( ${MeshValmetLib_BINARY_DIRECTORY} )

# for the CTest
set(BUILD_TESTING ON CACHE BOOL "Build, configure and copy testing files")

if( BUILD_TESTING )
  enable_testing()
  add_subdirectory( Testing )
endif()

install(TARGETS meshMetric RUNTIME DESTINATION ${INSTALL_RUNTIME_DESTINATION} )
install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${INSTALL_RUNTIME_DESTINATION} )

