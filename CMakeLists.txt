cmake_minimum_required(VERSION 3.0)
get_filename_component(PROJECT_NAME "${CMAKE_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} NONE)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type...")
set(SXC_CROSS_COMPILE ON)

find_package(SXC REQUIRED)
include_directories(${SXC_INCLUDE_DIRS})
include(SXCMacros)

find_package(libspine REQUIRED)
include_directories(${libspine_INCLUDE_DIRS})
add_to_py_exec_path(libspine_PY_DIRS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DISABLE_SOFTWARE_DIVIDE__")

build_executable(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  empty_var
  PY_FILES
)
sxc_firmware_obj_to_hex(
  ${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/main.gen empty_var empty_var hex_target
)
add_custom_target(${PROJECT_NAME}-hex-final ALL
  DEPENDS ${hex_target}
)
