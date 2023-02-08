cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

add_executable(main)
target_sources(main
    PRIVATE
        main.cpp
)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)