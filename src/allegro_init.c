#include <stdbool.h>
#include <allegro5/allegro5.h>

#include "allegro_init.h"

static ALLEGRO_TIMER* timer = NULL;
static ALLEGRO_EVENT_QUEUE* queue = NULL;
static ALLEGRO_DISPLAY* display = NULL;

static void (*game_loop_pointer)(void);

int allegro_init(void) {
    al_init();
    al_install_keyboard();

    timer = al_create_timer(1.0 / 30.0);
    queue = al_create_event_queue();

    display = al_create_display(320, 200);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void allegro_set_game_loop(void (*loop)(void)) {
    game_loop_pointer = loop;
}
