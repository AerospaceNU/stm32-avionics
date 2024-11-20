#ifndef COMMON_SYSTEM_TASKS_EVENT_MANAGER_H_
#define COMMON_SYSTEM_TASKS_EVENT_MANAGER_H_
/**
 * The event manager manages the recording and timing of events that happen
 * during flight. These events can be a state transition (as is true of launch,
 * apogee, and touchdown, but can also represent an event like burnout that does
 * not have a state transition but is still useful to record globally.
 */
enum class Event{
  launch,
  burnout,
  apogee,
  touchdown,
  unclean_restart,
  NUM_EVENT
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the event manager.
 */
void eventManager_init();

/**
 * Mark an event as complete.
 * @param event One of the possible event enum values to mark as completed.
 */
void eventManager_setEventComplete(Event event);

/**
 * Mark an event as imcomplete, as in no longer having happened.
 * @param event One of the possible event enum values to mark as incomplete.
 */
void eventManager_setEventIncomplete(Event event);

/**
 * Query the status of an event.
 * @param event One of the possible event enum values to get the status of.
 * @return boolean status, true if that event has happened, false if not.
 */
bool eventManager_getEventStatus(Event event);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_SYSTEM_TASKS_EVENT_MANAGER_H_
