if (TARGET tinyusb_device)

        add_executable(simpleST7796 simpleST7796.c)


        # Pull in our pico_stdlib which pulls in commonly used features
        target_link_libraries(simpleST7796 pico_stdlib hardware_i2c hardware_spi)

        pico_enable_stdio_usb(simpleST7796 1)
        pico_enable_stdio_uart(simpleST7796 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(simpleST7796)

        # add url via pico_set_program_url
        example_auto_set_url(simpleST7796)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()