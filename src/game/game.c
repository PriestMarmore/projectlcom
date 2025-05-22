#include "game.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "sprites.h"
#include "graphics.h"
#include "keyboard.h"
#include "timer.h"

extern int scancode; // from keyboard.c, updated via interrupts

static bool running = true;

int game_main_loop() {
    // Initialization if needed
    if (keyboard_subscribe_int() != 0) return 1;
    if (timer_subscribe_int() != 0) return 1;

    int ipc_status;
    message msg;
    int r, irq_set_kbd = BIT(KEYBOARD_IRQ), irq_set_timer = BIT(TIMER0_IRQ);

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // Keyboard input
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        keyboard_ih();

                        if (scancode == ESC_BREAK_CODE) {
                            running = false;
                        }

                        // Optionally store key state for game logic
                        update_key_state(scancode);
                    }

                    // Timer tick
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        game_update();  // Core logic for player, bullets, enemies
                        // draw_game_frame(); // TODO: implement when graphics ready
                    }

                    break;
            }
        }
    }

    // Cleanup
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    return 0;
}