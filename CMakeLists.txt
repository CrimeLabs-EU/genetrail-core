cmake_minimum_required(VERSION 3.24)

set(genetrailcore_VERSION 0.0.1)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(genetrailcore VERSION ${genetrailcore_VERSION}
        DESCRIPTION "The core library for genetrail2 used to build a high performance microservice applications."
        HOMEPAGE_URL "https://genetrail.crimelabs.eu"
        LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(Eigen3 CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams)

set(CMAKE_CXX_STANDARD 20)

add_library(genetrailcore SHARED)
target_sources(genetrailcore PRIVATE
        include/macros.h
        include/Matrix.h
        include/Exception.h
        src/Exception.cpp
        include/AbstractMatrix.h
        src/AbstractMatrix.cpp
        include/BoostGraph.h
        include/BoostGraphParser.h
        include/BoostGraphProcessor.h
        src/BoostGraphProcessor.cpp
        include/Category.h
        include/EntityDatabase.h
        src/EntityDatabase.cpp
        include/Metadata.h
        src/Metadata.cpp
        src/Category.cpp
        include/CategoryDatabase.h
        include/Editor.h
        src/CategoryDatabase.cpp
        include/File.h
        src/File.cpp
        include/CombineReducedEnrichments.h
        src/CombineReducedEnrichments.cpp
        include/Statistic.h
        include/ConfidenceInterval.h
        include/DenseMatrix.h
        src/DenseMatrix.cpp
        include/DenseColumnSubset.h
        src/DenseColumnSubset.cpp)
add_library(genetrailcore::genetrailcore ALIAS genetrailcore)


if (BUILD_SHARED_LIBS AND WIN32)
    target_compile_definitions(genetrailcore
            PRIVATE "GENETRAILCORE_EXPORT=__declspec(dllexport)"
            INTERFACE "GENETRAILCORE_EXPORT=__declspec(dllimport)")
endif ()

target_link_libraries(genetrailcore PRIVATE Eigen3::Eigen)
target_link_libraries(genetrailcore PRIVATE Boost::boost ${Boost_LIBRARIES})

target_compile_features(genetrailcore PRIVATE cxx_std_20)
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
        NAMESPACE GeneTrail::Core::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/genetrailcore)

