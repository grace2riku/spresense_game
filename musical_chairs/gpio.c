//#include <sdk/config.h>
#include <stdio.h>

#include <arch/board/board.h>
#include <arch/chip/pin.h>

#define message(format, ...)    printf(format, ##__VA_ARGS__)

// APS学習ボードピンアサイン
#define SWITCH_1  (39)
#define SWITCH_2  (29)
#define USER_LED_1  (46)
#define USER_LED_2  (47)

bool play_or_pause_trigger = false;
bool exit_app = false;

static int gpio_switch_1_handler(int irq, FAR void *context, FAR void *arg)
{
  int sw1_status = board_gpio_read(SWITCH_1);
  board_gpio_write(USER_LED_1, sw1_status);

  if (!sw1_status) exit_app = true;

  return 0;
}

static int gpio_switch_2_handler(int irq, FAR void *context, FAR void *arg)
{
  int sw2_status = board_gpio_read(SWITCH_2);
  board_gpio_write(USER_LED_2, sw2_status);

  if (!sw2_status) play_or_pause_trigger = true;

  return 0;
}

int gpio_init(void)
{
  /* 割り込み設定 */
  board_gpio_intconfig(SWITCH_1, INT_BOTH_EDGE,    true, gpio_switch_1_handler); 
  board_gpio_intconfig(SWITCH_2, INT_BOTH_EDGE,    true, gpio_switch_2_handler); 

  board_gpio_int(SWITCH_1, true);  
  board_gpio_int(SWITCH_2, true);  

  return 0;
}
