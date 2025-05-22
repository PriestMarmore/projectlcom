#include <lcom/lcf.h>
#include "../controllers/keyboard/keyboard.h"
#include "../controllers/keyboard/i8042.h"
#include "../controllers/timer/timer.h"
#include "game_logic.h"

static bool running = true;

int game_main_loop() {
  int kbd_irq = keyboard_subscribe_int();
  int timer_irq = timer_subscribe_int();

  if (kbd_irq < 0 || timer_irq < 0) {
    printf("Failed to subscribe interrupts\n");
    return 1;
  }

  int irq_set_kbd = BIT(kbd_irq);
  int irq_set_timer = BIT(timer_irq);

  int ipc_status;
  message msg;
  int r;

  while (running) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_timer) {
            game_update();  // Update game logic on timer tick
          }

          if (msg.m_notify.interrupts & irq_set_kbd) {
            keyboard_ih();  // Read scancode
            if (scancode == BREAK_CODE) {
              running = false;
            }
          }

          break;
        default:
          break;
      }
    }
  }

  keyboard_unsubscribe_int();
  timer_unsubscribe_int();

  return 0;
}