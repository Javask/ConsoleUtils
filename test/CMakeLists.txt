cmake_minimum_required(VERSION 3.18)

project(ConsoleUtilsTests)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/CPM.cmake)

#add Catch2
CPMAddPackage("gh:catchorg/Catch2@2.13.8")

file(GLOB_RECURSE CONSOLEUTILS_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.h"
							   "${CMAKE_CURRENT_LIST_DIR}/*.cpp")


add_executable(ConsoleUtilsTests ${CONSOLEUTILS_TEST_FILES})
target_link_libraries(ConsoleUtilsTests $<TARGET_NAME_IF_EXISTS:Catch2::Catch2> ConsoleUtils::ConsoleUtils)
add_dependencies(ConsoleUtilsTests ConsoleUtils::ConsoleUtils)
set_target_properties(ConsoleUtilsTests PROPERTIES CXX_STANDARD 20 COMPILE_OPTIONS "${CONSOLEUTILS_CXX_FLAGS}")
target_include_directories(ConsoleUtilsTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../src" "${CMAKE_CURRENT_LIST_DIR}/../include")


add_test(ConsoleUtilsTests ${EXECUTABLE_OUTPUT_PATH}/ConsoleUtilsTests)