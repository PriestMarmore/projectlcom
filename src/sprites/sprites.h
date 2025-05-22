#ifndef SPRITES_H
#define SPRITES_H

#include "graphics.h"  // For Sprite and xpm_map_t types

// Initialize and load all game sprites
int sprites_init();

// Free sprites and cleanup
void sprites_cleanup();

// Getters for the main sprites
Sprite* get_dog_sprite();
Sprite* get_cat_sprite();
Sprite* get_bullet_sprite();

#endif // SPRITES_H
