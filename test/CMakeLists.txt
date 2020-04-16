set(CMAKE_CXX_STANDARD 11)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

#build exe
add_executable(testPool ${DIR_SRCS})
target_link_libraries(testPool PRIVATE ThreadPool)

add_test (NAME test_pool1 COMMAND testPool future)
add_test (NAME test_pool2 COMMAND testPool normal)