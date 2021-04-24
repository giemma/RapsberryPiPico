
add_library(Stepper INTERFACE)


target_link_libraries(Stepper INTERFACE pico_stdlib hardware_spi)


target_sources(Stepper INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Stepper.cpp
)


target_include_directories(Stepper INTERFACE ${CMAKE_CURRENT_LIST_DIR})


