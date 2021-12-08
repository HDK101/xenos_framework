#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

int allegro_init(void);
void allegro_game_loop(void);
void allegro_destroy(void);
void allegro_set_game_loop(void (*loop)(void));

#endif
