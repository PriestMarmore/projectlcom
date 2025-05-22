#ifndef ENEMY_H
#define ENEMY_H

#include "graphics.h"

#define MAX_ENEMIES 10

typedef struct {
    int x, y;
    int alive;
    Sprite *sprite;
} Enemy;

void init_enemies();
void update_enemies();
void draw_enemies();
Enemy* get_enemies();

#endif // ENEMY_H
