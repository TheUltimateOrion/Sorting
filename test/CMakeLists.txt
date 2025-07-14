file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/src/sort/*.cpp)

add_executable(${PROJECT}_TEST main.test.cc ${TEST_FILES})

target_include_directories(${PROJECT}_TEST PRIVATE ${PROJECT_SOURCE_DIR}/includes/)
target_link_libraries(${PROJECT}_TEST PRIVATE GTest::gtest_main)
target_compile_definitions(${PROJECT}_TEST PRIVATE TESTING)

gtest_discover_tests(${PROJECT}_TEST)
set_target_properties(${PROJECT}_TEST PROPERTIES FOLDER test)