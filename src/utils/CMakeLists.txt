set(UTILS_SRC db.c serialization.c format.c)
include_directories ("/usr/local/include")
add_library(utils SHARED ${UTILS_SRC})

target_link_libraries(utils lmdb)
