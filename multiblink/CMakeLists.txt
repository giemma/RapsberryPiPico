if (TARGET tinyusb_device)

        add_executable(multiblink multiblink.c)
        
        # Pull in our pico_stdlib which pulls in commonly used features
        target_link_libraries(multiblink pico_stdlib hardware_i2c hardware_spi hardware_pio)

        pico_enable_stdio_usb(multiblink 1)
        pico_enable_stdio_uart(multiblink 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(multiblink)

        # add url via pico_set_program_url
        example_auto_set_url(multiblink)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()