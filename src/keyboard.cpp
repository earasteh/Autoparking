#include <iostream>

#include "keyboard.h"

std::atomic<bool> should_stop = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_reset = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_train = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_store_cache = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_load_cache = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_change_map = ATOMIC_VAR_INIT(false);
std::atomic<bool> paused = ATOMIC_VAR_INIT(false);
std::atomic<unsigned int> current_map = ATOMIC_VAR_INIT(0);

ALLEGRO_EVENT_QUEUE *event_queue = NULL;

bool start_keyboard() {

	if (!al_install_keyboard())
		return false;

	event_queue = al_create_event_queue();
	if(!event_queue)
		return false;

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	return true;
}


void stop_keyboard() {

	al_uninstall_keyboard();
}


void flush_other_commands() {

	ALLEGRO_EVENT ev;
	while (al_get_next_event(event_queue, &ev));
}


void poll_commands() {

	ALLEGRO_EVENT ev;

	while (al_get_next_event(event_queue, &ev)) {

		if (ev.type != ALLEGRO_EVENT_KEY_DOWN)
			continue;	// skip non-keydown events

		if (paused && ev.keyboard.keycode != ALLEGRO_KEY_P)
			continue;	// accept only resume request while paused

		if (should_train && ev.keyboard.keycode != ALLEGRO_KEY_T)
			continue;	// accept only resume request while paused

		switch(ev.keyboard.keycode) {
			case ALLEGRO_KEY_P:
				paused = !paused;
				flush_other_commands();
				break;
			case ALLEGRO_KEY_M:
			    should_change_map = true;
			    flush_other_commands();
				break;
			case ALLEGRO_KEY_Q:
				should_stop = true;
				flush_other_commands();
				break;
			case ALLEGRO_KEY_R:
				should_reset = true;
				flush_other_commands();
				break;
			case ALLEGRO_KEY_T:
				should_train = !should_train;
				flush_other_commands();
				break;
			case ALLEGRO_KEY_S:
				should_store_cache = true;
				flush_other_commands();
				break;
			case ALLEGRO_KEY_L:
				should_load_cache = true;
				flush_other_commands();
				break;
			default:
				break;
        }
	}
}