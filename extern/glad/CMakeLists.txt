cmake_minimum_required(VERSION 3.25)
project(glad_wrapper LANGUAGES C)

add_library(glad STATIC glad.c)
target_include_directories(glad PUBLIC .)
add_library(glad::glad ALIAS glad)