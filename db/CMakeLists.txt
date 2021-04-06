project(sqlite-lib
        LANGUAGES C)
cmake_minimum_required(VERSION 2.6)
add_library(sqlite3 STATIC sqlite/sqlite3.c)

include_directories(sqlite)

# foreach (example insert select )
#   target_link_libraries(${example} sqlite3 pthread dl)
# endforeach(example)
