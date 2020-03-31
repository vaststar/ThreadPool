cmake_minimum_required(VERSION 3.8)
project(ThreadPool VERSION 1.0
                  DESCRIPTION "thread pool"
                  LANGUAGES CXX
        )

set(TARGET_SOURCE 
        include/ThreadPool_global.h
        include/ThreadPool.h
        src/ThreadPool.cpp
    )	

##build shared library
add_library(${PROJECT_NAME} SHARED  ${TARGET_SOURCE})   
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE THREADPOOL_DLL THREADPOOL_LIB)
endif(WIN32)

target_include_directories(${PROJECT_NAME} PUBLIC 
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
                           )

## install 
install(FILES include/ThreadPool_global.h include/ThreadPool.h  DESTINATION include/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} 
        EXPORT ${PROJECT_NAME}Config
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin
		ARCHIVE DESTINATION lib)

install(EXPORT ${PROJECT_NAME}Config 
        DESTINATION cmake
        NAMESPACE ThreadPoolExport::
        )

##for project tree view
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCE})
