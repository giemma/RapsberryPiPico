
add_library(PicoST7796 INTERFACE)


target_link_libraries(PicoST7796 INTERFACE pico_stdlib hardware_i2c hardware_spi hardware_pio )




target_sources(PicoST7796 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/PicoST7796.cpp
)


target_include_directories(PicoST7796 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

pico_generate_pio_header(PicoST7796 ${CMAKE_CURRENT_LIST_DIR}/pioST7796.pio)

# Pull in pico libraries that we need
