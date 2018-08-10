add_definitions(-DTHREADPOOL_LIB )
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src SRC)

add_library(ThreadPool SHARED  ${SRC})

install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/ThreadPool.h ${CMAKE_CURRENT_LIST_DIR}/include/ThreadPool_global.h DESTINATION ${PROJECT_SOURCE_DIR}/include/ThreadPool)
install(TARGETS ThreadPool  
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
		LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)
