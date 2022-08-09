
#include <nuttx/config.h>
#include <stdio.h>
#include <time.h>

#include <arch/board/board.h>
#include <arch/chip/pin.h>

#define USER_LED_1  (46)
#define USER_LED_2  (47)
#define SWITCH_2    (29)

extern void shooting_watch_gpio_create(void);
extern void shooting_watch_gpio_destroy(void);

u_long shooting_count = 0;
extern bool exit_shooting_watch;
extern bool next_state;

typedef enum
{
    STOP,
    PRE_SHOOTING,
    SHOOTING,
} GAME_STATE;

typedef enum
{
    NONE,
    START,
    STAGE_END,
    END,
} GAME_EVENT;

GAME_STATE game_state = STOP;
GAME_EVENT game_event = NONE;


void shooting_process(uint32_t play_time)
{
  /* Timer Start */
  time_t start_time;
  time_t cur_time;
  int shooting_indicator = 0;

  time(&start_time);

  do {
    usleep(500 * 1000);

    shooting_indicator ^= 1;
    board_gpio_write(USER_LED_2, shooting_indicator);

  } while((time(&cur_time) - start_time) < play_time);
}


int main(int argc, FAR char *argv[])
{
  int one_time_counter = 0;

  shooting_watch_gpio_create();
  board_gpio_write(USER_LED_1, 1);
  board_gpio_write(USER_LED_2, 1);

  printf("Hello, shooting_watch!!!\n");

  while(exit_shooting_watch != true) {
    switch (game_state) {
    case STOP:
      if (one_time_counter == 0) {
        printf("----- Press SW2 to start the game.-----\n");
        printf("----- Press SW1 and SW2 to end the game.-----\n");
        one_time_counter = 1;
      }

      if (next_state) {
        next_state = false;

        if (board_gpio_int(SWITCH_2, false) < 0) {
          printf("board_gpio_int(switch_2) failure.\n");
        }  

        game_state = PRE_SHOOTING;
      }
      break;

    case PRE_SHOOTING:
      printf("Ready....\n");
      sleep(3);
      shooting_count = 0;
      game_state = SHOOTING;
      break;

    case SHOOTING:
      printf("Shooting!!!!!\n");
      shooting_process(10);

      printf("Shooting count = %ld\n", shooting_count);
      printf("\n");

      if (board_gpio_int(SWITCH_2, true) < 0) {
        printf("board_gpio_int(switch_2) failure.\n");
      }  

      one_time_counter = 0;
      game_state = STOP;
      break;

    default:
      break;
    }
  }

  exit_shooting_watch = false;
  shooting_watch_gpio_destroy();

  return 0;
}
