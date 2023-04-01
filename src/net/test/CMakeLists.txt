set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# timestamp test
add_executable(InetAddressTest inet_address_test.cpp)
target_link_libraries(InetAddressTest TurboNet)
add_test(NAME InetAddressTest COMMAND InetAddressTest)

# event_loop test
add_executable(EventLoopTest event_loop_test.cpp)
target_link_libraries(EventLoopTest TurboNet)
add_test(NAME EventLoopTest COMMAND EventLoopTest)