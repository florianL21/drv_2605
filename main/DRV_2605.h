#ifndef __DRV_2605_H__
#define __DRV_2605_H__

#include <stdio.h>
#include <stdbool.h>

#define DRV_2650_WRITE_ADDRESS 0x5A
#define DRV_2650_READ_ADDRESS 0xB5
#define DRV_2650_TIMEOUT 1000



#define DRV2605_REG_STATUS 0x00

#define DRV2605_REG_MODE 0x01
#define DRV2605_MASK_MODE_STANDBY 0x40
#define DRV2605_MASK_MODE_RESET 0x80
#define DRV2605_MASK_MODE_MODE 0x07

#define DRV2605_REG_RTPIN 0x02
#define DRV2605_REG_LIBRARY 0x03
#define DRV2605_MASK_LIBRARY_SEL 0x07
#define DRV2605_REG_WAVESEQ1 0x04
#define DRV2605_REG_WAVESEQ2 0x05
#define DRV2605_REG_WAVESEQ3 0x06
#define DRV2605_REG_WAVESEQ4 0x07
#define DRV2605_REG_WAVESEQ5 0x08
#define DRV2605_REG_WAVESEQ6 0x09
#define DRV2605_REG_WAVESEQ7 0x0A
#define DRV2605_REG_WAVESEQ8 0x0B
#define DRV2605_REG_GO 0x0C
#define DRV2605_REG_OVERDRIVE 0x0D
#define DRV2605_REG_SUSTAINPOS 0x0E
#define DRV2605_REG_SUSTAINNEG 0x0F
#define DRV2605_REG_BREAK 0x10
#define DRV2605_REG_AUDIOCTRL 0x11
#define DRV2605_REG_AUDIOLVL 0x12
#define DRV2605_REG_AUDIOMAX 0x13
#define DRV2605_REG_AUDIOOUTMIN 0x14
#define DRV2605_REG_AUDIOOUTMAX 0x15
#define DRV2605_REG_RATEDV 0x16
#define DRV2605_REG_CLAMPV 0x17
#define DRV2605_REG_AUTOCALCOMP 0x18
#define DRV2605_REG_AUTOCALEMP 0x19
#define DRV2605_REG_FEEDBACK 0x1A
#define DRV2605_MASK_FEEDBACK_ERM_LRA 0x80
#define DRV2605_MASK_FEEDBACK_BREAK_FACTOR 0x70
#define DRV2605_MASK_FEEDBACK_LOOP_GAIN 0x70
#define DRV2605_REG_CONTROL1 0x1B
#define DRV2605_MASK_CONTROL1_DRIVE_TIME 0x1F
#define DRV2605_REG_CONTROL2 0x1C
#define DRV2605_MASK_CONTROL2_SAMPLE_TIME 0x30
#define DRV2605_MASK_CONTROL2_BLANKING_TIME 0x0C
#define DRV2605_MASK_CONTROL2_IDISS_TIME 0x03
#define DRV2605_REG_CONTROL3 0x1D
#define DRV2605_REG_CONTROL4 0x1E
#define DRV2605_REG_CONTROL5 0x1F
#define DRV2605_MASK_CONTROL5_BLANKING_TIME 0x0C
#define DRV2605_MASK_CONTROL5_IDISS_TIME 0x03

#define DRV2605_MASK_CONTROL4_AUTO_CAL_TIME 0x30
#define DRV2605_MASK_CONTROL4_ZC_DET_TIME 0xC0
#define DRV2605_REG_OPNLOOPPER 0x20
#define DRV2605_REG_VBAT 0x21
#define DRV2605_REG_LRARESON 0x22

typedef enum {
    DRV2605_MOTOR_TYPE_ERM = 0x00,
    DRV2605_MOTOR_TYPE_LRA = 0x01
} DRV2605_motor_type_t;

typedef enum {
    // Waveforms are fired by setting the GO bit in the GO register
    DRV2605_MODE_INTERNAL_TRIGGER=0x00,
    // A rising edge on the IN/TRIG pin sets the GO Bit.
    // A second rising edge on the IN/TRIG pin cancels the waveform
    // if the second rising edge occurs before the GO bit has cleared.
    DRV2605_MODE_EXTERNAL_TRIGGER=0x01,
    // A PWM or analog signal is accepted at the IN/TRIG pin and used as
    // the driving source. The device actively drives the actuator while in
    // this mode. The PWM or analog input selection occurs by using the
    // N_PWM_ANALOG bit.
    DRV2605_MODE_PWM_ANALOG_INPUT=0x02,
    // An AC-coupled audio signal is accepted at the IN/TRIG pin. The
    // device converts the audio signal into meaningful haptic vibration. The
    // AC_COUPLE and N_PWM_ANALOG bits should also be set.
    DRV2605_MODE_AUDIO_VIBE=0x04,
    // The device actively drives the actuator with the contents of the
    // RTP_INPUT[7:0] bit in register 0x02.
    DRV2605_MODE_REALTIME=0x05,
    // Set the device in this mode to perform a diagnostic test on the
    // actuator. The user must set the GO bit to start the test. The test is
    // complete when the GO bit self-clears. Results are stored in the
    // DIAG_RESULT bit in register 0x00.
    DRV2605_MODE_DIAGNOSTICS=0x06,
    // Set the device in this mode to auto calibrate the device for the
    // actuator. Before starting the calibration, the user must set the all
    // required input parameters. The user must set the GO bit to start the
    // calibration. Calibration is complete when the GO bit self-clears. For
    // more information see the Auto Calibration Procedure section.
    DRV2605_MODE_AUTO_CALIBRATION=0x07
} DRV2605_mode_t;

typedef enum {
    // No library selected
    DRV2605_LIBRARY_EMPTY=0x00,
    // ### Tuned for ERMs in open loop mode
    // Rated voltage: 1.3V
    // Overdrive Voltage: 3V
    // Rise time: 40ms to 60ms
    // break time: 20ms to 40ms
    DRV2605_LIBRARY_TS2200_LIB_A=0x01,
    // ### Tuned for ERMs in open loop mode
    // Rated voltage: 3V
    // Overdrive Voltage: 3V
    // Rise time: 40ms to 60ms
    // break time: 5ms to 15ms
    DRV2605_LIBRARY_TS2200_LIB_B=0x02,
    // ### Tuned for ERMs in open loop mode
    // Rated voltage: 3V
    // Overdrive Voltage: 3V
    // Rise time: 60ms to 80ms
    // break time: 10ms to 20ms
    DRV2605_LIBRARY_TS2200_LIB_C=0x03,
    // ### Tuned for ERMs in open loop mode
    // Rated voltage: 3V
    // Overdrive Voltage: 3V
    // Rise time: 100ms to 140ms
    // break time: 15ms to 25ms
    DRV2605_LIBRARY_TS2200_LIB_D=0x04,
    // ### Tuned for ERMs in open loop mode
    // Rated voltage: 3V
    // Overdrive Voltage: 3V
    // Rise time: > 140ms
    // break time: > 30ms
    DRV2605_LIBRARY_TS2200_LIB_E=0x05,
    // ### Tuned for LRAs in closed loop mode
    DRV2605_LIBRARY_LRA=0x06,
    // ### Tuned for ERMs in open loop mode
    // Rated voltage: 4.5V
    // Overdrive Voltage: 5V
    // Rise time: 35ms to 45ms
    // break time: 10ms to 20ms
    DRV2605_LIBRARY_TS2200_LIB_F=0x07
} DRV2605_library_t;


// Effects have names and strengths.
// All effects are named in the pattern DRV2605_EFFECT_`<effect name>`_`<effect strength in %>`
typedef enum {
    // Special "effect" that will stop the wave sequencer from continuing
    DRV2605_EFFECT_STOP_SEQUENCE = 0,
    DRV2605_EFFECT_StrongClick_100 = 1,
    DRV2605_EFFECT_StrongClick_60 = 2,
    DRV2605_EFFECT_StrongClick_30 = 3,
    DRV2605_EFFECT_SharpClick_100 = 4,
    DRV2605_EFFECT_SharpClick_60 = 5,
    DRV2605_EFFECT_SharpClick_30 = 6,
    DRV2605_EFFECT_SoftBump_100 = 7,
    DRV2605_EFFECT_SoftBump_60 = 8,
    DRV2605_EFFECT_SoftBump_30 = 9,
    DRV2605_EFFECT_DoubleClick_100 = 10,
    DRV2605_EFFECT_DoubleClick_60 = 11,
    DRV2605_EFFECT_TripleClick_100 = 12,
    DRV2605_EFFECT_SoftFuzz_60 = 13,
    DRV2605_EFFECT_StrongBuzz_100 = 14,
    DRV2605_EFFECT_Alert750ms_100 = 15,
    DRV2605_EFFECT_Alert1000ms_100 = 16,
    DRV2605_EFFECT_StrongClick1_100 = 17,
    DRV2605_EFFECT_StrongClick2_80 = 18,
    DRV2605_EFFECT_StrongClick3_60 = 19,
    DRV2605_EFFECT_StrongClick4_30 = 20,
    DRV2605_EFFECT_MediumClick1_100 = 21,
    DRV2605_EFFECT_MediumClick2_80 = 22,
    DRV2605_EFFECT_MediumClick3_60 = 23,
    DRV2605_EFFECT_SharpTick_100 = 24,
    DRV2605_EFFECT_SharpTick_80 = 25,
    DRV2605_EFFECT_SharpTick_60 = 26,
    DRV2605_EFFECT_ShortDoubleClickStrong_100 = 27,
    DRV2605_EFFECT_ShortDoubleClickStrong_80 = 28,
    DRV2605_EFFECT_ShortDoubleClickStrong_60 = 29,
    DRV2605_EFFECT_ShortDoubleClickStrong_30 = 30,
    DRV2605_EFFECT_ShortDoubleClickMedium_100 = 31,
    DRV2605_EFFECT_ShortDoubleClickMedium_80 = 32,
    DRV2605_EFFECT_ShortDoubleClickMedium_60 = 33,
    DRV2605_EFFECT_ShortDoubleSharpTick_100 = 34,
    DRV2605_EFFECT_ShortDoubleSharpTick_80 = 35,
    DRV2605_EFFECT_ShortDoubleSharpTick_60 = 36,
    DRV2605_EFFECT_LongDoubleSharpClickStrong_100 = 37,
    DRV2605_EFFECT_LongDoubleSharpClickStrong_80 = 38,
    DRV2605_EFFECT_LongDoubleSharpClickStrong_60 = 39,
    DRV2605_EFFECT_LongDoubleSharpClickStrong_30 = 40,
    DRV2605_EFFECT_LongDoubleSharpClickMedium_100 = 41,
    DRV2605_EFFECT_LongDoubleSharpClickMedium_80 = 42,
    DRV2605_EFFECT_LongDoubleSharpClickMedium_60 = 43,
    DRV2605_EFFECT_LongDoubleSharpTick_100 = 44,
    DRV2605_EFFECT_LongDoubleSharpTick_80 = 45,
    DRV2605_EFFECT_LongDoubleSharpTick_60 = 46,
    DRV2605_EFFECT_Buzz_100 = 47,
    DRV2605_EFFECT_Buzz_80 = 48,
    DRV2605_EFFECT_Buzz_60 = 49,
    DRV2605_EFFECT_Buzz_40 = 50,
    DRV2605_EFFECT_Buzz_20 = 51,
    DRV2605_EFFECT_PulsingStrong_100 = 52,
    DRV2605_EFFECT_PulsingStrong_60 = 53,
    DRV2605_EFFECT_PulsingMedium_100 = 54,
    DRV2605_EFFECT_PulsingMedium_60 = 55,
    DRV2605_EFFECT_PulsingSharp_100 = 56,
    DRV2605_EFFECT_PulsingSharp_60 = 57,
    DRV2605_EFFECT_TransitionClick_100 = 58,
    DRV2605_EFFECT_TransitionClick_80 = 59,
    DRV2605_EFFECT_TransitionClick_60 = 60,
    DRV2605_EFFECT_TransitionClick_40 = 61,
    DRV2605_EFFECT_TransitionClick_20 = 62,
    DRV2605_EFFECT_TransitionClick_10 = 63,
    DRV2605_EFFECT_TransitionHum_100 = 64,
    DRV2605_EFFECT_TransitionHum_80 = 65,
    DRV2605_EFFECT_TransitionHum_60 = 66,
    DRV2605_EFFECT_TransitionHum_40 = 67,
    DRV2605_EFFECT_TransitionHum_20 = 68,
    DRV2605_EFFECT_TransitionHum_10 = 69,
    DRV2605_EFFECT_TransitionRampDownLongSmooth1_100 = 70,
    DRV2605_EFFECT_TransitionRampDownLongSmooth2_100 = 71,
    DRV2605_EFFECT_TransitionRampDownMediumSmooth1_100 = 72,
    DRV2605_EFFECT_TransitionRampDownMediumSmooth2_100 = 73,
    DRV2605_EFFECT_TransitionRampDownShortSmooth1_100 = 74,
    DRV2605_EFFECT_TransitionRampDownShortSmooth2_100 = 75,
    DRV2605_EFFECT_TransitionRampDownLongSharp1_100 = 76,
    DRV2605_EFFECT_TransitionRampDownLongSharp2_100 = 77,
    DRV2605_EFFECT_TransitionRampDownMediumSharp1_100 = 78,
    DRV2605_EFFECT_TransitionRampDownMediumSharp2_100 = 79,
    DRV2605_EFFECT_TransitionRampDownShortSharp1_100 = 80,
    DRV2605_EFFECT_TransitionRampDownShortSharp2_100 = 81,
    DRV2605_EFFECT_TransitionRampUpLongSmooth1_100 = 82,
    DRV2605_EFFECT_TransitionRampUpLongSmooth2_100 = 83,
    DRV2605_EFFECT_TransitionRampUpMediumSmooth1_100 = 84,
    DRV2605_EFFECT_TransitionRampUpMediumSmooth2_100 = 85,
    DRV2605_EFFECT_TransitionRampUpShortSmooth1_100 = 86,
    DRV2605_EFFECT_TransitionRampUpShortSmooth2_100 = 87,
    DRV2605_EFFECT_TransitionRampUpLongSharp1_100 = 88,
    DRV2605_EFFECT_TransitionRampUpLongSharp2_100 = 89,
    DRV2605_EFFECT_TransitionRampUpMediumSharp1_100 = 90,
    DRV2605_EFFECT_TransitionRampUpMediumSharp2_100 = 91,
    DRV2605_EFFECT_TransitionRampUpShortSharp1_100 = 92,
    DRV2605_EFFECT_TransitionRampUpShortSharp2_100 = 93,
    DRV2605_EFFECT_TransitionRampDownLongSmooth1_50 = 94,
    DRV2605_EFFECT_TransitionRampDownLongSmooth2_50 = 95,
    DRV2605_EFFECT_TransitionRampDownMediumSmooth1_50 = 96,
    DRV2605_EFFECT_TransitionRampDownMediumSmooth2_50 = 97,
    DRV2605_EFFECT_TransitionRampDownShortSmooth1_50 = 98,
    DRV2605_EFFECT_TransitionRampDownShortSmooth2_50 = 99,
    DRV2605_EFFECT_TransitionRampDownLongSharp1_50 = 100,
    DRV2605_EFFECT_TransitionRampDownLongSharp2_50 = 101,
    DRV2605_EFFECT_TransitionRampDownMediumSharp1_50 = 102,
    DRV2605_EFFECT_TransitionRampDownMediumSharp2_50 = 103,
    DRV2605_EFFECT_TransitionRampDownShortSharp1_50 = 104,
    DRV2605_EFFECT_TransitionRampDownShortSharp2_50 = 105,
    DRV2605_EFFECT_TransitionRampUpLongSmooth1_50 = 106,
    DRV2605_EFFECT_TransitionRampUpLongSmooth2_50 = 107,
    DRV2605_EFFECT_TransitionRampUpMediumSmooth1_50 = 108,
    DRV2605_EFFECT_TransitionRampUpMediumSmooth2_50 = 109,
    DRV2605_EFFECT_TransitionRampUpShortSmooth1_50 = 110,
    DRV2605_EFFECT_TransitionRampUpShortSmooth2_50 = 111,
    DRV2605_EFFECT_TransitionRampUpLongSharp1_50 = 112,
    DRV2605_EFFECT_TransitionRampUpLongSharp2_50 = 113,
    DRV2605_EFFECT_TransitionRampUpMediumSharp1_50 = 114,
    DRV2605_EFFECT_TransitionRampUpMediumSharp2_50 = 115,
    DRV2605_EFFECT_TransitionRampUpShortSharp1_50 = 116,
    DRV2605_EFFECT_TransitionRampUpShortSharp2_50 = 117,
    DRV2605_EFFECT_LongBuzz_100 = 118,
    DRV2605_EFFECT_SmoothHum_50 = 119,
    DRV2605_EFFECT_SmoothHum_40 = 120,
    DRV2605_EFFECT_SmoothHum_30 = 121,
    DRV2605_EFFECT_SmoothHum_20 = 122,
    DRV2605_EFFECT_SmoothHum_10 = 123
} DRV2605_effect_t;

typedef struct {
    // This bit adds a time offset to the overdrive portion of the library
    // waveforms. Some motors require more overdrive time than others,
    // therefore this register allows the user to add or remove overdrive time
    // from the library waveforms. The maximum voltage value in the library
    // waveform is automatically determined to be the overdrive portion. This
    // register is only useful in open-loop mode. Overdrive is automatic for
    // closed-loop mode. The offset is interpreted as 2s complement, therefore
    // the time offset can be positive or negative.
    // `Overdrive Time Offset (ms) = ODT[7:0] × PLAYBACK_INTERVAL`
    int8_t overdrive_time_offset;

    // This bit adds a time offset to the positive sustain portion of the library
    // waveforms. Some motors have a faster or slower response time than
    // others, therefore this register allows the user to add or remove positive
    // sustain time from the library waveforms. Any positive voltage value other
    // than the overdrive portion is considered as a sustain positive value. The
    // offset is interpreted as 2s complement, therefore the time offset can positive
    // or negative.
    // `Sustain-Time Positive Offset (ms) = SPT[7:0] * PLAYBACK_INTERVAL`
    int8_t sustain_time_offset_positive;

    // This bit adds a time offset to the negative sustain portion of the library
    // waveforms. Some motors have a faster or slower response time than
    // others, therefore this register allows the user to add or remove negative
    // sustain time from the library waveforms. Any negative voltage value other
    // than the overdrive portion is considered as a sustaining negative value. The
    // offset is interpreted as two’s complement, therefore the time offset can be
    // positive or negative.
    // `Sustain-Time Negative Offset (ms) = SNT[7:0] * PLAYBACK_INTERVAL`
    int8_t sustain_time_offset_negative;

    // This bit adds a time offset to the braking portion of the library waveforms.
    // Some motors require more braking time than others, therefore this register
    // allows the user to add or take away brake time from the library waveforms.
    // The most negative voltage value in the library waveform is automatically
    // determined to be the braking portion. This register is only useful in open-loop
    // mode. Braking is automatic for closed-loop mode. The offset is interpreted as
    // 2s complement, therefore the time offset can be positive or negative.
    // `Brake Time Offset (ms) = BRT[7:0] * PLAYBACK_INTERVAL`
    int8_t break_time_offset;

} DRV2605_offsets_t;

typedef struct {
    // Motor drive type
    // Options are
    // - DRV2605_MOTOR_TYPE_ERM
    // - DRV2605_MOTOR_TYPE_LRA
    DRV2605_motor_type_t motor_type;
    // This bit selects the feedback gain ratio between braking gain and driving gain.
    // In general, adding additional feedback gain while braking is desirable so that the
    // actuator brakes as quickly as possible. Large ratios provide less-stable
    // operation than lower ones. The advanced user can select to optimize this
    // register. Otherwise, the default value should provide good performance for most
    // actuators. This value should be set prior to running auto calibration.
    // 0: 1x
    // 1: 2x
    // 2: 3x
    // 3: 4x
    // 4: 6x
    // 5: 8x
    // 6: 16x
    // 7: Braking disabled
    uint8_t break_factor;
    // This bit selects a loop gain for the feedback control. The LOOP_GAIN[1:0] bit
    // sets how fast the loop attempts to make the back-EMF (and thus motor velocity)
    // match the input signal level. Higher loop-gain (faster settling) options provide
    // less-stable operation than lower loop gain (slower settling). The advanced user
    // can select to optimize this register. Otherwise, the default value should provide
    // good performance for most actuators. This value should be set prior to running
    // auto calibration.
    // 0: Low
    // 1: Medium (default)
    // 2: High
    // 3: Very High
    uint8_t loop_gain;
    // This bit sets the reference voltage for full-scale output during closed-loop
    // operation. The auto-calibration routine uses this register as an input, therefore
    // this register must be written with the rated voltage value of the motor before
    // calibration is performed. This register is ignored for open-loop operation
    // because the overdrive voltage sets the reference for that case. Any
    // modification of this register value should be followed by calibration to set
    // A_CAL_BEMF appropriately.
    // See the Rated Voltage Programming section for calculating the correct register
    // value.
    uint8_t rated_voltage;
    // During closed-loop operation the actuator feedback allows the output voltage
    // to go above the rated voltage during the automatic overdrive and automatic
    // braking periods. This register sets a clamp so that the automatic overdrive is
    // bounded. This bit also serves as the full-scale reference voltage for open-loop
    // operation.
    // See the Overdrive Voltage-Clamp Programming section for calculating the
    // correct register value.
    uint8_t od_clamp;
    // This bit sets the length of the auto calibration time. The AUTO_CAL_TIME[1:0]
    // bit should be enough time for the motor acceleration to settle when driven at the
    // RATED_VOLTAGE[7:0] value.
    // 0: 150 ms (minimum), 350 ms (maximum)
    // 1: 250 ms (minimum), 450 ms (maximum)
    // 2: 500 ms (minimum), 700 ms (maximum)
    // 3: 1000 ms (minimum), 1200 ms (maximum)
    uint8_t auto_cal_time;
    // ### LRA Mode: Sets initial guess for LRA drive-time in LRA mode. Drive time is
    // automatically adjusted for optimum drive in real time; however, this register
    // should be optimized for the approximate LRA frequency. If the bit is set too low,
    // it can affect the actuator startup time. If the bit is set too high, it can cause
    // instability.
    // Optimum drive time (ms) ≈ 0.5 × LRA Period
    // `Drive time (ms) = DRIVE_TIME[4:0] * 0.1 ms + 0.5 ms`
    // ### ERM Mode: Sets the sample rate for the back-EMF detection. Lower drive times
    // cause higher peak-to-average ratios in the output signal, requiring more supply
    // headroom. Higher drive times cause the feedback to react at a slower rate.
    // `Drive Time (ms) = DRIVE_TIME[4:0] * 0.2 ms + 1 ms`
    uint8_t drive_time;
    // #### ONLY RELEVANT FOR LRAs
    // LRA auto-resonance sampling time (Advanced use only)
    // 0: 150 µs
    // 1: 200 µs
    // 2: 250 µs
    // 3: 300 µs
    uint8_t sample_time;
    // #### ONLY RELEVANT FOR LRAs
    // Blanking time before the back-EMF AD makes a conversion. (Advanced use only)
    // Blanking time for LRA has an additional 2 bits (BLANKING_TIME[3:2]) located in
    // register 0x1F. Depending on the status of N_ERM_LRA the blanking time
    // represents different values.
    // N_ERM_LRA = 0 (ERM mode)
    // 0: 45 µs
    // 1: 75 µs
    // 2: 150 µs
    // 3: 225 µs
    // N_ERM_LRA = 1(LRA mode)
    // 0: 15 µs
    // 1: 25 µs
    // 2: 50 µs
    // 3: 75 µs
    // 4: 90 µs
    // 5: 105 µs
    // 6: 120 µs
    // 7: 135 µs
    // 8: 150 µs
    // 9: 165 µs
    // 10: 180 µs
    // 11: 195 µs
    // 12: 210 µs
    // 13: 235 µs
    // 14: 260 µs
    // 15: 285 µs
    uint8_t blanking_time;
    // #### ONLY RELEVANT FOR LRAs
    // Current dissipation time. This bit is the time allowed for the current to dissipate
    // from the actuator between PWM cycles for flyback mitigation. (Advanced use
    // only)
    // the current dissipation time for LRA has an additional 2 bits (IDISS_TIME[3:2])
    // located in register 0x1F. Depending on the status of N_ERM_LRA the idiss time
    // represents different values
    // N_ERM_LRA = 0 (ERM mode)
    // 0: 45 µs
    // 1: 75 µs
    // 2: 150 µs
    // 3: 225 µs
    // N_ERM_LRA = 1(LRA mode)
    // 0: 15 µs
    // 1: 25 µs
    // 2: 50 µs
    // 3: 75 µs
    // 4: 90 µs
    // 5: 105 µs
    // 6: 120 µs
    // 7: 135 µs
    // 8: 150 µs
    // 9: 165 µs
    // 10: 180 µs
    // 11: 195 µs
    // 12: 210 µs
    // 13: 235 µs
    // 14: 260 µs
    // 15: 285 µs
    uint8_t IDISS_time;
    // #### ONLY RELEVANT FOR LRAs
    // This bit sets the minimum length of time devoted for detecting a zero crossing
    // (advanced use only).
    // 0: 100 µs
    // 1: 200 µs
    // 2: 300 µs
    // 3: 390 µs
    uint8_t ZC_det_time;
} DRV2605_autocalibration_inputs_t;

DRV2605_autocalibration_inputs_t haptic_init(uint8_t ic2_port, DRV2605_motor_type_t motor_type);
void haptic_click(uint8_t ic2_port);
void haptic_calculate_LRA_calibration(DRV2605_autocalibration_inputs_t* configuration, double v_rated, double v_max, double f_res);
void haptic_calculate_ERM_calibration(DRV2605_autocalibration_inputs_t* configuration, double v_rated, double v_max, double drive_time_ms);
bool haptic_calibrate(uint8_t ic2_port, DRV2605_autocalibration_inputs_t* configuration);
void haptic_register_dump(uint8_t ic2_port);
void haptic_set_mode(uint8_t ic2_port, DRV2605_mode_t mode);

#endif