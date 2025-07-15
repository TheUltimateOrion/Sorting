file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/src/sort/*.cpp)

add_executable(${PROJECT_NAME}_TEST main.test.cc ${TEST_FILES})

target_include_directories(${PROJECT_NAME}_TEST PRIVATE 
    ${PROJECT_SOURCE_DIR}/includes/
)

target_link_libraries(${PROJECT_NAME}_TEST PRIVATE GTest::gtest_main)
target_compile_definitions(${PROJECT_NAME}_TEST PRIVATE TESTING)

gtest_discover_tests(${PROJECT_NAME}_TEST)
set_target_properties(${PROJECT_NAME}_TEST PROPERTIES FOLDER test)