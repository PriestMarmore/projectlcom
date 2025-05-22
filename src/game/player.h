#ifndef PLAYER_H
#define PLAYER_H

#include "graphics.h"
#include "bullet.h"

typedef struct {
    int x, y;
    Sprite *sprite;
} Player;

void init_player();
void move_player_left();
void move_player_right();
Bullet* shoot_bullet();
Player* get_player();

#endif // PLAYER_H
