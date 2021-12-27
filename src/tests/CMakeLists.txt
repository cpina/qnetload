set(CMAKE_AUTOMOC ON)
find_package(Qt5Test REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(testnetworkinformationreader testnetworkinformationreader.cpp ../networkinformationreader.cpp ../utils.cpp)
add_executable(testformatnumber testformatnumber.cpp ../formatnumber.cpp)
add_executable(testinformationstorage testinformationstorage.cpp ../informationstorage.cpp ../utils.cpp)

add_test(testnetworkinformationreader testnetworkinformationreader)
add_test(testformatnumber testformatnumber)
add_test(testinformationstorage testinformationstorage)

target_link_libraries(testnetworkinformationreader Qt5::Test Qt5::Network)
target_link_libraries(testformatnumber Qt5::Test)
target_link_libraries(testinformationstorage Qt5::Test)

# Warning: files are not copied again if modified
file(COPY proc_net_dev_files/example01.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/proc_net_dev_files)
