#include "enemy.h"

static Enemy enemies[MAX_ENEMIES];
static int direction = 1;

void init_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = 50 + (i % 5) * 60;
        enemies[i].y = 50 + (i / 5) * 60;
        enemies[i].alive = 1;
        enemies[i].sprite = get_cat_sprite();
    }
}

void update_enemies() {
    int move_down = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;

        enemies[i].x += direction * ENEMY_SPEED;

        // Check if any enemy hits screen edge
        if (enemies[i].x <= 0 || enemies[i].x >= (get_hres() - enemies[i].sprite->width)) {
            move_down = 1;
        }
    }

    if (move_down) {
        direction = -direction;  // Reverse direction
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].alive)
                enemies[i].y += ENEMY_DROP;  // Move enemies down
        }
    }
}

void draw_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive)
            draw_sprite(enemies[i].sprite, enemies[i].x, enemies[i].y);
    }
}

Enemy* get_enemies() {
    return enemies;
}

