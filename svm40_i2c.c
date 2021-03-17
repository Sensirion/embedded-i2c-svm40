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

#include "svm40_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c_hal.h"

#define SVM40_I2C_ADDRESS 0x6A

int16_t svm40_start_continuous_measurement(void) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x10);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t svm40_stop_measurement(void) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x104);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(50000);
    return NO_ERROR;
}

int16_t svm40_read_measured_values_as_integers(int16_t* voc_index,
                                               int16_t* humidity,
                                               int16_t* temperature) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3A6);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        return error;
    }
    *voc_index = sensirion_common_bytes_to_int16_t(&buffer[0]);
    *humidity = sensirion_common_bytes_to_int16_t(&buffer[2]);
    *temperature = sensirion_common_bytes_to_int16_t(&buffer[4]);
    return NO_ERROR;
}

int16_t svm40_read_measured_values_as_integers_with_raw_parameters(
    int16_t* voc_index, int16_t* humidity, int16_t* temperature,
    uint16_t* raw_voc_ticks, int16_t* raw_humidity, int16_t* raw_temperature) {
    int16_t error;
    uint8_t buffer[18];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3B0);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 12);
    if (error) {
        return error;
    }
    *voc_index = sensirion_common_bytes_to_int16_t(&buffer[0]);
    *humidity = sensirion_common_bytes_to_int16_t(&buffer[2]);
    *temperature = sensirion_common_bytes_to_int16_t(&buffer[4]);
    *raw_voc_ticks = sensirion_common_bytes_to_uint16_t(&buffer[6]);
    *raw_humidity = sensirion_common_bytes_to_int16_t(&buffer[8]);
    *raw_temperature = sensirion_common_bytes_to_int16_t(&buffer[10]);
    return NO_ERROR;
}

int16_t svm40_set_temperature_offset_for_rht_measurements(int16_t t_offset) {
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6014);

    offset = sensirion_i2c_add_int16_t_to_buffer(&buffer[0], offset, t_offset);

    return sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
}

int16_t svm40_get_temperature_offset_for_rht_measurements(int16_t* t_offset) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6014);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *t_offset = sensirion_common_bytes_to_int16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t svm40_set_voc_algorithm_tuning_parameters(
    int16_t voc_index_offset, int16_t learning_time_hours,
    int16_t gating_max_duration_minutes, int16_t std_initial) {
    uint8_t buffer[14];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6083);

    offset = sensirion_i2c_add_int16_t_to_buffer(&buffer[0], offset,
                                                 voc_index_offset);
    offset = sensirion_i2c_add_int16_t_to_buffer(&buffer[0], offset,
                                                 learning_time_hours);
    offset = sensirion_i2c_add_int16_t_to_buffer(&buffer[0], offset,
                                                 gating_max_duration_minutes);
    offset =
        sensirion_i2c_add_int16_t_to_buffer(&buffer[0], offset, std_initial);

    return sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
}

int16_t svm40_get_voc_algorithm_tuning_parameters(
    int16_t* voc_index_offset, int16_t* learning_time_hours,
    int16_t* gating_max_duration_minutes, int16_t* std_initial) {
    int16_t error;
    uint8_t buffer[12];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6083);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 8);
    if (error) {
        return error;
    }
    *voc_index_offset = sensirion_common_bytes_to_int16_t(&buffer[0]);
    *learning_time_hours = sensirion_common_bytes_to_int16_t(&buffer[2]);
    *gating_max_duration_minutes =
        sensirion_common_bytes_to_int16_t(&buffer[4]);
    *std_initial = sensirion_common_bytes_to_int16_t(&buffer[6]);
    return NO_ERROR;
}

int16_t svm40_store_nv_data(void) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6002);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(500000);
    return NO_ERROR;
}

int16_t svm40_set_voc_algorithm_state(const uint8_t* state,
                                      uint8_t state_size) {
    uint8_t buffer[14];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6181);

    offset = sensirion_i2c_add_bytes_to_buffer(&buffer[0], offset, state,
                                               state_size);

    return sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
}

int16_t svm40_get_voc_algorithm_state(uint8_t* state, uint8_t state_size) {
    int16_t error;
    uint8_t buffer[12];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x6181);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 8);
    if (error) {
        return error;
    }
    sensirion_common_copy_bytes(&buffer[0], state, state_size);
    return NO_ERROR;
}

int16_t svm40_get_version(uint8_t* firmware_major, uint8_t* firmware_minor,
                          bool* firmware_debug, uint8_t* hardware_major,
                          uint8_t* hardware_minor, uint8_t* protocol_major,
                          uint8_t* protocol_minor) {
    int16_t error;
    uint8_t buffer[12];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xD100);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 8);
    if (error) {
        return error;
    }
    *firmware_major = buffer[0];
    *firmware_minor = buffer[1];
    *firmware_debug = buffer[2];
    *hardware_major = buffer[3];
    *hardware_minor = buffer[4];
    *protocol_major = buffer[5];
    *protocol_minor = buffer[6];
    return NO_ERROR;
}

int16_t svm40_get_serial_number(unsigned char* serial_number,
                                uint8_t serial_number_size) {
    int16_t error;
    uint8_t buffer[39];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xD033);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SVM40_I2C_ADDRESS, &buffer[0], 26);
    if (error) {
        return error;
    }
    sensirion_common_copy_bytes(&buffer[0], serial_number, serial_number_size);
    return NO_ERROR;
}

int16_t svm40_device_reset(void) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xD304);

    error = sensirion_i2c_write_data(SVM40_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(100000);
    return NO_ERROR;
}
