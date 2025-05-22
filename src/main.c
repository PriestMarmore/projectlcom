/*! \mainpage Pawcalipse - A recreation of the game Space Invaders but with cats and dogs
 *
 * @date 22/05/2025
 */

#include <lcom/lcf.h>
#include <string.h>
#include "controllers/graphics/VBE.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/keyboard/i8042.h"
#include "game/game.h"
/*
#include "controllers/video/graphics.h"
#include "sprites/sprites.h"
#include "game/modes/drawing.h"
#include "game/modes/guessing.h"
#include "game/modes/inst.h"
#include "game/modes/menu.h"
#include "game/UTILS.h"
*/

int main(int argc, char *argv[]) {
  // Disable LCF logging to file if you prefer
  lcf_set_log_file("/dev/null");

  // Parse LCF arguments and start MINIX services
  lcf_start(argc, argv);

  // Main game loop
  game_main_loop();

  return 0;
}