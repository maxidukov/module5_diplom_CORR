cmake_minimum_required(VERSION 3.5)

project(module5_diplom_CORR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(module5_diplom_CORR main.cpp)

include(GNUInstallDirs)
install(TARGETS module5_diplom_CORR
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
