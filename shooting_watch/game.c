#include "game.h"
#include "initialize.h"

static void run(void);

typedef enum
{
    STOP,
    PRE_SHOOTING,
    SHOOTING,
} GAME_STATE;


void game_start(void) {
    initialize();

    run();

    terminate();    
}

static void run(void) {

}

static void output_message(const char* msg) {

}

