#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "allegro_init.h"

static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_EVENT_QUEUE *queue = NULL;
static ALLEGRO_DISPLAY *display = NULL;

static void (*game_loop_pointer)(void);

int allegro_init(void)
{
    al_init();
    al_install_keyboard();
    al_init_image_addon();

    timer = al_create_timer(1.0 / 60.0);
    queue = al_create_event_queue();

    display = al_create_display(320, 200);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    return 0;
}

void allegro_game_loop(void)
{
    bool redraw = true;
    bool game_break = false;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1) {
        while (!al_event_queue_is_empty(queue)) {
            al_wait_for_event(queue, &event);
            if (event.type == ALLEGRO_EVENT_TIMER)
                redraw = true;
            else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                game_break = true;
        }

        if (game_break)
            break;

        if (redraw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            game_loop_pointer();
            al_flip_display();

            redraw = false;
        }
    }
}

void allegro_destroy(void)
{
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

void allegro_set_game_loop(void (*loop)(void))
{
    game_loop_pointer = loop;
}
