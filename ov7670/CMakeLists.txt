if (TARGET tinyusb_device)

        add_executable(ov7670 ov7670.cpp)


        # Pull in our pico_stdlib which pulls in commonly used features
        target_link_libraries(ov7670 pico_stdlib hardware_i2c hardware_spi )

        pico_enable_stdio_usb(ov7670 1)
        pico_enable_stdio_uart(ov7670 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(ov7670)

        # add url via pico_set_program_url
        example_auto_set_url(ov7670)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()