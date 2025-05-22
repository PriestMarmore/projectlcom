#include "player.h"
#include "sprites.h"

#define PLAYER_START_X 300
#define PLAYER_START_Y 400
#define PLAYER_SPEED   5

static Player player;
extern int key_pressed;

void init_player() {
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    player.sprite = get_dog_sprite();
}

void move_player_left() {
    player.x -= PLAYER_SPEED;
    if (player.x < 0) player.x = 0;
}

void move_player_right() {
    player.x += PLAYER_SPEED;
    if (player.x > get_hres() - player.sprite->width)
        player.x = get_hres() - player.sprite->width;
}

Bullet* shoot_bullet() {
    extern Bullet bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = player.x + player.sprite->width / 2;
            bullets[i].y = player.y;
            bullets[i].active = 1;
            return &bullets[i];
        }
    }
    return NULL;
}

Player* get_player() {
    return &player;
}

void update_player() {
    Player *p = get_player();

    // Move left or right based on current keys pressed
    if (key_pressed == KEY_LEFT) {
        move_player_left();
    } else if (key_pressed == KEY_RIGHT) {
        move_player_right();
    }

    // Shoot if space pressed
    if (key_pressed == KEY_SPACE) {
        shoot_bullet();
        // Optional: implement cooldown so bullets aren't spammed every frame
    }
}