if (TARGET tinyusb_device)

    add_executable(ST7796LibraryExample ST7796LibraryExample.cpp)

    
    
    # Pull in our pico_stdlib which pulls in commonly used features
    target_link_libraries(ST7796LibraryExample pico_stdlib hardware_i2c hardware_spi hardware_pio PicoST7796)

    pico_enable_stdio_usb(ST7796LibraryExample 1)
    pico_enable_stdio_uart(ST7796LibraryExample 0)

    # create map/bin/hex file etc.
    pico_add_extra_outputs(ST7796LibraryExample)

    # add url via pico_set_program_url
    example_auto_set_url(ST7796LibraryExample)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()