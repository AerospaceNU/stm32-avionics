#ifndef SYSTEM_TASKS_EVENT_MANAGER_H_
#define SYSTEM_TASKS_EVENT_MANAGER_H_
/**
 * The event manager manages the recording and timing of events that happen
 * during flight. These events can be a state transition (as is true of launch,
 * apogee, and touchdown, but can also represent an event like burnout that does
 * not have a state transition but is still useful to record globally.
 */
typedef enum { launch, burnout, apogee, touchdown, NUM_EVENT } Event_e;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the event manager.
 */
void eventManager_init();

/**
 * Mark an event as complete. This will use the current timestamp to store as
 * the timestamp of the event.
 * @param event Nne of the possible event enum values to mark as completed.
 */
void eventManager_setEventComplete(Event_e event);

/**
 * Query the status of an event.
 * @param event One of the possible event enum values to get the status of.
 * @return boolean status, true if that event has happened, false if not.
 */
bool eventManager_getEventStatus(Event_e event);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_EVENT_MANAGER_H_