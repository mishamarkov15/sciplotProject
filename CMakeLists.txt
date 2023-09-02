cmake_minimum_required(VERSION 3.23)

# Устанавливаем название проекта и язык программирования
project(sciplot_graph CXX)

add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_20)
# Добавляем путь к модулю FindSciPlot.cmake в переменную CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(sciplot)

# Добавляем директорию с исходным кодом в переменную SOURCES
set(SOURCES main.cpp)
set(TESTING test.cpp)

# Добавляем исполняемый файл с названием sciplot_graph и исходным кодом из переменной SOURCES
add_executable(sciplot_graph ${SOURCES})

target_link_libraries(sciplot_graph sciplot::sciplot)

add_subdirectory(sciplot)