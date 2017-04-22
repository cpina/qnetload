set(CMAKE_AUTOMOC ON)
find_package(Qt5Test REQUIRED)
add_executable(testnetworkinformation testnetworkinformation.cpp ../networkinformationreader.cpp)
add_test(testnetworkinformation testnetworkinformation)

target_link_libraries(testnetworkinformation Qt5::Test)

# Files are not copied again if modified
file(COPY proc_net_dev_files/example01.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/proc_net_dev_files)
