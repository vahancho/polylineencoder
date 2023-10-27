cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)

option(ENABLE_TESTING "Enable unit test build" OFF)

project(polylineencoder
        VERSION 2.1.0
        DESCRIPTION "A header-only c++ library that provides implementation of Google Encoded Polyline Algorithm Format."
        HOMEPAGE_URL "https://github.com/vahancho/polylineencoder")

include(GNUInstallDirs)

add_library(polylineencoder INTERFACE)

target_include_directories(polylineencoder INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(polylineencoder INTERFACE cxx_std_11)

# Installation
install(TARGETS polylineencoder
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("polylineencoderConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE gepaf::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES ${PROJECT_SOURCE_DIR}/src/polylineencoder.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Enable packaging with CPack
include(CPack)

if (ENABLE_TESTING)
    enable_testing()
    include(Dart)

    add_subdirectory(test)
endif()
