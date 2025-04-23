cmake_minimum_required(VERSION 3.20.0)

set(BOARD samd21_xpro)  # Define board to use

find_package(Zephyr)    # pull zephyr and create a tagret named 'app'
project(my-app)     # Must be call after the previous line to avoid conflict
                    # Define my CMake application project

target_sources(app PRIVATE src/main.c)  # Add my-app to the app(zephyr) project
