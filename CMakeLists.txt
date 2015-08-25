project(tlab)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
 
set(TLab_SRCS
  main.cpp
)

kde4_add_executable(tlab plotgraph.cpp plotwindow.cpp ${TLab_SRCS})

FIND_LIBRARY(TLab_LIB
             NAMES pqxx
             PATHS /usr/lib64)
IF (TLab_LIB)
# http://stackoverflow.com/questions/5395578/cmake-target-link-libraries-not-linking-my-library
    target_link_libraries(tlab ${TLab_LIB})
ENDIF (TLab_LIB)

target_link_libraries(tlab ${KDE4_KDEUI_LIBS})

add_definitions(-fexceptions -std=c++11)

install(TARGETS tlab RUNTIME DESTINATION bin)

