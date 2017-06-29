#if !defined (_EVENT_H)
#   define _EVENT_H

#include<time.h>

struct event_s
{
  void (*action)(void *p);
  void *p;
  time_t expire;

  struct event_s *next;
};

void schedule_event(void (*a)(void *p),void *param,time_t *expt);
void init_schedule(void);
void finish_schedule(void);

#endif
