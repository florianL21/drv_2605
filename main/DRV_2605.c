
#include "DRV_2605.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_check.h"
#include "math.h"

const char* TAG = "DRV_2605";

esp_err_t i2c_write_reg(uint8_t ic2_port, uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    ESP_RETURN_ON_ERROR(i2c_master_write_to_device(ic2_port, DRV_2650_WRITE_ADDRESS, buffer, 2, DRV_2650_TIMEOUT), TAG, "Could not write value %d to register %d", value, reg);
    return ESP_OK;
}

esp_err_t i2c_write_reg_seq(uint8_t ic2_port, uint8_t reg, uint8_t* value, size_t length) {
    uint8_t buffer[1] = {reg};
    ESP_RETURN_ON_ERROR(i2c_master_write_to_device(ic2_port, DRV_2650_WRITE_ADDRESS, buffer, 1, DRV_2650_TIMEOUT), TAG, "Could not initiate write to register %d", reg);
    ESP_RETURN_ON_ERROR(i2c_master_write_to_device(ic2_port, DRV_2650_WRITE_ADDRESS, value, length, DRV_2650_TIMEOUT), TAG, "Could not write data sequence to register %d", reg);
    return ESP_OK;
}

esp_err_t i2c_read_reg(uint8_t ic2_port, uint8_t reg, uint8_t* data) {
    uint8_t buffer[1] = {reg};
    ESP_RETURN_ON_ERROR(i2c_master_write_read_device(ic2_port, DRV_2650_WRITE_ADDRESS, buffer, 1, buffer, 1, DRV_2650_TIMEOUT), TAG, "Could not read data from register %d", reg);
    *data = buffer[0];
    return ESP_OK;
}

esp_err_t i2c_modify_reg(uint8_t ic2_port, uint8_t reg, uint8_t value, uint8_t mask) {
    uint8_t temp;
    ESP_RETURN_ON_ERROR(i2c_read_reg(ic2_port, reg, &temp), TAG, "Could not read value from register %d", reg);
    temp &= ~mask;
    temp |= value & mask;
    ESP_RETURN_ON_ERROR(i2c_write_reg(ic2_port, reg, temp), TAG, "Could not modify value in register %d", reg);
    ESP_RETURN_ON_ERROR(i2c_read_reg(ic2_port, reg, &temp), TAG, "Could not read value from register %d", reg);
    ESP_LOGI(TAG, "REG %x == %x", reg, temp);
    return ESP_OK;
}

void debug_print_reg(char* reg_name, uint8_t reg_address, char** reg_descriptions, uint8_t data, bool last) {
    // Print
    const char* seperator = "+=========================+--------------------+--------------------+--------------------+--------------------+--------------------+--------------------+--------------------+--------------------+\n";
    printf(seperator);
    printf("|%25s", reg_name);
    for(uint8_t i = 0; i < 8; i++) {
        if(reg_descriptions[i] == NULL) {
            printf("|            RESERVED");
        } else {
            printf("|%20s", reg_descriptions[i]);
        }
    }
    printf("|\n");
    // Print register values
    printf("|                     0x%02x", reg_address);
    for(uint8_t i = 0; i < 8; i++) {
        if(reg_descriptions[i] == NULL) {
            printf("|                   X");
        } else {
            uint8_t bit = data >> i & 0x01;
            printf("|                   %d", bit);
        }
    }
    printf("|\n");
    if(last) {
        printf(seperator);
    }
}

void debug_set_reg_descriptions(char* desc[8], char* desc0, char* desc1, char* desc2, char* desc3, char* desc4, char* desc5, char* desc6, char* desc7) {
    desc[0] = desc0;
    desc[1] = desc1;
    desc[2] = desc2;
    desc[3] = desc3;
    desc[4] = desc4;
    desc[4] = desc4;
    desc[5] = desc5;
    desc[6] = desc6;
    desc[7] = desc7;
}

void haptic_register_dump(uint8_t ic2_port) {
    ESP_LOGI(TAG, "Start of DRV2605 Register dump:");
    uint8_t data;
    char* descriptions[8] = {"DEVICE_ID[2]", "DEVICE_ID[1]", "DEVICE_ID[0]", NULL, "DIAG_RESULT", NULL, "OVER_TEMP", "OC_DETECT"};
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_STATUS, &data));
    debug_print_reg("Status", DRV2605_REG_STATUS, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "DEV_RESET", "STANDBY", NULL, NULL, NULL, "MODE[2]", "MODE[1]", "MODE[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_MODE, &data));
    debug_print_reg("Mode", DRV2605_REG_MODE, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "RTP_INPUT[7]", "RTP_INPUT[6]", "RTP_INPUT[5]", "RTP_INPUT[4]", "RTP_INPUT[3]", "RTP_INPUT[2]", "RTP_INPUT[1]", "RTP_INPUT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_RTPIN, &data));
    debug_print_reg("Real Time Playback Input", DRV2605_REG_RTPIN, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, NULL, NULL, NULL, "HI_Z", NULL, "LIBRARY_SEL[2]", "LIBRARY_SEL[1]", "LIBRARY_SEL[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_LIBRARY, &data));
    debug_print_reg("Library Select", DRV2605_REG_LIBRARY, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "WAIT", "WAV_FRM_SEQ[6]", "WAV_FRM_SEQ[5]", "WAV_FRM_SEQ[4]", "WAV_FRM_SEQ[3]", "WAV_FRM_SEQ[2]", "WAV_FRM_SEQ[1]", "WAV_FRM_SEQ[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ1, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ1, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ2, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ2, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ3, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ3, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ4, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ4, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ5, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ5, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ6, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ6, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ7, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ7, descriptions, data, false);
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_WAVESEQ8, &data));
    debug_print_reg("Waveform Sequencer", DRV2605_REG_WAVESEQ8, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "GO");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_GO, &data));
    debug_print_reg("Mode", DRV2605_REG_GO, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "ODT[7]", "ODT[6]", "ODT[5]", "ODT[4]", "ODT[3]", "ODT[2]", "ODT[1]", "ODT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_OVERDRIVE, &data));
    debug_print_reg("Overdrive Time Offset", DRV2605_REG_OVERDRIVE, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "SPT[7]", "SPT[6]", "SPT[5]", "SPT[4]", "SPT[3]", "SPT[2]", "SPT[1]", "SPT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_SUSTAINPOS, &data));
    debug_print_reg("Sustain Time Pos. Offset", DRV2605_REG_SUSTAINPOS, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "SNT[7]", "SNT[6]", "SNT[5]", "SNT[4]", "SNT[3]", "SNT[2]", "SNT[1]", "SNT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_SUSTAINNEG, &data));
    debug_print_reg("Sustain Time Neg. Offset", DRV2605_REG_SUSTAINNEG, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "BRT[7]", "BRT[6]", "BRT[5]", "BRT[4]", "BRT[3]", "BRT[2]", "BRT[1]", "BRT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_BREAK, &data));
    debug_print_reg("Break Time Offset", DRV2605_REG_BREAK, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, NULL, NULL, NULL, NULL, "PEAK_TIME[1]", "PEAK_TIME[0]", "FILTER[1]", "FILTER[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUDIOCTRL, &data));
    debug_print_reg("Audio-to-Vibe(A2V) Contr.", DRV2605_REG_AUDIOCTRL, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "MIN_INPUT[7]", "MIN_INPUT[6]", "MIN_INPUT[5]", "MIN_INPUT[4]", "MIN_INPUT[3]", "MIN_INPUT[2]", "MIN_INPUT[1]", "MIN_INPUT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUDIOLVL, &data));
    debug_print_reg("A2V Minimum Input Level", DRV2605_REG_AUDIOLVL, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "MAX_INPUT[7]", "MAX_INPUT[6]", "MAX_INPUT[5]", "MAX_INPUT[4]", "MAX_INPUT[3]", "MAX_INPUT[2]", "MAX_INPUT[1]", "MAX_INPUT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUDIOMAX, &data));
    debug_print_reg("A2V Maximum Input Level", DRV2605_REG_AUDIOMAX, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "MIN_DRIVE[7]", "MIN_DRIVE[6]", "MIN_DRIVE[5]", "MIN_DRIVE[4]", "MIN_DRIVE[3]", "MIN_DRIVE[2]", "MIN_DRIVE[1]", "MIN_DRIVE[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUDIOOUTMIN, &data));
    debug_print_reg("A2V Minimum Output Drive", DRV2605_REG_AUDIOOUTMIN, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "MAX_DRIVE[7]", "MAX_DRIVE[6]", "MAX_DRIVE[5]", "MAX_DRIVE[4]", "MAX_DRIVE[3]", "MAX_DRIVE[2]", "MAX_DRIVE[1]", "MAX_DRIVE[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUDIOOUTMAX, &data));
    debug_print_reg("A2V Maximum Output Drive", DRV2605_REG_AUDIOOUTMAX, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "RATED_VOLTAGE[7]", "RATED_VOLTAGE[6]", "RATED_VOLTAGE[5]", "RATED_VOLTAGE[4]", "RATED_VOLTAGE[3]", "RATED_VOLTAGE[2]", "RATED_VOLTAGE[1]", "RATED_VOLTAGE[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_RATEDV, &data));
    debug_print_reg("Rated Voltage", DRV2605_REG_RATEDV, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "OD_CLAMP[7]", "OD_CLAMP[6]", "OD_CLAMP[5]", "OD_CLAMP[4]", "OD_CLAMP[3]", "OD_CLAMP[2]", "OD_CLAMP[1]", "OD_CLAMP[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_CLAMPV, &data));
    debug_print_reg("Overdrive Clamp Voltage", DRV2605_REG_CLAMPV, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "A_CAL_COMP[7]", "A_CAL_COMP[6]", "A_CAL_COMP[5]", "A_CAL_COMP[4]", "A_CAL_COMP[3]", "A_CAL_COMP[2]", "A_CAL_COMP[1]", "A_CAL_COMP[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUTOCALCOMP, &data));
    debug_print_reg("Auto Cal. Comp. Result", DRV2605_REG_AUTOCALCOMP, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "A_CAL_BEMF[7]", "A_CAL_BEMF[6]", "A_CAL_BEMF[5]", "A_CAL_BEMF[4]", "A_CAL_BEMF[3]", "A_CAL_BEMF[2]", "A_CAL_BEMF[1]", "A_CAL_BEMF[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_AUTOCALEMP, &data));
    debug_print_reg("Auto Cal. Back-EMF Res.", DRV2605_REG_AUTOCALEMP, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "N_ERM_LRA", "FB_BRAKE_FACTOR[2]", "FB_BRAKE_FACTOR[1]", "FB_BRAKE_FACTOR[0]", "LOOP_GAIN[1]", "LOOP_GAIN[0]", "BEMF_GAIN[1]", "BEMF_GAIN[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_FEEDBACK, &data));
    debug_print_reg("Feedback Control", DRV2605_REG_FEEDBACK, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "STARTUP_BOOST", NULL, "AC_COUPLE", "DRIVE_TIME[4]", "DRIVE_TIME[3]", "DRIVE_TIME[2]", "DRIVE_TIME[1]", "DRIVE_TIME[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_CONTROL1, &data));
    debug_print_reg("Control1", DRV2605_REG_CONTROL1, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "BIDIR_INPUT", "BRAKE_STABILIZER", "SAMPLE_TIME[1]", "SAMPLE_TIME[0]", "BLANKING_TIME[1]", "BLANKING_TIME[0]", "IDISS_TIME[1]", "IDISS_TIME[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_CONTROL2, &data));
    debug_print_reg("Control2", DRV2605_REG_CONTROL2, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "NG_THRESH[1]", "NG_THRESH[0]", "ERM_OPEN_LOOP", "SUPPLY_COMP_DIS", "DATA_FORMAT_RTP", "LRA_DRIVE_MODE", "N_PWM_ANALOG", "LRA_OPEN_LOOP");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_CONTROL3, &data));
    debug_print_reg("Control3", DRV2605_REG_CONTROL3, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "ZC_DET_TIME[1]", "ZC_DET_TIME[0]", "AUTO_CAL_TIME[1]", "AUTO_CAL_TIME[0]", NULL, "OTP_STATUS", NULL, "OTP_PROGRAM");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_CONTROL4, &data));
    debug_print_reg("Control4", DRV2605_REG_CONTROL4, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "AUTO_OL_CNT[1]", "AUTO_OL_CNT[0]", "LRA_AUTO_OPEN_LOOP", "PLAYBACK_INTERVAL", "BLANKING_TIME[3]", "BLANKING_TIME[2]", "IDISS_TIME[3]", "IDISS_TIME[2]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_CONTROL5, &data));
    debug_print_reg("Control5", DRV2605_REG_CONTROL5, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, NULL, "OL_LRA_PERIOD[6]", "OL_LRA_PERIOD[5]", "OL_LRA_PERIOD[4]", "OL_LRA_PERIOD[3]", "OL_LRA_PERIOD[2]", "OL_LRA_PERIOD[1]", "OL_LRA_PERIOD[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_OPNLOOPPER, &data));
    debug_print_reg("LRA Open Loop Period", DRV2605_REG_OPNLOOPPER, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "VBAT[7]", "VBAT[6]", "VBAT[5]", "VBAT[4]", "VBAT[3]", "VBAT[2]", "VBAT[1]", "VBAT[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_VBAT, &data));
    debug_print_reg("V(BAT) Voltage Monitor", DRV2605_REG_VBAT, descriptions, data, false);
    debug_set_reg_descriptions(descriptions, "LRA_PERIOD[7]", "LRA_PERIOD[6]", "LRA_PERIOD[5]", "LRA_PERIOD[4]", "LRA_PERIOD[3]", "LRA_PERIOD[2]", "LRA_PERIOD[1]", "LRA_PERIOD[0]");
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_LRARESON, &data));
    debug_print_reg("LRA Resonance Period", DRV2605_REG_LRARESON, descriptions, data, true);
}

void haptic_reset(uint8_t ic2_port) {
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_MODE, DRV2605_MASK_MODE_RESET));
    uint8_t reset_in_progress = 1;
    while (reset_in_progress != 0) {
        if(i2c_read_reg(ic2_port, DRV2605_REG_MODE, &reset_in_progress) == ESP_OK) {
            reset_in_progress &= DRV2605_MASK_MODE_RESET;
        }
    }
}

void haptic_set_mode(uint8_t ic2_port, DRV2605_mode_t mode) {
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_MODE, mode, DRV2605_MASK_MODE_MODE));
}

void haptic_set_standby(uint8_t ic2_port, bool standby) {
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_MODE, (standby << 6), DRV2605_MASK_MODE_STANDBY));
}

void haptic_select_library(uint8_t ic2_port, DRV2605_library_t lib) {
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_LIBRARY, lib, DRV2605_MASK_LIBRARY_SEL));
}

void haptic_realtime(uint8_t ic2_port, int8_t input) {
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_RTPIN, (uint8_t) input));
}

void haptic_go(uint8_t ic2_port) {
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_GO, 1));
}

void haptic_set_waveform(uint8_t ic2_port, uint8_t slot, DRV2605_effect_t effect) {
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_WAVESEQ1 + slot, effect & 0x7F));
}

void haptic_set_delay(uint8_t ic2_port, uint8_t slot, uint16_t delay_ms) {
    uint8_t delay_value = delay_ms / 10;
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_WAVESEQ1 + slot, 0x80 | delay_value));
}

void haptic_configure_offsets(uint8_t ic2_port, DRV2605_offsets_t offsets) {
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_OVERDRIVE, (uint8_t) offsets.overdrive_time_offset));
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_SUSTAINPOS, (uint8_t) offsets.sustain_time_offset_positive));
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_SUSTAINNEG, (uint8_t) offsets.sustain_time_offset_negative));
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_BREAK, (uint8_t) offsets.break_time_offset));
}

void haptic_set_motor_type(uint8_t ic2_port, DRV2605_motor_type_t motor_type) {
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_FEEDBACK, (motor_type << 7), DRV2605_MASK_FEEDBACK_ERM_LRA));
}

void haptic_calculate_LRA_calibration(DRV2605_autocalibration_inputs_t* configuration, double v_rated, double v_max, double f_res) {
    configuration->od_clamp = round(v_max / 21.22E-3);
    double t = 0.00015 + 0.00005 * configuration->sample_time;
    configuration->rated_voltage = round((sqrt(1.0 - (4.0 * t + 300E-6) * ((double)f_res)) * v_rated) / 20.58E-3);
    configuration->drive_time = round(((0.5 * (1.0 / f_res)) - 0.001) / 0.0002);
}

void haptic_calculate_ERM_calibration(DRV2605_autocalibration_inputs_t* configuration, double v_rated, double v_max, double drive_time_ms) {
    double translation_array[] = {45E-6, 75E-6, 150E-6, 225E-6};
    double t_drive_time = (drive_time_ms - 1E-3) / 0.2E-3;
    double t_blanking_time = 75E-6;
    if (configuration->blanking_time < 4) {
        t_blanking_time = translation_array[configuration->blanking_time];
    }
    double t_idiss_time = 75E-6;
    if (configuration->IDISS_time < 4) {
        t_idiss_time = translation_array[configuration->IDISS_time];
    }
    double od_clamp = (v_max * (t_drive_time + t_idiss_time + t_blanking_time)) / (21.64E-3*(t_drive_time - 300E-6));
    configuration->drive_time = round(t_drive_time);
    configuration->od_clamp = round(od_clamp);
    configuration->rated_voltage = round(v_rated / 21.18E-3);
}

void haptic_set_calibration_inputs(uint8_t ic2_port, DRV2605_autocalibration_inputs_t* configuration) {
    uint8_t temp =
        ((configuration->loop_gain << 2) & DRV2605_MASK_FEEDBACK_LOOP_GAIN) |
        ((configuration->break_factor << 4) & DRV2605_MASK_FEEDBACK_BREAK_FACTOR);
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_FEEDBACK, temp, DRV2605_MASK_FEEDBACK_BREAK_FACTOR | DRV2605_MASK_FEEDBACK_LOOP_GAIN));
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_RATEDV, configuration->rated_voltage));
    ESP_ERROR_CHECK(i2c_write_reg(ic2_port, DRV2605_REG_CLAMPV, configuration->od_clamp));
    temp =
        ((configuration->auto_cal_time << 4) & DRV2605_MASK_CONTROL4_AUTO_CAL_TIME) |
        ((configuration->ZC_det_time << 6) & DRV2605_MASK_CONTROL4_ZC_DET_TIME);
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_CONTROL4, temp, DRV2605_MASK_CONTROL4_AUTO_CAL_TIME | DRV2605_MASK_CONTROL4_ZC_DET_TIME));
    ESP_ERROR_CHECK(i2c_modify_reg(ic2_port, DRV2605_REG_CONTROL1, configuration->drive_time, DRV2605_MASK_CONTROL1_DRIVE_TIME));
    temp =
        ((configuration->sample_time << 4) & DRV2605_MASK_CONTROL2_SAMPLE_TIME) |
        ((configuration->blanking_time << 2) & DRV2605_MASK_CONTROL2_BLANKING_TIME) |
        (configuration->IDISS_time & DRV2605_MASK_CONTROL2_IDISS_TIME);
    ESP_ERROR_CHECK(i2c_modify_reg(
        ic2_port,
        DRV2605_REG_CONTROL2,
        temp,
        DRV2605_MASK_CONTROL2_SAMPLE_TIME | DRV2605_MASK_CONTROL2_BLANKING_TIME | DRV2605_MASK_CONTROL2_IDISS_TIME
    ));
    temp =
        (((configuration->IDISS_time & 0x0C) >> 2) & DRV2605_MASK_CONTROL5_IDISS_TIME) |
        (configuration->blanking_time & DRV2605_MASK_CONTROL5_BLANKING_TIME);
    ESP_ERROR_CHECK(i2c_modify_reg(
        ic2_port,
        DRV2605_REG_CONTROL5,
        temp,
        DRV2605_MASK_CONTROL5_IDISS_TIME | DRV2605_MASK_CONTROL5_BLANKING_TIME
    ));
}

bool haptic_calibrate(uint8_t ic2_port, DRV2605_autocalibration_inputs_t* configuration) {
    haptic_set_mode(ic2_port, DRV2605_MODE_AUTO_CALIBRATION);
    haptic_set_calibration_inputs(ic2_port, configuration);
    haptic_go(ic2_port);
    uint8_t calibration_running = 1;
    while (calibration_running != 0) {
        ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_GO, &calibration_running));
    }
    uint8_t status;
    ESP_ERROR_CHECK(i2c_read_reg(ic2_port, DRV2605_REG_STATUS, &status));
    return (status & 0x08) != 0;
}

DRV2605_autocalibration_inputs_t haptic_init(uint8_t ic2_port, DRV2605_motor_type_t motor_type) {
    haptic_reset(ic2_port);
    uint16_t tries = 0;
    uint8_t dummy;
    while(tries < 1000) {
        if(i2c_read_reg(ic2_port, DRV2605_REG_STATUS, &dummy) == ESP_OK) {
            break;
        }
    }
    haptic_set_standby(ic2_port, false);
    haptic_set_motor_type(ic2_port, motor_type);
    if(motor_type == DRV2605_MOTOR_TYPE_LRA) {
        haptic_select_library(ic2_port, DRV2605_LIBRARY_LRA);
    } else {
        haptic_select_library(ic2_port, DRV2605_LIBRARY_TS2200_LIB_A);
    }

    // populate with recomended defaults from datasheet
    DRV2605_autocalibration_inputs_t cal_settings = {
        .motor_type = motor_type,
        .break_factor = 2, // FB_BRAKE_FACTOR
        .loop_gain = 2, // LOOP_GAIN
        .auto_cal_time = 3, // AUTO_CAL_TIME
        .sample_time = 3, // SAMPLE_TIME
        .blanking_time = 1, // BLANKING_TIME
        .IDISS_time = 1, // IDISS_TIME
        .ZC_det_time = 0, // ZC_DET_TIME
        .drive_time = 0x13,/*calculated*/ // DRIVE_TIME
        .rated_voltage = 0x3E,/*calculated*/ // RATED_VOLTAGE
        .od_clamp = 0x8C,/*calculated*/ // OD_CLAMP
    };
    return cal_settings;
}

void haptic_click(uint8_t ic2_port) {
    haptic_set_waveform(ic2_port, 0, DRV2605_EFFECT_StrongClick_100);
    haptic_set_waveform(ic2_port, 1, DRV2605_EFFECT_STOP_SEQUENCE);
    haptic_go(ic2_port);
}