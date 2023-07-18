#include <arch/board/board.h>
#include "shooting_count.h"

static long shooting_count = 0;

void reset_count(void) {

}

long get_count(void) {

    return shooting_count;
}
