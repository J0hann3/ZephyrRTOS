cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT C:\\Users\\JohanneVIGNY\\ZephyrRTOS)  # Define path to board

find_package(Zephyr)    # pull zephyr and create a tagret named 'app'
project(my-app)     # Must be call after the previous line to avoid conflict
                    # Define my CMake application project

target_sources(app PRIVATE src/main.c
                            src/lum_sensor.c
                            src/temp_hum_sensor.c
                            src/sd_card.c
                            src/measures_logger.c)  # Add my-app to the app(zephyr) project

target_include_directories(app PRIVATE include src)