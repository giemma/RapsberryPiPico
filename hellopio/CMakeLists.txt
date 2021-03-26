if (TARGET tinyusb_device)

        add_executable(hellopio hellopio.c)

        pico_generate_pio_header(hellopio ${CMAKE_CURRENT_LIST_DIR}/hello.pio)
        
        # Pull in our pico_stdlib which pulls in commonly used features
        target_link_libraries(hellopio pico_stdlib hardware_i2c hardware_spi hardware_pio)

        pico_enable_stdio_usb(hellopio 1)
        pico_enable_stdio_uart(hellopio 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(hellopio)

        # add url via pico_set_program_url
        example_auto_set_url(hellopio)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()