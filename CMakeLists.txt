cmake_minimum_required(VERSION 3.23...3.28)

project(project_multivariate_linear_regression LANGUAGES C)
include(CTest)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/dist")
endif()

add_subdirectory(${PROJECT_ROOT}/man/mkdata)
add_subdirectory(${PROJECT_ROOT}/man/train)
add_subdirectory(${PROJECT_ROOT}/src/mkdata)
add_subdirectory(${PROJECT_ROOT}/src/train)
add_subdirectory(${PROJECT_ROOT}/third_party/libboxmuller)
add_subdirectory(${PROJECT_ROOT}/third_party/libkwargs)
add_subdirectory(${PROJECT_ROOT}/third_party/libmatrix)
