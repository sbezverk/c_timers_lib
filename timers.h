#pragma once

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef enum TIMER_STATE_ {
  TIMER_DELETED,
  TIMER_INIT,
  TIMER_RUNNING,
  TIMER_CANCELLED,
  TIMER_PAUSED,
  TIMER_RESUMED
} TIMER_STATE_T;

typedef struct flex_timer_ {
  timer_t posix_timer;
  void *user_arg;
  unsigned long exp_timer_ms;
  unsigned long secondary_exp_timer_ms;
  void (*cb)(flex_timer_t *, void *);
  uint32_t threshold;
  bool exponential_backoff;

  // Dynamic attributes
  unsigned long time_remaining;
  uint32_t invocation_counter;
  struct itimerspec ts;
  unsigned long exp_backoff_timer;
  TIMER_STATE_T timer_state;
} flex_timer_t;

void print_current_system_time(void);

unsigned long timespec_to_millisec(struct itimerspec *time);

void timer_fill_itimerspec(struct itimerspec *ts, unsigned long msec);
