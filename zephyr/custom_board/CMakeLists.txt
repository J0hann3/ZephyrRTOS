cmake_minimum_required(VERSION 3.20.0)

# set(BOARD nxg_court)  # Define board to use
# list(APPEND BOARD_ROOT C:\\Users\\JohanneVIGNY\\ZephyrRTOS\\zephyr)  # Define path to board

find_package(Zephyr)    # pull zephyr and create a tagret named 'app'
project(my-app)     # Must be call after the previous line to avoid conflict
                    # Define my CMake application project

target_sources(app PRIVATE src/main.c)  # Add my-app to the app(zephyr) project
