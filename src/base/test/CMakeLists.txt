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

# count_down_latch test
add_executable(CountDownLatchTest count_down_latch_test.cpp)
target_link_libraries(CountDownLatchTest TurboNet)
add_test(NAME CountDownLatchTest COMMAND CountDownLatchTest)

# thread test
add_executable(ThreadTest thread_test.cpp)
target_link_libraries(ThreadTest TurboNet)
add_test(NAME ThreadTest COMMAND ThreadTest)

# thread_pool test
add_executable(ThreadPoolTest thread_pool_test.cpp)
target_link_libraries(ThreadPoolTest TurboNet)
add_test(NAME ThreadPoolTest COMMAND ThreadPoolTest)

# logger test
add_executable(LoggerTest logger_test.cpp)
target_link_libraries(LoggerTest TurboNet)
add_test(NAME LoggerTest COMMAND LoggerTest)

# async_logging test
add_executable(AsyncLoggingTest async_logging_test.cpp)
target_link_libraries(AsyncLoggingTest TurboNet)
add_test(NAME AsyncLoggingTest COMMAND AsyncLoggingTest)