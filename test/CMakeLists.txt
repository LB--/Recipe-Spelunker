
macro(resplunk_api_test test_name)
	get_property(RESPLUNK_API_HEADERS GLOBAL PROPERTY RESPLUNK_API_HEADERS)
	get_property(RESPLUNK_API_SOURCES GLOBAL PROPERTY RESPLUNK_API_SOURCES)
	get_property(RESPLUNK_API_DEPENDS GLOBAL PROPERTY RESPLUNK_API_DEPENDS)

	string(REGEX REPLACE "[^a-zA-Z0-9_-]" "_" full_name "${CMAKE_CURRENT_LIST_DIR}/${test_name}")
	string(LENGTH ${CMAKE_SOURCE_DIR} start)
	string(SUBSTRING ${full_name} ${start} -1 full_name)

	set(test_sources "${CMAKE_CURRENT_LIST_DIR}/${test_name}.cpp")

	add_executable       (${full_name} ${test_sources} $<TARGET_OBJECTS:resplunk-server-obj>)
	target_link_libraries(${full_name} ${RESPLUNK_API_DEPENDS})
	add_test(
		NAME ${full_name}
		COMMAND ${full_name} ${ARGN}
	)
endmacro(resplunk_api_test)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/resplunk/")
