cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT C:\\Users\\JohanneVIGNY\\ZephyrRTOS\\zephyr)  # Define path to board

set(BUILD_MODE "release" CACHE STRING "Build mode: debug or release")   # set build mode to release by default

if(BUILD_MODE STREQUAL "debug")
  set(CONF_FILE "prj.conf prj_debug.conf")                      # Enable console log via uart on kconfig

  if(BOARD STREQUAL nxg_court)
    set(DTC_OVERLAY_FILE "boards/nxg_court_debug.overlay")        # Enable uart in devicetree
  endif()
elseif(BUILD_MODE STREQUAL "release")
  set(CONF_FILE "prj.conf prj_release.conf")                    # Disable default console log via uart
else()
  message(FATAL_ERROR "Invalid BUILD_MODE: ${BUILD_MODE}. Use 'debug' or 'release'.")   # Error if build mode unknown
endif()

set(CONF_FILE ${CONF_FILE} CACHE INTERNAL "Merged config file list")    # merge default prj.conf with the one to enable or disable uart

find_package(Zephyr)    # pull zephyr and create a tagret named 'app'
project(my-app)     # Must be call after the previous line to avoid conflict
                    # Define my CMake application project

if(BUILD_MODE STREQUAL "debug")                             # If debug mode enable add macro Debug
    target_compile_definitions(app PRIVATE DEBUG=1)
endif()

target_sources(app PRIVATE src/main.c
                            src/lum_sensor.c
                            src/temp_hum_sensor.c
                            src/sd_card.c
                            src/lora.c
                            src/spi.c
                            ../../common/measures_logger/measures_logger.c) 

target_include_directories(app PRIVATE src ../../common/measures_logger/)