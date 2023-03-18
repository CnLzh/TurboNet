set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# timestamp test
add_executable(TimestampTest timestamp_test.cpp)
target_link_libraries(TimestampTest TurboNet)
add_test(NAME TimestampTest COMMAND TimestampTest)

# current_thread test
add_executable(CurrentThreadTest current_thread_test.cpp)
target_link_libraries(CurrentThreadTest TurboNet)
add_test(NAME CurrentThreadTest COMMAND CurrentThreadTest)

# mutex test
add_executable(MutexTest mutex_test.cpp)
target_link_libraries(MutexTest TurboNet)
add_test(NAME MutexTest COMMAND MutexTest)

# condition test
add_executable(ConditionTest condition_test.cpp)
target_link_libraries(ConditionTest TurboNet)
add_test(NAME ConditionTest COMMAND ConditionTest)
