#include <arch/board/board.h>
#include "aps_lerning_board_bsp.h"

// APS学習ボードピンアサイン
#define SWITCH_1  (39)
#define SWITCH_2  (29)
#define USER_LED_1  (46)
#define USER_LED_2  (47)

int user_switch_initialize(void) {

}

void user_switch_terminate(void) {

}

static uint32_t led_name2pin_number[LEDS_TOTAL] = {USER_LED_1, USER_LED_2};
void led_onoff(USER_LED led, int value) {
    board_gpio_write(led_name2pin_number[led], value);
}

static uint32_t switch_name2pin_number[SWITCHES_TOTAL] = {USER_SW1, USER_SW2};
int read_user_switch(USER_SWITCH sw) {
    return board_gpio_read(switch_name2pin_number[sw]);
}

void user_switch_interrupt() {

}
