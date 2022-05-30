cmake_minimum_required(VERSION 3.18)

project(ConsoleUtils VERSION 1.0.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake)

option(CONSOLEUTILS_BUILD_OWN_PACKAGES "Build all dependencies from scratch." OFF)
if(NOT CONSOLEUTILS_BUILD_OWN_PACKAGES)
  set(CPM_USE_LOCAL_PACKAGES ON)
else()
  message(STATUS "Building ConsoleUtils dependencies from scratch...")
  set(CPM_USE_LOCAL_PACKAGES OFF)
endif()

if(MSVC)
  set(CONSOLEUTILS_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
  set(CONSOLEUTILS_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic)
endif()

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

if(UNIX)
    add_definitions(-DCONSOLEUTILS_PLATFORM_LINUX)
elseif(WIN32)
    add_definitions(-DCONSOLEUTILS_PLATFORM_WINDOWS)
endif()

CPMAddPackage(NAME Utilities 
              GIT_TAG v1.0.0-alpha6
              GITHUB_REPOSITORY Javask/Utilities
              OPTIONS
                "UTILITIES_BUILD_TESTS OFF")

file(GLOB_RECURSE CONSOLEUTILS_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_library(ConsoleUtils OBJECT ${CONSOLEUTILS_SOURCE_FILES})
set_target_properties(ConsoleUtils PROPERTIES CXX_STANDARD 20 COMPILE_OPTIONS "${CONSOLEUTILS_CXX_FLAGS}")
target_link_libraries(ConsoleUtils Utilities::Utilities)
if(UNIX)
    target_link_libraries(ConsoleUtils pthread)
endif()

target_include_directories(ConsoleUtils
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(ConsoleUtils::ConsoleUtils ALIAS ConsoleUtils)

option(CONSOLEUTILS_BUILD_TESTS "Build ConsoleUtils Tests" ON)
if(CONSOLEUTILS_BUILD_TESTS)
  enable_testing()
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/")
endif()

