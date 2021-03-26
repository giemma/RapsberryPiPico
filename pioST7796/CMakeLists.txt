if (TARGET tinyusb_device)

    add_executable(pioST7796 pioST7796.cpp)

    
    
    # Pull in our pico_stdlib which pulls in commonly used features
    target_link_libraries(pioST7796 pico_stdlib hardware_i2c hardware_spi hardware_pio PicoST7796)

    pico_enable_stdio_usb(pioST7796 1)
    pico_enable_stdio_uart(pioST7796 0)

    # create map/bin/hex file etc.
    pico_add_extra_outputs(pioST7796)

    # add url via pico_set_program_url
    example_auto_set_url(pioST7796)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()