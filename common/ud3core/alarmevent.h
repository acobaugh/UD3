/*
 * UD3
 *
 * Copyright (c) 2018 Jens Kerrinnes
 * Copyright (c) 2015 Steve Ward
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#if !defined(alarmevent_H)
#define alarmevent_H
    
#include <device.h>
#include "FreeRTOS.h"
#include "TTerm.h"
    
typedef struct __alarms__ {
uint16_t num;
uint8_t alarm_level;
char* message;
uint32_t timestamp;
uint32_t value;
} ALARMS;


    
BaseType_t ptr_is_in_flash(void* ptr);
void alarm_push_c(uint8_t level, char* message, uint16_t len, int32_t value);
void alarm_push(uint8_t level, const char* message, int32_t value);
uint32_t alarm_get_num();
void alarm_init();
uint32_t alarm_get(uint32_t index, ALARMS * alm);
void alarm_clear();
uint32_t alarm_pop(ALARMS * alm);
uint32_t alarm_free(ALARMS * alm);
uint8_t CMD_alarms(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);

#define ALM_PRIO_INFO       0
#define ALM_PRIO_WARN       1
#define ALM_PRIO_ALARM      2
#define ALM_PRIO_CRITICAL   3

#define ALM_NO_VALUE        0x80000000


static const char warn_general_startup[]= "INFO: UD3 startup";

static const char warn_task_analog[]= "TASK: Analog started";

static const char warn_task_serial_cli[]= "TASK: Serial-CLI started";
static const char warn_task_usb_cli[]= "TASK: USB-CLI started";
static const char warn_task_min_cli[]= "TASK: MIN-CLI started";

static const char warn_task_fault[]= "TASK: Fault started";
static const char warn_task_midi[]= "TASK: MIDI started";
static const char warn_task_min[]= "TASK: MIN started";
static const char warn_task_i2c[]= "TASK: I2C started";

static const char warn_PCA9685_malloc[]= "PCA9685: Malloc failed";

static const char warn_task_VT100_overlay[]= "TASK: Overlay started VT100";
static const char warn_task_TT_overlay[]= "TASK: Overlay started TT";
static const char warn_task_MQTT_overlay[]= "TASK: Overlay started MQTT";

static const char warn_task_thermistor[]= "TASK: Thermistor started";
static const char warn_task_uart[]= "TASK: UART started";
static const char warn_task_usb[]= "TASK: USB started";

static const char warn_bus_charging[]= "BUS: Charging";
static const char warn_bus_ready[]= "BUS: Ready";
static const char warn_bus_fault[]= "BUS: Fault";
static const char warn_bus_off[]= "BUS: Off";
static const char warn_bus_undervoltage[]= "BUS: Undervoltage";

static const char warn_temp1_fault[]= "NTC: Temperature Therm1 high";
static const char warn_temp2_fault[]= "NTC: Temperature Therm2 high";
static const char warn_temp_adc_zero[]= "ADC: Temperature ADC offset";

static const char warn_driver_undervoltage[]= "DRIVER: Undervoltage";

static const char warn_kill_set[]= "FAULT: Killbit set";
static const char warn_kill_reset[]= "INFO: Killbit reset";
static const char warn_feedback_error[]= "FAULT: No Feedback";

static const char warn_eeprom_no_dataset[]= "EEPROM: No or old dataset found";
static const char warn_eeprom_unknown_id[]= "EEPROM: Found unknown ID";
static const char warn_eeprom_unknown_param[]= "EEPROM: Found unknown parameter";
static const char warn_eeprom_loaded[]= "EEPROM: Dataset loaded";
static const char warn_eeprom_written[]= "EEPROM: Dataset written changes";


static const char warn_watchdog[]= "WD: Watchdog triggerd";

static const char warn_serial_overrun[]= "COM: Serial buffer overrun";
static const char warn_midi_overrun[]= "COM: MIDI buffer overrun";

static const char warn_min_id[]= "COM: Unknown MIN ID";
static const char warn_min_command[]= "COM: Unknown command";
static const char warn_min_reset[]= "COM: MIN reset";
static const char warn_min_event[]= "COM: Unknown event";

static const char warn_midi_noSpace[]= "MIDI: No Space";

static const char warn_sid_malformed[]= "SID: Malformed SID frame";
static const char warn_sid_old[]= "SID: Old SID frame received";

static const char warn_interrupter_ext[]= "INT: External interrupter active";

static const char warn_event_get_info[]= "EVENT: Fibernet got ID";
static const char warn_event_eth_init_fail[]= "EVENT: Fibernet ETH init fail";
static const char warn_event_eth_init_done[]= "EVENT: Fibernet ETH init success";
static const char warn_event_eth_link_up[]= "EVENT: Fibernet link up";
static const char warn_event_eth_link_down[]= "EVENT: Fibernet link down";
static const char warn_event_eth_dhcp_success[]= "EVENT: DHCP success";
static const char warn_event_eth_dhcp_fail[]= "EVENT: DHCP fail";
static const char warn_event_fs_card_con[]= "EVENT: Fibernet SD connected";
static const char warn_event_fs_card_dis[]= "EVENT: Fibernet SD disconnected";
    
    
/* ------------------------------------------------------------------------ */
#endif
/* [] END OF FILE */