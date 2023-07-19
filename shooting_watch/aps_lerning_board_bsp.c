#include <arch/board/board.h>
#include "aps_lerning_board_bsp.h"

// APS学習ボードピンアサイン
#define SWITCH_1_PIN    (39)
#define SWITCH_2_PIN    (29)
#define USER_LED_1_PIN  (46)
#define USER_LED_2_PIN  (47)

int user_switch_initialize(void) {

}

void user_switch_terminate(void) {

}

static uint32_t led_name2pin_number[LEDS_TOTAL] = {USER_LED_1_PIN, USER_LED_2_PIN};
void led_onoff(USER_LED led, int value) {
    board_gpio_write(led_name2pin_number[led], value);
}

static uint32_t switch_name2pin_number[SWITCHES_TOTAL] = {SWITCH_1_PIN, SWITCH_2_PIN};
int read_user_switch(USER_SWITCH sw) {
    return board_gpio_read(switch_name2pin_number[sw]);
}

void user_switch_interrupt() {

}
