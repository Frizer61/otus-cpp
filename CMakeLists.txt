cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")

project(allocator_app VERSION 0.0.${PATCH_VERSION} LANGUAGES CXX)

add_executable(allocator_app
    main.cpp
)

set_target_properties(
    allocator_app
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(allocator_app PRIVATE /W4)
else()
    target_compile_options(
        allocator_app
        PRIVATE
            -Wall
            -Wextra
            -pedantic
            -Werror
    )
endif()

install(
    TARGETS allocator_app
    RUNTIME DESTINATION bin
)

include(CTest)
if(BUILD_TESTING)
    add_test(NAME allocator_app_runs COMMAND allocator_app)
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "example@example.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
