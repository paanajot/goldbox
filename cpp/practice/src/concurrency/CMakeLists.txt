set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(
  concurrency SHARED atomics.cpp shared_mutex.cpp cooperative_interruption.cpp
)
add_library(${CMAKE_PROJECT_NAME}::concurrency ALIAS concurrency)

target_link_libraries(concurrency PUBLIC goldboxl::cxx20 Threads::Threads)

target_include_directories(concurrency PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
