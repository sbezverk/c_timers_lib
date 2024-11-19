#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "timers.h"

extern struct sigevent evp;
extern timer_t timer_id;

void timer_handler(void *arg) {
  print_current_system_time();
  if (arg) {
    printf("timer_handler invoked with argument: %s\n", (char *)arg);
  } else {
    printf("timer_handler invoked with no argument\n");
  }
}

void timer_callback(union sigval arg) { timer_handler(arg.sival_ptr); }

char *message = "hello timer...";

int main() {
  // struct sigevent *
  // evp = malloc(sizeof(struct sigevent));
  memset(&evp, 0, sizeof(struct sigevent));
  //  timer_t *timer_id = malloc(sizeof(timer_t));
  memset(&timer_id, 0, sizeof(timer_t));
  struct itimerspec *ts = malloc(sizeof(struct itimerspec));
  memset(ts, 0, sizeof(struct itimerspec));

  evp.sigev_notify_function = timer_callback;
  evp.sigev_notify_attributes = NULL;
  // Address of an argument passed to the callback
  evp.sigev_value.sival_ptr = message;
  evp.sigev_notify = SIGEV_THREAD;

  ts->it_value.tv_sec = 5;
  ts->it_value.tv_nsec = 0;

  ts->it_interval.tv_sec = 5;
  ts->it_interval.tv_nsec = 0;

  if (timer_create(CLOCK_REALTIME, &evp, &timer_id) == -1) {
    fprintf(stderr, "failed to create a timer with error:  %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  }

  if (timer_settime(timer_id, 0, ts, NULL) == -1) {
    fprintf(stderr, "failed to start the timer with error:  %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  }

  pause();

  if (timer_delete(timer_id) == -1) {
    fprintf(stderr, "failed to delete a timer with error:  %s\n",
            strerror(errno));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}