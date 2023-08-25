cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(NoTrpDrm)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
  add_definitions(-DENABLE_LOGGING)
endif()

add_executable(NoTrpDrm
  main.c
)

target_link_libraries(NoTrpDrm
  taihen_stub
  SceLibKernel_stub
  SceIofilemgr_stub
  SceDisplay_stub
  SceCtrl_stub
  k
  gcc
  SceKernelModulemgr_stub
  ScePower_stub
)

set_target_properties(NoTrpDrm
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(NoTrpDrm.suprx NoTrpDrm
  CONFIG ${CMAKE_SOURCE_DIR}/NoTrpDrm.yml
)
