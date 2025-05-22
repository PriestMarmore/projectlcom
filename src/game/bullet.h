#ifndef BULLET_H
#define BULLET_H

#define MAX_BULLETS 5

typedef struct {
    int x, y;
    int active;
} Bullet;

void init_bullets();
void update_bullets();
void draw_bullets();
void check_bullet_collisions();

#endif // BULLET_H
