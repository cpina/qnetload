cmake_minimum_required(VERSION 3.25.1)
include(GNUInstallDirs)
enable_testing()

project(qnetload)

add_subdirectory(tests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6Widgets REQUIRED)
find_package(Qt6Network REQUIRED)

set(QT6_LIBRARIES Qt6::Widgets Qt6::Network)

set(TARGET ${CMAKE_PROJECT_NAME})

set(SOURCES
  main.cpp
  mainwindow.cpp
  networkinformationreader.cpp
  informationstorage.cpp
  formatnumber.cpp
  plot.cpp
  clickablelabel.cpp
  utils.cpp
  resources.qrc
)

set(UI_SOURCES
  mainwindow.ui
)

set(CMAKE_AUTORCC ON)

qt6_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_executable(${TARGET} ${SOURCES} ${UI_GENERATED_HEADERS})

target_link_libraries(${TARGET} ${QT6_LIBRARIES})

message(WARNING "The value of TARGET: ${TARGET}")
message(WARNING "The value of CMAKE_INSTALL_MANDIR: ${CMAKE_INSTALL_MANDIR}")

install(TARGETS qnetload DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES manual/qnetload.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
