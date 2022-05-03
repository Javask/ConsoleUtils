include(GNUInstallDirs)


install(TARGETS ConsoleUtils
    EXPORT ConsoleUtils-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT ConsoleUtils-targets
  FILE
    ConsoleUtilsTargets.cmake
  NAMESPACE
  ConsoleUtils::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/ConsoleUtils
)

get_filename_component(ConsoleUtils_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

if(NOT TARGET ConsoleUtils::ConsoleUtils)
    include("${ConsoleUtils_CMAKE_DIR}/ConsoleUtilsTargets.cmake")
endif()