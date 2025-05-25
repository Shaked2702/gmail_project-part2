cmake_minimum_required(VERSION 3.14)
project(proj_ex1)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create executable named ids_tests for IDataStructure tests file.
add_executable(ids_tests DataStructureTest.cpp
 HashFunctions.cpp BloomFilter.cpp)
target_link_libraries(ids_tests gtest_main)
set_target_properties(ids_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")

# Create executable named io_tests for IO tests file.
add_executable(io_tests IOTest.cpp
 SocketIO.cpp StreamIO.cpp)
target_link_libraries(io_tests gtest_main)
set_target_properties(io_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")

# Create executable named command_tests for ICommand tests file.
add_executable(command_tests CommandTest.cpp
 StreamIO.cpp HashFunctions.cpp BloomFilter.cpp DeleteCommand.cpp CheckCommand.cpp AddCommand.cpp)
target_link_libraries(command_tests gtest_main)
set_target_properties(command_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")

# Create executable named setup_tests for ISetup tests file.
add_executable(setup_tests SetupTest.cpp
StreamIO.cpp HashFunctions.cpp BloomFilter.cpp LineSetup.cpp)
target_link_libraries(setup_tests gtest_main)
set_target_properties(setup_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")

# Create executable named menu_tests for ISetup tests file.
add_executable(menu_tests MenuTest.cpp
StreamIO.cpp ConsoleMenu.cpp LineSetup.cpp HashFunctions.cpp BloomFilter.cpp DeleteCommand.cpp CheckCommand.cpp AddCommand.cpp)
target_link_libraries(menu_tests gtest_main)
set_target_properties(menu_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")

# Create executable named app_tests for App tests file.
add_executable(app_tests AppTest.cpp
SocketIO.cpp StreamIO.cpp App.cpp ConsoleMenu.cpp LineSetup.cpp HashFunctions.cpp BloomFilter.cpp DeleteCommand.cpp CheckCommand.cpp AddCommand.cpp)
target_link_libraries(app_tests gtest_main)
set_target_properties(app_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")

# Create executable named main for the project main executable file.
add_executable(main Main.cpp
StreamIO.cpp SocketIO.cpp App.cpp ConsoleMenu.cpp LineSetup.cpp HashFunctions.cpp BloomFilter.cpp DeleteCommand.cpp CheckCommand.cpp AddCommand.cpp)
target_link_libraries(main gtest_main)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exe")
