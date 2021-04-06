project(gtk_tutorial LANGUAGES C)

cmake_minimum_required(VERSION 2.8.12)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

message(STATUS "Include: ${GTK3_INCLUDE_DIRS}\n LIB_DIR: ${GTK3_LIBRARY_DIRS} \n CFLAGS_OTHERS: ${GTK3_CFLAGS_OTHER} \n LIBS: ${GTK3_LIBRARIES}")

add_subdirectory(db)



set(DB db/student-db.c)

add_executable(student-list student-list.c ${DB})
target_link_libraries(student-list sqlite3 pthread dl)
target_link_libraries(student-list ${GTK3_LIBRARIES})
