if (TARGET tinyusb_device)

        add_executable(template template.cpp)


        # Pull in our pico_stdlib which pulls in commonly used features
        target_link_libraries(template pico_stdlib hardware_i2c hardware_spi )

        pico_enable_stdio_usb(template 1)
        pico_enable_stdio_uart(template 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(template)

        # add url via pico_set_program_url
        example_auto_set_url(template)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()