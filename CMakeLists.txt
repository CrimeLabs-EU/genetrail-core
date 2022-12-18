cmake_minimum_required(VERSION 3.24)

set(genetrailcore_VERSION 0.0.1)

project(genetrailcore VERSION ${genetrailcore_VERSION}
        DESCRIPTION "The core library for genetrail2 used to build a high performance microservice applications."
        HOMEPAGE_URL "https://genetrail.crimelabs.eu"
        LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(Eigen3 CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_library(genetrailcore include/macros.h src/main.cpp include/Matrix.h include/Exception.h src/Exception.cpp include/AbstractMatrix.h src/AbstractMatrix.cpp include/BoostGraph.h include/BoostGraphParser.h)
add_library(genetrailcore::genetrailcore ALIAS genetrailcore)

if(BUILD_SHARED_LIBS AND WIN32)
    target_compile_definitions(genetrailcore
            PRIVATE "GENETRAILCORE_EXPORT=__declspec(dllexport)"
            INTERFACE "GENETRAILCORE_EXPORT=__declspec(dllimport)")
endif()

target_link_libraries(genetrailcore PRIVATE Eigen3::Eigen)
target_link_libraries(genetrailcore PRIVATE Boost::boost)

target_compile_features(genetrailcore PUBLIC cxx_std_20)
target_include_directories(genetrailcore
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

configure_package_config_file(cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/genetrailcore-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/genetrailcore
        NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/genetrailcore-config-version.cmake
        VERSION ${genetrailcore_VERSION}
        COMPATIBILITY SameMajorVersion)
install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/genetrailcore-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/genetrailcore-config-version.cmake
        DESTINATION
        ${CMAKE_INSTALL_DATADIR}/genetrailcore)
install(TARGETS genetrailcore EXPORT genetrailcore-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT genetrailcore-targets
        NAMESPACE genetrailcore::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/genetrailcore)

