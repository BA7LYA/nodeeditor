file(GLOB_RECURSE CPPS  ./*.cxx )

add_executable(vertical_layout ${CPPS})

target_link_libraries(vertical_layout QtNodes)
