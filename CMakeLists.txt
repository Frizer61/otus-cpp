cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")

project(bayan VERSION 0.0.${PATCH_VERSION} LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS filesystem program_options)

add_executable(bayan main.cpp bayan.cpp)

set_target_properties(bayan PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_link_libraries(bayan PRIVATE Boost::filesystem Boost::program_options)

if(MSVC)
	target_compile_options(bayan PRIVATE /W4)
else()
	target_compile_options(bayan PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

include(CTest)
if(BUILD_TESTING)
	find_package(Python3 COMPONENTS Interpreter REQUIRED)
	add_test(NAME bayan_tests COMMAND ${Python3_EXECUTABLE}
		${CMAKE_CURRENT_SOURCE_DIR}/test_bayan.py $<TARGET_FILE:bayan>)
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "example@example.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
