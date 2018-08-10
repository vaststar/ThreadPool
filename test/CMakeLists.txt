#contain srcs
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

#build exe
add_executable(test ${DIR_SRCS})

target_link_libraries(test ${PROJECT_SOURCE_DIR}/lib/ThreadPool.lib)

install(TARGETS test  
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)