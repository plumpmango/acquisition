cmake_minimum_required(VERSION 2.8)
project( acq )
find_package( OpenCV REQUIRED )

find_package(GTK2 2.6 REQUIRED gtk)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

include(FindPkgConfig)
pkg_search_module(ARAVIS REQUIRED aravis-0.6 aravis-0.4 aravis-0.2)

include_directories(

  ${ARAVIS_INCLUDE_DIRS}

)


include_directories(${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

# need this to include the auto generated ui_mainwindow.h
# file from the qt5_wrap_ui call below.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( SOURCES
 src/Acquisition/arvdriver.cpp
 src/Acquisition/acquisition.cpp
 src/Manage/manage.cpp
)

set ( MOC_HEADERS
 src/Acquisition/arvdriver.h
 src/Acquisition/acquisition.h
 src/Manage/manage.h
)

set ( UIS
 src/Manage/manage.ui
)

#set ( RESOURCES
 #main.qrc
#)


# using Qt meta-system (precompiler)
#QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
QT5_WRAP_UI( UI_HEADERS ${UIS} )
QT5_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

add_executable( acq  src/main.cpp ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS})

target_link_libraries(acq
  ${ARAVIS_LDFLAGS}
  ${OpenCV_LIBS}
  Qt5::Widgets
)

add_executable( acqMulti  src/mainMulti.cpp ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS})

target_link_libraries(acqMulti
  ${ARAVIS_LDFLAGS}
  ${OpenCV_LIBS}
  Qt5::Widgets
)
