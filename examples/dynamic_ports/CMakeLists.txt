file(GLOB_RECURSE CPPS  ./*.cxx )

add_executable(dynamic_ports ${CPPS})

target_link_libraries(dynamic_ports QtNodes)

# TODO(ba7lya)
# add_custom_command(
#     TARGET QtNodes
#     POST_BUILD
#     COMMAND ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_DEFAULT_TRIPLET}/tools/Qt6/bin/
# )
