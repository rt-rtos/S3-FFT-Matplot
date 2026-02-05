#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Waveform types for generation
 */
typedef enum {
    WAVE_TYPE_SINE = 0,
    WAVE_TYPE_SQUARE,
    WAVE_TYPE_TRIANGLE,
    WAVE_TYPE_SAWTOOTH
} wave_type_t;

/**
 * @brief Configuration structure for Wave Generator
 */
typedef struct {
    int bck_io_num;      /*!< Bit Clock Pin (SCK) */
    int ws_io_num;       /*!< Word Select Pin (LRCK/WS) */
    int data_out_num;    /*!< Data Out Pin (DIN) */
    int mck_io_num;      /*!< Master Clock Pin (MCK), set to -1 if unused */
    uint32_t sample_rate;/*!< Sample rate in Hz (e.g., 44100 or 48000) */
} wave_gen_config_t;

/**
 * @brief Initialize the Wave Generator component
 * 
 * @param config Pointer to configuration structure
 * @return esp_err_t ESP_OK on success
 */
esp_err_t wave_gen_init(const wave_gen_config_t *config);

/**
 * @brief Start the wave generation task
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t wave_gen_start(void);

/**
 * @brief Stop the wave generation task
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t wave_gen_stop(void);

/**
 * @brief Set the frequency of the generated wave
 * 
 * @param freq_hz Frequency in Hertz
 */
void wave_gen_set_freq(float freq_hz);

/**
 * @brief Set the waveform type
 * 
 * @param type One of wave_type_t
 */
void wave_gen_set_type(wave_type_t type);

/**
 * @brief Set the output volume/amplitude
 * 
 * @param volume 0.0 to 1.0 (clamped)
 */
void wave_gen_set_volume(float volume);

/**
 * @brief Start the SDM sine generator (uses CONFIG_SDM_* settings)
 *
 * @return esp_err_t ESP_OK on success
 */
esp_err_t wave_gen_sdm_start(void);

/**
 * @brief Stop the SDM sine generator
 *
 * @return esp_err_t ESP_OK on success
 */
esp_err_t wave_gen_sdm_stop(void);

#ifdef __cplusplus
}
#endif
