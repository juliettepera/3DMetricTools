# test 1
add_executable( testData testData.cxx ../dataM.cxx )
target_link_libraries( testData ${VTK_LIBRARIES} )
add_test(NAME testData COMMAND $<TARGET_FILE:testData> )

# test 2
#ADD_EXECUTABLE( testDisplay testDisplay.cxx ../display.cxx )
#TARGET_LINK_LIBRARIES( testDisplay ${VTK_LIBRARIES} )
#ADD_TEST(NAME testDisplay COMMAND $<TARGET_FILE:testDisplay> )

# test 3
#ADD_EXECUTABLE( testProcessing testProcessing.cxx ../processing.cxx )
#TARGET_LINK_LIBRARIES( testProcessing ${VTK_LIBRARIES} )
#ADD_TEST(NAME testProcessing COMMAND $<TARGET_FILE:testProcessing> )
