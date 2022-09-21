add_executable(person_sensor_i2c
        person_sensor_i2c.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(person_sensor_i2c pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(person_sensor_i2c)

pico_enable_stdio_usb(person_sensor_i2c 1)
