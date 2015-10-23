include_directories("../src/utils" "../include")
add_executable(test_c test.c)

add_executable(serialization serialization.c)
target_link_libraries(serialization utils)

add_executable(test_db test_db.c)
target_link_libraries(test_db utils)

file(GLOB TestFiles RELATIVE "${PROJECT_SOURCE_DIR}/tests" "*.js" "*.java")

foreach(TestFile ${TestFiles})
  execute_process(COMMAND cmake -E copy_if_different
  ${PROJECT_SOURCE_DIR}/tests/${TestFile} tests/${TestFile})
endforeach(TestFile)

add_custom_target(run_c
   COMMAND LD_PRELOAD=../src/libgdns.so ./test_c
)

add_custom_target(run_js
  COMMAND LD_PRELOAD=../src/libgdns.so node ./test.js
)

add_custom_target(run_java javac Test.java
  COMMAND LD_PRELOAD=../src/libgdns.so java Test
)
