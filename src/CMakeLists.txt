find_package(Magnum REQUIRED
	MeshTools
	Primitives
	Shaders
	Sdl2Application)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORRADE_CXX_FLAGS}")
include_directories(${MAGNUM_INCLUDE_DIRS} ${MAGNUM_APPLICATION_INCLUDE_DIRS})

add_executable(resplunk-client resplunk-client.cpp)
target_link_libraries(resplunk-client
	${MAGNUM_APPLICATION_LIBRARIES}
	${MAGNUM_LIBRARIES}
	${MAGNUM_MESHTOOLS_LIBRARIES}
	${MAGNUM_PRIMITIVES_LIBRARIES}
	${MAGNUM_SHADERS_LIBRARIES})
