if (TARGET tinyusb_device)

        add_executable(StepperLibraryExample StepperLibraryExample.cpp)


        # Pull in our pico_stdlib which pulls in commonly used features
        target_link_libraries(StepperLibraryExample pico_stdlib hardware_spi pico_multicore Stepper )

        pico_enable_stdio_usb(StepperLibraryExample 1)
        pico_enable_stdio_uart(StepperLibraryExample 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(StepperLibraryExample)

        # add url via pico_set_program_url
        example_auto_set_url(StepperLibraryExample)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()