#include "sprites.h"
#include "images/game/dog.xpm"
#include "images/game/cat.xpm"
#include "images/game/bullet.xpm"

static Sprite *dog_sprite = NULL;
static Sprite *cat_sprite = NULL;
static Sprite *bullet_sprite = NULL;

int sprites_init() {
    dog_sprite = load_sprite((xpm_map_t)dog_xpm);
    if (!dog_sprite) return 1;

    cat_sprite = load_sprite((xpm_map_t)cat_xpm);
    if (!cat_sprite) {
        free_sprite(dog_sprite);
        return 1;
    }

    bullet_sprite = load_sprite((xpm_map_t)bullet_xpm);
    if (!bullet_sprite) {
        free_sprite(dog_sprite);
        free_sprite(cat_sprite);
        return 1;
    }

    return 0;
}

void sprites_cleanup() {
    if (dog_sprite) free_sprite(dog_sprite);
    if (cat_sprite) free_sprite(cat_sprite);
    if (bullet_sprite) free_sprite(bullet_sprite);
}

Sprite* get_dog_sprite() {
    return dog_sprite;
}

Sprite* get_cat_sprite() {
    return cat_sprite;
}

Sprite* get_bullet_sprite() {
    return bullet_sprite;
}
