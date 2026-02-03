idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES esp-dsp driver esp_adc esp_timer esp_tinyusb led_strip wave_gen
)
