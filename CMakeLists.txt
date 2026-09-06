cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")

project(print_ip VERSION 0.0.${PATCH_VERSION} LANGUAGES CXX)

add_executable(print_ip
	main.cpp
)

set_target_properties(
	print_ip
	PROPERTIES
		CXX_STANDARD 14
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
)

if(MSVC)
	target_compile_options(print_ip PRIVATE /W4)
else()
	target_compile_options(
		print_ip
		PRIVATE
			-Wall
			-Wextra
			-pedantic
			-Werror
	)
endif()

install(
	TARGETS print_ip
	RUNTIME DESTINATION bin
)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "example@example.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
