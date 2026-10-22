cmake_minimum_required(VERSION 3.25)
project(stbimage_wrapper LANGUAGES C)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE .)
add_library(stb_image::stb_image ALIAS stb_image)