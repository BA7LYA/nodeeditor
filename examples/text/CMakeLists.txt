file(GLOB_RECURSE CPPS  ./*.cxx )
file(GLOB_RECURSE HPPS  ./*.hxx )

add_executable(text ${CPPS} ${HPPS})

target_link_libraries(text QtNodes)
