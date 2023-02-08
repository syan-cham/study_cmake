cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

include(FetchContent)

FetchContent_Declare(json
    URL         ${CMAKE_CURRENT_LIST_DIR}/tar/json-3.11.2.tar.gz
    URL_HASH    SHA256=d69f9deb6a75e2580465c6c4c5111b89c4dc2fa94e3a85fcd2ffcd9a143d9273
)
# target_link_libraries(<your-target> PRIVATE nlohmann_json::nlohmann_json)

FetchContent_MakeAvailable(json)