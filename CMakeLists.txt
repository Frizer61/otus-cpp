cmake_minimum_required(VERSION 3.12)

project(allocator_app)

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