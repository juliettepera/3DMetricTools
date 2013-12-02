# name of the project
project( meshMetric )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

# find the QT4 headers
find_package( Qt4 REQUIRED ) 
include( ${QT_USE_FILE} )

# include the directories for the binary and sources files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
${QT_INCLUDE_DIR} ${MeshValmetLib_SOURCE_DIRECTORY} )

# generate moc and ui files
QT4_WRAP_CPP( QtProject_HEADERS_MOC meshMetricGui.h colorBar.h)
QT4_WRAP_UI( UI_FILES meshMetricGui.ui )

# set the sources files
set( Dsources 
meshMetricGui.cxx
dataM.cxx
processing.cxx
display.cxx
colorBar.cxx
${QtProject_HEADERS_MOC}
${UI_FILES}
)

# xml info in GUI
file(READ meshMetric.xml var)

string(REGEX MATCH "<version>.*</version>" ext "${var}")
string(REPLACE "<version>" "" version_number ${ext} )
string(REPLACE "</version>" "" version_number ${version_number})

ADD_DEFINITIONS(-DMESHMETRIC3D_VERSION="${version_number}")

string(REGEX MATCH "<contributor>.*</contributor>" ext "${var}")
string(REPLACE "<contributor>" "" contributor_names ${ext} )
string(REPLACE "</contributor>" "" contributor_names ${contributor_names})

ADD_DEFINITIONS(-DMESHMETRIC3D_CONTRIBUTOR="${contributor_names}")

string(REGEX MATCH "<documentation-url>.*</documentation-url>" ext "${var}")
string(REPLACE "<documentation-url>" "" documentation-url ${ext} )
string(REPLACE "</documentation-url>" "" documentation-url ${documentation-url})

ADD_DEFINITIONS(-DMESHMETRIC3D_DOCUMENTATION="${documentation-url}")



# generate the executable
#add_executable( meshMetric ${Dsources} )
SEMMacroBuildCLI(
    NAME meshMetric
    ${STATIC}
    ADDITIONAL_SRCS ${Dsources}
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MeshValmetLib_SOURCE_DIRECTORY}
    LINK_DIRECTORIES ${MeshValmetLib_BINARY_DIRECTORY}
    TARGET_LIBRARIES MeshValmetLib ${VTK_LIBRARIES} ${QT_LIBRARIES}
    INSTALL_RUNTIME_DESTINATION ${INSTALL_RUNTIME_DESTINATION}
    INSTALL_LIBRARY_DESTINATION ${INSTALL_LIBRARY_DESTINATION}
    INSTALL_ARCHIVE_DESTINATION ${INSTALL_ARCHIVE_DESTINATION}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)


# for the CTest
set(BUILD_TESTING ON CACHE BOOL "Build, configure and copy testing files")

if( BUILD_TESTING )
  enable_testing()
  add_subdirectory( Testing )
endif()

#install(TARGETS meshMetric RUNTIME DESTINATION ${INSTALL_RUNTIME_DESTINATION} )
install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${INSTALL_RUNTIME_DESTINATION} )


