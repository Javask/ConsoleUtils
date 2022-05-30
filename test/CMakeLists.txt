cmake_minimum_required(VERSION 3.18)

project(ConsoleUtilsTests)

#add Catch2
CPMAddPackage("gh:catchorg/Catch2@3.0.1")

file(GLOB_RECURSE CONSOLEUTILS_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.h"
							   "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

file(GLOB_RECURSE CONSOLEUTILS_CLI_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/cli/*.h"
							   "${CMAKE_CURRENT_LIST_DIR}/cli/*.cpp")
list(REMOVE_ITEM CONSOLEUTILS_TEST_FILES ${CONSOLEUTILS_CLI_TEST_FILES})

add_executable(ConsoleUtilsTests ${CONSOLEUTILS_TEST_FILES})
target_link_libraries(ConsoleUtilsTests $<TARGET_NAME_IF_EXISTS:Catch2::Catch2WithMain> ConsoleUtils::ConsoleUtils Utilities::Utilities)
add_dependencies(ConsoleUtilsTests ConsoleUtils::ConsoleUtils)
set_target_properties(ConsoleUtilsTests PROPERTIES CXX_STANDARD 20 COMPILE_OPTIONS "${CONSOLEUTILS_CXX_FLAGS}")
target_include_directories(ConsoleUtilsTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../src" "${CMAKE_CURRENT_LIST_DIR}/../include")


add_test(ConsoleUtilsTests ${EXECUTABLE_OUTPUT_PATH}/ConsoleUtilsTests)


add_executable(ConsoleUtilsCLITest ${CONSOLEUTILS_CLI_TEST_FILES})
target_link_libraries(ConsoleUtilsCLITest ConsoleUtils::ConsoleUtils Utilities::Utilities)
add_dependencies(ConsoleUtilsCLITest ConsoleUtils::ConsoleUtils)
set_target_properties(ConsoleUtilsCLITest PROPERTIES CXX_STANDARD 20 COMPILE_OPTIONS "${CONSOLEUTILS_CXX_FLAGS}")