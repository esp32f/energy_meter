set(COMPONENT_ADD_INCLUDEDIRS .)

set(COMPONENT_SRCS "cmd_nvs.c")

set(COMPONENT_REQUIRES esp32 nvs_flash)

register_component()
