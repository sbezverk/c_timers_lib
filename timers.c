#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "timers.h"

struct sigevent evp;
timer_t timer_id;

void print_current_system_time(void) {
  time_t t;
  time(&t);

  printf("%s", ctime(&t));
}

unsigned long timespec_to_millisec(struct itimerspec *ts) {
  return (unsigned long)((ts->it_value.tv_sec * 1000) +
                         (ts->it_value.tv_nsec / 1000000));
}

void timer_fill_itimerspec(struct itimerspec *ts, unsigned long msec) {
  if (ts) {
    ts->it_value.tv_sec = msec / 1000;
    ts->it_value.tv_nsec = (msec % 1000) * 1000000;
  }
}

static inline TIMER_STATE_T timer_get_current_state(flex_timer_t *timer) {
  return timer->timer_state;
}

static inline void timer_set_state(flex_timer_t *timer, TIMER_STATE_T state) {
  timer->timer_state = state;
}