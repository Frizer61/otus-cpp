cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")

project(editor VERSION 0.0.${PATCH_VERSION} LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EDITOR_HEADERS
	include/editor/Circle.h
	include/editor/ConsoleRenderer.h
	include/editor/Controller.h
	include/editor/Document.h
	include/editor/EditorView.h
	include/editor/IDocumentIO.h
	include/editor/IRenderer.h
	include/editor/Point.h
	include/editor/Primitive.h
	include/editor/Rectangle.h
	include/editor/StubDocumentIO.h
)

add_executable(editor
	src/Circle.cpp
	src/ConsoleRenderer.cpp
	src/Controller.cpp
	src/Document.cpp
	src/EditorView.cpp
	src/Rectangle.cpp
	src/StubDocumentIO.cpp
	src/main.cpp
)

# Каждый заголовок компилируется отдельно, чтобы были видны недостающие #include.
foreach(header ${EDITOR_HEADERS})
	get_filename_component(headerName ${header} NAME_WE)
	set(checkSource "${CMAKE_CURRENT_BINARY_DIR}/header_check/${headerName}.cpp")
	file(GENERATE
		OUTPUT "${checkSource}"
		CONTENT "#include \"editor/${headerName}.h\"\n"
	)
	list(APPEND HEADER_CHECK_SOURCES "${checkSource}")
endforeach()

add_library(editor_header_check OBJECT ${HEADER_CHECK_SOURCES})

foreach(target editor editor_header_check)
	target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}/include")

	set_target_properties(
		${target}
		PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
	)

	if(MSVC)
		target_compile_options(${target} PRIVATE /W4)
	else()
		target_compile_options(
			${target}
			PRIVATE
				-Wall
				-Wextra
				-pedantic
				-Werror
		)
	endif()
endforeach()

install(
	TARGETS editor
	RUNTIME DESTINATION bin
)

include(CTest)
if(BUILD_TESTING)
	add_test(NAME editor_runs COMMAND editor)
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "example@example.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
