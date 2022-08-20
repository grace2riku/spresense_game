//#include <sdk/config.h>
#include <stdio.h>

#include <arch/board/board.h>
#include <arch/chip/pin.h>

#define message(format, ...)    printf(format, ##__VA_ARGS__)

// APS学習ボードピンアサイン
#define SWITCH_1    (39)
#define SWITCH_2    (29)
#define USER_LED_1  (46)
#define USER_LED_2  (47)

bool exit_electric_guitar = false;


static int electric_guitar_gpio_switch_2_handler(int irq, FAR void *context, FAR void *arg)
{
  int sw2_status = board_gpio_read(SWITCH_2);
  int sw1_status = board_gpio_read(SWITCH_1);

  if (!sw1_status && !sw2_status) exit_electric_guitar = true;

  return 0;
}

void electric_guitar_gpio_create(void)
{
  board_gpio_config(SWITCH_1, 0, true, false, PIN_PULLUP);

  /* 割り込み設定 */
  board_gpio_intconfig(SWITCH_2, INT_FALLING_EDGE,    true, electric_guitar_gpio_switch_2_handler); 

  if (board_gpio_int(SWITCH_2, true) < 0) {
    message("gpio_create board_gpio_int(switch_2) failure.\n");
  }  

  return;
}


void electric_guitar_gpio_destroy(void)
{
  if (board_gpio_int(SWITCH_2, false) < 0) {
    message("gpio_destroy board_gpio_int(switch_2) failure.\n");
  }  

  return;
}