PROJECT_NAME := energy_meter

include $(IDF_PATH)/make/project.mk


# Create a SPIFFS image from the contents of the 'spiffs_image' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'make flash'. 
$(eval $(call spiffs_create_partition_image,storage,spiffs,FLASH_IN_PROJECT))
