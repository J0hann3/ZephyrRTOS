cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT C:\\Users\\JohanneVIGNY\\ZephyrRTOS\\zephyr)  # Define path to board

set(BUILD_MODE "release" CACHE STRING "Build mode: debug or release")

if(BUILD_MODE STREQUAL "debug")
  set(CONF_FILE "prj.conf prj_debug.conf")
  set(DTC_OVERLAY_FILE "boards/nxg_court_debug.overlay")
elseif(BUILD_MODE STREQUAL "release")
  set(CONF_FILE "prj.conf prj_release.conf")
else()
  message(FATAL_ERROR "Invalid BUILD_MODE: ${BUILD_MODE}. Use 'debug' or 'release'.")
endif()

set(CONF_FILE ${CONF_FILE} CACHE INTERNAL "Merged config file list")

find_package(Zephyr)    # pull zephyr and create a tagret named 'app'
project(my-app)     # Must be call after the previous line to avoid conflict
                    # Define my CMake application project

target_sources(app PRIVATE src/main.c
                            src/lum_sensor.c
                            src/temp_hum_sensor.c
                            src/sd_card.c
                            ../../common/measures_logger/measures_logger.c) 

target_include_directories(app PRIVATE src ../../common/measures_logger/)