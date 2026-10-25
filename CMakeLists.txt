cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")

project(bulk VERSION 0.0.${PATCH_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bulk_lib bulk.cpp logger.cpp)
add_executable(bulk main.cpp)
target_link_libraries(bulk PRIVATE bulk_lib)

include(CTest)
if(BUILD_TESTING)
	add_executable(test_bulk test_bulk.cpp)
	target_link_libraries(test_bulk PRIVATE bulk_lib)
	add_test(NAME test_bulk COMMAND test_bulk)
endif()

if(MSVC)
	target_compile_options(bulk_lib PRIVATE /W4)
	target_compile_options(bulk PRIVATE /W4)
else()
	target_compile_options(bulk_lib PRIVATE -Wall -Wextra -pedantic -Werror)
	target_compile_options(bulk PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "example@example.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
