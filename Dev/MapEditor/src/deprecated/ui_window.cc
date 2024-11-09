#include "ui_window.h"


void UI_Window::draw()
{
	for(auto &item : items) {
		item->draw();
    }
}

void UI_Window::handle_events(SDL_Event *event)
{
	bool event_handled = false;

	if (items.size() < 1) {
		return;
	}

	// iterate from last to first
	// (items are added at vector back, so start with
	// last added)
	for (auto item = items.end() - 1; ; item--) {
		// pass event to window item
		event_handled = event_handled || (*item)->handle_events(event);
		// if event accepted -> stop
		if (event_handled || (item == items.begin())) {
			break;
		}
	}
}


void UI_Window::add_item(UI_Item *target)
{
	items.push_back(target);
}
