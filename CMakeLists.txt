cmake_minimum_required(VERSION 3.20)

project(DES-CPP
	VERSION 1.0.0
	DESCRIPTION "A C++ implementation of the DES encryption standard"
	LANGUAGES CXX
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
	set(DES_CPP_IS_TOP_LEVEL ON)
else()
	set(DES_CPP_IS_TOP_LEVEL OFF)
endif()

option(DES_CPP_BUILD_TESTS "Build DES-CPP unit tests" ${DES_CPP_IS_TOP_LEVEL})
option(DES_CPP_BUILD_EXAMPLES "Build DES-CPP examples" ${DES_CPP_IS_TOP_LEVEL})

include(GNUInstallDirs)

add_library(des_cpp src/DES.cpp)
add_library(DES::DES ALIAS des_cpp)

target_compile_features(des_cpp PUBLIC cxx_std_17)
target_include_directories(des_cpp
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(des_cpp PROPERTIES
	EXPORT_NAME DES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(DES_CPP_BUILD_EXAMPLES)
	add_subdirectory(example)
endif()

include(CTest)
if(DES_CPP_BUILD_TESTS AND BUILD_TESTING)
	add_subdirectory(tests)
endif()

install(TARGETS des_cpp
	EXPORT DES-CPPTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT DES-CPPTargets
	FILE DES-CPPTargets.cmake
	NAMESPACE DES::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DES-CPP
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/DES-CPPConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/DES-CPPConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DES-CPP
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/DES-CPPConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/DES-CPPConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/DES-CPPConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DES-CPP
)

