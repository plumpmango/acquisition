cmake_minimum_required(VERSION 2.8)
project( acq )
find_package( OpenCV REQUIRED )

find_package(GTK2 2.6 REQUIRED gtk)

find_package(Qt5Widgets REQUIRED)

include(FindPkgConfig)
pkg_search_module(ARAVIS REQUIRED aravis-0.6 aravis-0.4 aravis-0.2)

include_directories(

  ${ARAVIS_INCLUDE_DIRS}

)


include_directories(${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

set ( SOURCES
 src/main.cpp
 src/Acquisition/arvdriver.cpp
 src/acquisition.cpp
)

set ( MOC_HEADERS
 src/Acquisition/arvdriver.h
 src/acquisition.h
)

#set ( UIS
# mainwindow.ui
#)

#set ( RESOURCES
# main.qrc
#)


# using Qt meta-system (precompiler)
QT5_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
QT5_WRAP_UI( UI_HEADERS ${UIS} )
QT5_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

add_executable( acq ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS})

target_link_libraries(acq
  ${ARAVIS_LDFLAGS}
  ${OpenCV_LIBS}
  Qt5::Widgets
)
