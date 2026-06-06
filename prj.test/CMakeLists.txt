add_executable(bitsetd_test bitsetd_test.cpp)

target_include_directories(bitsetd_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../prj.thirdparty)
target_include_directories(bitsetd_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../prj.labs)
target_link_libraries(bitsetd_test bitsetd)
