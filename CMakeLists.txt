cmake_minimum_required(VERSION 3.0.2)
enable_testing()

project(qnetload)

add_subdirectory(tests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set(QT5_LIBRARIES Qt5::Widgets)

set(TARGET ${CMAKE_PROJECT_NAME})

set(SOURCES
  main.cpp
  mainwindow.cpp
  networkinformationreader.cpp
  informationstorage.cpp
  formatnumber.cpp
  plot.cpp
)

set(UI_SOURCES
  mainwindow.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_executable(${TARGET} ${SOURCES} ${UI_GENERATED_HEADERS})

target_link_libraries(${TARGET} ${QT5_LIBRARIES})

install(TARGETS qnetload DESTINATION bin)
