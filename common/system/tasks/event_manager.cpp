#include "event_manager.h"

static bool eventStatus[Event_e::NUM_EVENT];
void eventManager_init() {
  for (bool& b : eventStatus) {
    b = false;
  }
}

void eventManager_setEventComplete(Event_e event) { eventStatus[event] = true; }

void eventManager_setEventIncomplete(Event_e event) {
  eventStatus[event] = false;
}

bool eventManager_getEventStatus(Event_e event) { return eventStatus[event]; }
