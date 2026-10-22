cmake_minimum_required(VERSION 3.25)
project(simdjson LANGUAGES CXX)

add_library(simdjson STATIC simdjson.cpp)
target_include_directories(simdjson PUBLIC .)
add_library(simdjson::simdjson ALIAS simdjson)