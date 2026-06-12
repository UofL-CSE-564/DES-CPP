add_executable(des_cpp_example main.cpp)
target_link_libraries(des_cpp_example PRIVATE DES::DES)

add_custom_command(TARGET des_cpp_example POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/sample.txt
		$<TARGET_FILE_DIR:des_cpp_example>/sample.txt
)
