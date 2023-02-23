#ifndef SYSTEM_TASKS_EVENT_MANAGER_H_
#define SYSTEM_TASKS_EVENT_MANAGER_H_

typedef enum { launch, burnout, apogee, touchdown, NUM_EVENT } Event_e;

#ifdef __cplusplus
extern "C" {
#endif

void eventManager_init();

void eventManager_setEventComplete(Event_e event);

bool eventManager_getEventStatus(Event_e event);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_EVENT_MANAGER_H_
