set(COMPONENT_ADD_INCLUDEDIRS .)

set(COMPONENT_SRCS
  "int_i2c.c"
  "int_nvs.c"
  "int_spi.c"
)

set(COMPONENT_REQUIRES esp32)

register_component()
