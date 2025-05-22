#include "bullet.h"
#include "enemy.h"
#include "player.h"

void check_bullet_collisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].alive) continue;

            // Simple bounding box collision detection
            if (bullets[i].x >= enemies[j].x &&
                bullets[i].x <= enemies[j].x + enemies[j].sprite->width &&
                bullets[i].y >= enemies[j].y &&
                bullets[i].y <= enemies[j].y + enemies[j].sprite->height) {
                    
                enemies[j].alive = 0;
                bullets[i].active = 0;
                // Optional: increment score here
                break;
            }
        }
    }
}
