idf_component_register(SRCS "wave_gen.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_driver_i2s esp_driver_sdm esp_driver_gptimer driver)
