# name of the project
project( meshMetric )

# copy the icons folder on the binary folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

# find the QT4 headers
find_package( Qt4 REQUIRED ) 
include( ${QT_USE_FILE} )

# generate moc and ui files
QT4_WRAP_CPP( QtProject_HEADERS_MOC meshMetricGui.h colorBar.h)
QT4_WRAP_UI( UI_FILES meshMetricGui.ui )

# set the sources files
set( Dsources 
meshMetric.cxx
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

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MeshValmetLib_SOURCE_DIRECTORY} ${QT_INCLUDE_DIR} )
link_directories( ${MeshValmetLib_BINARY_DIRECTORY} )
generateclp( Dsources meshMetric.xml )
add_executable( meshMetric MACOSX_BUNDLE ${Dsources} )

target_link_libraries( meshMetric MeshValmetLib ${VTK_LIBRARIES} ${QT_LIBRARIES} )
install(TARGETS meshMetric
      RUNTIME DESTINATION ${INSTALL_RUNTIME_DESTINATION} COMPONENT RuntimeLibraries
      LIBRARY DESTINATION ${INSTALL_LIBRARY_DESTINATION} COMPONENT RuntimeLibraries
      ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DESTINATION} COMPONENT Development
      BUNDLE DESTINATION  ${INSTALL_RUNTIME_DESTINATION}
      )
      
#install(DIRECTORY ${QT_LIBRARY_DIR} DESTINATION ${INSTALL_RUNTIME_DESTINATION} )
#install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${INSTALL_RUNTIME_DESTINATION}${MeshMetricDir} )


if(APPLE)
      # launcher
      add_executable( meshMetricLauncher Launcher.cxx ${Dsources} )
      target_link_libraries( meshMetricLauncher ${ITK_LIBRARIES} )
      # create the bundle
      set(plugin_dest_dir ${INSTALL_RUNTIME_DESTINATION}/meshMetric.app/Contents/MacOS)
      set(qtconf_dest_dir ${INSTALL_RUNTIME_DESTINATION}/meshMetric.app/Contents/Resources)
      set(APPS "\${CMAKE_INSTALL_PREFIX}/${INSTALL_RUNTIME_DESTINATION}/meshMetric.app")

      install(CODE "
         file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
         " COMPONENT Runtime)

      install(CODE "
         execute_process(COMMAND cp -r \"${QT_LIBRARY_DIR}/QtGui.framework/Resources/qt_menu.nib\" \".\" WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir})")
      
      set(DIRS
      ${QT_LIBRARY_DIRS}
      ${VTK_DIR}
      ${MeshValmetLib_BINARY_DIRECTORY}
      )

      install(CODE "
         include(BundleUtilities)
         fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
         " COMPONENT Runtime)

    #install icons
    install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${qtconf_dest_dir} )
    
    set(CPACK_BINARY_DRAGNDROP ON)
    include(CPack)
    install( TARGETS meshMetricLauncher  RUNTIME DESTINATION ${INSTALL_RUNTIME_DESTINATION}/meshMetric.app COMPONENT RuntimeLibraries )
else()
    install(DIRECTORY ${QT_LIBRARY_DIR} DESTINATION ${INSTALL_RUNTIME_DESTINATION} )
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${INSTALL_RUNTIME_DESTINATION} )
endif()




# for the CTest
set(BUILD_TESTING ON CACHE BOOL "Build, configure and copy testing files")
if( BUILD_TESTING )
  enable_testing()
  add_subdirectory( Testing )
endif()



