set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# logger test
add_executable(LoggerTest logger_test.cpp)
target_link_libraries(LoggerTest TurboNet)
add_test(NAME LoggerTest COMMAND LoggerTest)

# async_logging test
add_executable(AsyncLoggingTest async_logging_test.cpp)
target_link_libraries(AsyncLoggingTest TurboNet)
add_test(NAME AsyncLoggingTest COMMAND AsyncLoggingTest)
