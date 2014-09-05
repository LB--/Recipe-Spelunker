cmake_minimum_required(VERSION 3.0.1)

project("resplunk-server")

# TODO: reduce code duplication

macro(resplunk_api_headers)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_API_HEADERS ${_v})
	endforeach()
endmacro(resplunk_api_headers)

macro(resplunk_api_sources)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_API_SOURCES ${_v})
	endforeach()
endmacro(resplunk_api_sources)

macro(resplunk_api_depends)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_API_DEPENDS ${_v})
	endforeach()
endmacro(resplunk_api_depends)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/include/")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src/")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include/")
link_directories   ("${CMAKE_CURRENT_LIST_DIR}/lib/")

get_property(RESPLUNK_API_HEADERS GLOBAL PROPERTY RESPLUNK_API_HEADERS)
get_property(RESPLUNK_API_SOURCES GLOBAL PROPERTY RESPLUNK_API_SOURCES)
get_property(RESPLUNK_API_DEPENDS GLOBAL PROPERTY RESPLUNK_API_DEPENDS)

add_library               ("resplunk-server-obj" OBJECT ${RESPLUNK_API_SOURCES})
add_library               ("resplunk-server"     SHARED $<TARGET_OBJECTS:resplunk-server-obj>)
target_link_libraries     ("resplunk-server"            ${RESPLINK_API_DEPENDS})
target_include_directories("resplunk-server" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")

enable_testing()
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/")
