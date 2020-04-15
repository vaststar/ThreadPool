set(CMAKE_CXX_STANDARD 11)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

#build exe
add_executable(testPool ${DIR_SRCS})
target_link_libraries(testPool PUBLIC ThreadPool)

add_test (NAME MyTest COMMAND testPool future)
# add_test (NAME MyTest1 COMMAND testPool normal)