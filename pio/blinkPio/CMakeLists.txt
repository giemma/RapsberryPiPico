if (TARGET tinyusb_device)

        add_executable(blinkPio blinkPio.cpp)
        
        target_link_libraries(blinkPio pico_stdlib hardware_i2c hardware_spi hardware_pio)

        pico_generate_pio_header(blinkPio ${CMAKE_CURRENT_LIST_DIR}/blinkPio.pio)

        # Pull in our pico_stdlib which pulls in commonly used features
        
        pico_enable_stdio_usb(blinkPio 1)
        pico_enable_stdio_uart(blinkPio 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(blinkPio)

        # add url via pico_set_program_url
        example_auto_set_url(blinkPio)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()