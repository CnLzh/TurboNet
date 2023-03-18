set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# mutex test
add_executable(MutexTest mutex_test.cpp)
target_link_libraries(MutexTest TurboNet)
add_test(NAME MutexTest COMMAND MutexTest)
