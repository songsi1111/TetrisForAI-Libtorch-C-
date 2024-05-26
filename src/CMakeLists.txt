cmake_minimum_required(VERSION 3.26)
#设置C++标准20
set(CMAKE_CXX_STANDARD 17)

project(tetris)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#查找Torch库
set(Torch_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libtorch/share/cmake/Torch")
find_package(Torch REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
find_package(Qt5 COMPONENTS Widgets REQUIRED)
include_directories(/usr/include/x86_64-linux-gnu/qt5)
include_directories(/usr/include/x86_64-linux-gnu/qt5/QtWidgets)


add_executable(train train.cpp tetris.cpp qt.cpp qtDraw.hpp block.qrc)
target_link_libraries(train PRIVATE 
 Qt5::Widgets
 ${TORCH_LIBRARIES}
)
add_executable(test train.cpp test.cpp tetris.cpp qt.cpp qtDraw.hpp block.qrc)
target_link_libraries(test PRIVATE 
 Qt5::Widgets
 ${TORCH_LIBRARIES}
)
