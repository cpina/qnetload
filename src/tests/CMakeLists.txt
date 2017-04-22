set(CMAKE_AUTOMOC ON)
find_package(Qt5Test REQUIRED)
add_executable(testnetworkinformation testnetworkinformation.cpp)
add_test(testnetworkinformation testnetworkinformation)

target_link_libraries(testnetworkinformation Qt5::Test)
