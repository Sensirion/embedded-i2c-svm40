/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * I2C-Generator: 0.2.0
 * Yaml Version: 0.3.0
 * Template Version: 0.7.0
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "i2c_mux.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c_hal.h"
#include "sensirion_test_setup.h"
#include "svm40_i2c.h"
#include <inttypes.h>
#include <stdio.h>

#define MAX_VOC_INDEX 5000
#define MIN_VOC_INDEX 0
#define MAX_VOC_RAW 65535
#define MAX_RH_RAW (119 * 100)
#define MIN_RH_RAW (-6 * 100)
#define MAX_T (130 * 200)
#define MIN_T (-45 * 200)

TEST_GROUP (SVM40_Tests) {
    void setup() {
        int16_t error;
        sensirion_i2c_hal_init();

        // Select MUX 2 channel 7 (SVM40) on ch-sta-p-rp0002
        error = sensirion_i2c_mux_set_single_channel(0x72, 7);
        CHECK_EQUAL_ZERO_TEXT(error, "sensirion_i2c_mux_set_single_channel");
    }

    void teardown() {
        int16_t error;
        svm40_stop_measurement();
        error = svm40_device_reset();
        CHECK_EQUAL_ZERO_TEXT(error, "svm40_device_reset");
        sensirion_i2c_hal_free();
    }
};

TEST (SVM40_Tests, SVM40_Test_start_and_stop_measurement) {
    int16_t error;

    error = svm40_start_continuous_measurement();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_start_continuous_measurement");

    error = svm40_stop_measurement();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_stop_measurement");
}

TEST (SVM40_Tests, SVM40_Test_read_measured_values_as_integers) {
    int16_t error;
    int16_t voc_index;
    int16_t humidity;
    int16_t temperature;

    error = svm40_start_continuous_measurement();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_start_continuous_measurement");

    error = svm40_read_measured_values_as_integers(&voc_index, &humidity,
                                                   &temperature);
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_read_measured_values_as_integers");
    printf("Voc index: %i\n", voc_index);
    printf("Humidity: %i\n", humidity);
    printf("Temperature: %i\n", temperature);

    CHECK_TRUE_TEXT(voc_index >= MIN_VOC_INDEX && voc_index <= MAX_VOC_INDEX,
                    "svm40_read_measured_values_as_integers");
    CHECK_TRUE_TEXT(humidity >= MIN_RH_RAW && humidity <= MAX_RH_RAW,
                    "svm40_read_measured_values_as_integers");
    CHECK_TRUE_TEXT(temperature >= MIN_T && temperature <= MAX_T,
                    "svm40_read_measured_values_as_integers");

    error = svm40_stop_measurement();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_stop_measurement");
}

TEST (SVM40_Tests,
      SVM40_Test_read_measured_values_as_integers_with_raw_parameters) {
    int16_t error;
    int16_t voc_index;
    int16_t humidity;
    int16_t temperature;
    uint16_t raw_voc_ticks;
    int16_t raw_humidity;
    int16_t raw_temperature;

    error = svm40_start_continuous_measurement();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_start_continuous_measurement");

    error = svm40_read_measured_values_as_integers_with_raw_parameters(
        &voc_index, &humidity, &temperature, &raw_voc_ticks, &raw_humidity,
        &raw_temperature);
    CHECK_EQUAL_ZERO_TEXT(
        error, "svm40_read_measured_values_as_integers_with_raw_parameters");
    printf("Voc index: %i\n", voc_index);
    printf("Humidity: %i\n", humidity);
    printf("Temperature: %i\n", temperature);
    printf("Raw voc ticks: %u\n", raw_voc_ticks);
    printf("Raw humidity: %i\n", raw_humidity);
    printf("Raw temperature: %i\n", raw_temperature);

    CHECK_TRUE_TEXT(voc_index >= MIN_VOC_INDEX && voc_index <= MAX_VOC_INDEX,
                    "svm40_read_measured_values_as_integers");
    CHECK_TRUE_TEXT(humidity >= MIN_RH_RAW && humidity <= MAX_RH_RAW,
                    "svm40_read_measured_values_as_integers_with_raw_params");
    CHECK_TRUE_TEXT(temperature >= MIN_T && temperature <= MAX_T,
                    "svm40_read_measured_values_as_integers_with_raw_params");
    CHECK_TRUE_TEXT(raw_humidity >= MIN_RH_RAW && raw_humidity <= MAX_RH_RAW,
                    "svm40_read_measured_values_as_integers_with_raw_params");
    CHECK_TRUE_TEXT(raw_temperature >= MIN_T && raw_temperature <= MAX_T,
                    "svm40_read_measured_values_as_integers_with_raw_params");
    CHECK_TRUE_TEXT(raw_voc_ticks <= MAX_VOC_RAW,
                    "svm40_read_measured_values_as_integers_with_raw_params");

    error = svm40_stop_measurement();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_stop_measurement");
}

TEST (SVM40_Tests, SVM40_Test_temperature_offset_for_rht_measurements) {
    int16_t error;
    int16_t expected_offset = 420;
    int16_t t_offset;

    error = svm40_set_temperature_offset_for_rht_measurements(expected_offset);
    CHECK_EQUAL_ZERO_TEXT(error,
                          "svm40_set_temperature_offset_for_rht_measurements");

    error = svm40_get_temperature_offset_for_rht_measurements(&t_offset);
    CHECK_EQUAL_ZERO_TEXT(error,
                          "svm40_get_temperature_offset_for_rht_measurements");
    printf("T offset: %i\n", t_offset);

    CHECK_TRUE_TEXT(t_offset == expected_offset,
                    "svm40_get_temperature_offset_for_rht_measurements");
}

TEST (SVM40_Tests, SVM40_Test_set_voc_algorithm_tuning_parameters) {
    int16_t error;
    int16_t voc_index_offset = 100;
    int16_t learning_time_hours = 12;
    int16_t gating_max_duration_minutes = 180;
    int16_t std_initial = 50;
    error = svm40_set_voc_algorithm_tuning_parameters(
        voc_index_offset, learning_time_hours, gating_max_duration_minutes,
        std_initial);
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_set_voc_algorithm_tuning_parameters");
}

TEST (SVM40_Tests, SVM40_Test_get_voc_algorithm_tuning_parameters) {
    int16_t error;
    int16_t voc_index_offset;
    int16_t learning_time_hours;
    int16_t gating_max_duration_minutes;
    int16_t std_initial;
    error = svm40_get_voc_algorithm_tuning_parameters(
        &voc_index_offset, &learning_time_hours, &gating_max_duration_minutes,
        &std_initial);
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_get_voc_algorithm_tuning_parameters");
    printf("Voc index offset: %i\n", voc_index_offset);
    printf("Learning time hours: %i\n", learning_time_hours);
    printf("Gating max duration minutes: %i\n", gating_max_duration_minutes);
    printf("Std initial: %i\n", std_initial);
}

TEST (SVM40_Tests, SVM40_Test_store_nv_data) {
    int16_t error;
    error = svm40_store_nv_data();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_store_nv_data");
}

TEST (SVM40_Tests, SVM40_Test_set_voc_algorithm_state) {
    int16_t error;
    uint8_t state[8];
    uint8_t state_size = 8;
    error = svm40_set_voc_algorithm_state(&state[0], state_size);
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_set_voc_algorithm_state");
}

TEST (SVM40_Tests, SVM40_Test_get_voc_algorithm_state) {
    int16_t error;
    uint8_t state[8];
    uint8_t state_size = 8;
    error = svm40_get_voc_algorithm_state(&state[0], state_size);
    CHECK_EQUAL_TEXT(0, error, "svm40_get_voc_algorithm_state");
}

TEST (SVM40_Tests, SVM40_Test_get_serial_number) {
    int16_t error;
    uint8_t serial_number[32];
    uint8_t serial_number_size = 32;
    error = svm40_get_serial_number(&serial_number[0], serial_number_size);
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_get_serial_number");
    printf("serial_number: %s\n", serial_number);
}

TEST (SVM40_Tests, SVM40_Test_get_version) {
    int16_t error;
    uint8_t firmware_major;
    uint8_t firmware_minor;
    bool firmware_debug;
    uint8_t hardware_major;
    uint8_t hardware_minor;
    uint8_t protocol_major;
    uint8_t protocol_minor;
    error = svm40_get_version(&firmware_major, &firmware_minor, &firmware_debug,
                              &hardware_major, &hardware_minor, &protocol_major,
                              &protocol_minor);
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_get_version");
    printf("firmware_major: %i\n", firmware_major);
    printf("firmware_minor: %i\n", firmware_minor);
    printf("firmware_debug: %i\n", firmware_debug);
    printf("hardware_major: %i\n", hardware_major);
    printf("hardware_minor: %i\n", hardware_minor);
    printf("protocol_major: %i\n", protocol_major);
    printf("protocol_minor: %i\n", protocol_minor);
}

TEST (SVM40_Tests, SVM40_Test_device_reset) {
    int16_t error;
    error = svm40_device_reset();
    CHECK_EQUAL_ZERO_TEXT(error, "svm40_device_reset");
}
