#ifndef __APS_LERNING_BOARD_BSP_H__
#define __APS_LERNING_BOARD_BSP_H__

typedef enum {
    USER_LED1,
    USER_LED2,
    LEDS_TOTAL,
} USER_LED;

typedef enum {
    USER_SW1,
    USER_SW2,
    SWITCHES_TOTAL,
} USER_SWITCH;

#define USER_LED_TURN_ON  (1)
#define USER_LED_TURN_OFF (0)


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    int user_switch_initialize(void);
    void user_switch_terminate(void);
    void led_initialize(void);
    void led_onoff(USER_LED led, int value);
    int read_user_switch(USER_SWITCH sw);
    void user_switch_interrupt();
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __APS_LERNING_BOARD_BSP_H__ */
