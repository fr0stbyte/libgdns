include_directories("../include" "utils")
set(OVERLOAD_SRC overload.c)
set(C_ARES_OVERLOAD_SRC c_ares_overload.c)
add_library(gdns SHARED ${OVERLOAD_SRC})
add_library(gdns_c_ares SHARED ${C_ARES_OVERLOAD_SRC})

target_link_libraries(gdns dl)
target_link_libraries(gdns utils)
target_link_libraries(gdns_c_ares dl)

add_subdirectory(utils)
