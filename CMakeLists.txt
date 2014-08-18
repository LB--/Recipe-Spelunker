cmake_minimum_required(VERSION 3.0.1)

project("resplunk-server")

# TODO: reduce code duplication

macro(resplunk_api_headers)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_API_HEADERS ${_v})
	endforeach()
endmacro(resplunk_api_headers)

macro(resplunk_impl_headers)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_IMPL_HEADERS ${_v})
	endforeach()
endmacro(resplunk_impl_headers)

macro(resplunk_api_sources)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_API_SOURCES ${_v})
	endforeach()
endmacro(resplunk_api_sources)

macro(resplunk_impl_sources)
	foreach(_v ${ARGN})
		set_property(GLOBAL APPEND PROPERTY RESPLUNK_IMPL_SOURCES ${_v})
	endforeach()
endmacro(resplunk_impl_sources)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/include/")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src/")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include/")
link_directories   ("${CMAKE_CURRENT_LIST_DIR}/lib/")

get_property(RESPLUNK_API_HEADERS  GLOBAL PROPERTY RESPLUNK_API_HEADERS)
get_property(RESPLUNK_API_SOURCES  GLOBAL PROPERTY RESPLUNK_API_SOURCES)

#add_library          ("resplunk-server" SHARED ${RESPLUNK_API_SOURCES})
#target_link_libraries("resplunk-server"        ${RESPLINK_API_DEPENDS})

get_property(RESPLUNK_IMPL_HEADERS GLOBAL PROPERTY RESPLUNK_IMPL_HEADERS)
get_property(RESPLUNK_IMPL_SOURCES GLOBAL PROPERTY RESPLUNK_IMPL_SOURCES)

set(RESPLUNK_IMPL_HEADERS "${RESPLUNK_API_HEADERS};${RESPLUNK_IMPL_HEADERS}")
set(RESPLUNK_IMPL_SOURCES "${RESPLUNK_API_SOURCES};${RESPLUNK_IMPL_SOURCES}")
set(RESPLUNK_IMPL_DEPENDS "${RESPLUNK_API_DEPENDS};${RESPLUNK_IMPL_DEPENDS}")

add_executable       ("resplunk-server-impl" ${RESPLUNK_IMPL_SOURCES})
target_link_libraries("resplunk-server-impl" ${RESPLUNK_IMPL_DEPENDS})
