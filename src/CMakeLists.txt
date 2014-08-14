
include_directories("${CMAKE_CURRENT_LIST_DIR}/")

resplunk_impl_sources(
	"${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/resplunk/")
