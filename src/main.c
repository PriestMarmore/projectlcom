/*! \mainpage Pawcalipse - A recreation of the game Space Invaders but with cats and dogs
 *
 * @date 22/05/2025
 */

#include <lcom/lcf.h>
#include <string.h>
#include "controllers/graphics/VBE.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/keyboard/i8042.h"
#include "game.h"
/*
#include "controllers/video/graphics.h"
#include "sprites/sprites.h"
#include "game/modes/drawing.h"
#include "game/modes/guessing.h"
#include "game/modes/inst.h"
#include "game/modes/menu.h"
#include "game/UTILS.h"
*/

int main() {
    if (game_init() != 0) {
        return 1;  // Failed to Initialize 
    }
    
    game_loop();
    
    game_cleanup();
    
    return 0;
}