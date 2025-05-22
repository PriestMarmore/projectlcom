#include "bullet.h"
#include "enemy.h"
#include "graphics.h"

Bullet bullets[MAX_BULLETS];

void init_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        bullets[i].y -= BULLET_SPEED;
        if (bullets[i].y < 0) {
            bullets[i].active = 0;  // Deactivate bullet
        }
    }
}

void draw_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            draw_rect(bullets[i].x, bullets[i].y, 3, 6, 0xFFFF);  // white bullet
        }
    }
}

void check_bullet_collisions() {
    Enemy* enemies = get_enemies();
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].alive) continue;

            int bx = bullets[i].x;
            int by = bullets[i].y;
            int ex = enemies[j].x;
            int ey = enemies[j].y;
            int ew = enemies[j].sprite->width;
            int eh = enemies[j].sprite->height;

            if (bx >= ex && bx <= ex + ew && by >= ey && by <= ey + eh) {
                enemies[j].alive = 0;
                bullets[i].active = 0;
                break;
            }
        }
    }
}
