set(COMPONENT_ADD_INCLUDEDIRS .)

set(COMPONENT_SRCS "cmd_system.c")

set(COMPONENT_REQUIRES esp32 spi_flash)

register_component()
