//  ---------------------------------------------------------------------------
//  keyboard.h ― Public interface for the keyboard module
//  ---------------------------------------------------------------------------
//  This header exposes a *clean* API for dealing with PS/2 keyboard traffic
//  under Minix, decoupling the rest of the project from the low‑level i8042
//  quirks handled in KBC.c.
//
//  All routines return 0 on success unless stated otherwise.
//  ---------------------------------------------------------------------------
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>      // BIT(), bool, uintXX_t …
#include <stdbool.h>
#include <stdint.h>

// ──────────────────────────────────────────────────────────────────────────────
//                                  CONSTANTS
// ──────────────────────────────────────────────────────────────────────────────
#define ESC_BREAK         0x81   ///< Breakcode for the Esc key
#define TWO_BYTE_PREFIX   0xE0   ///< Prefix that announces an extended code

// ──────────────────────────────────────────────────────────────────────────────
//                              PUBLIC  API
// ──────────────────────────────────────────────────────────────────────────────

/**
 * @brief Subscribes keyboard interrupts.
 *
 * @param[out] bit_no  Bit position to be set in the IPC notification mask.
 *
 * @return 0 on success, non‑zero otherwise.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the keyboard interrupt policy established earlier.
 */
int (kbd_unsubscribe_int)(void);

/**
 * @brief Restores the KBC command byte so that keyboard interrupts are re‑enabled.
 *
 * Needed after a polling‑only routine, otherwise the OS driver remains deaf.
 */
int (kbd_restore)(void);

/**
 * @brief Interrupt Service Routine — **must** be named exactly like this for
 *        the lab3 tests.
 */
void (kbd_ih)(void);

/**
 * @brief Checks if the module has already assembled a complete scancode.
 *
 * This allows the main program to poll the module *without* busy‑waiting for
 * every single byte — handy for timer‑keyboard mixed tests.
 */
bool (kbd_has_scancode)(void);

/**
 * @brief Retrieves the most recent *complete* scancode byte sequence.
 *
 * Call only if `kbd_has_scancode()` returned true; the internal buffer is
 * reset afterwards.
 *
 * @param[out] size   Number of bytes that compose this scancode (1 or 2).
 * @param[out] bytes  Array where the bytes will be stored (must be ≥ 2).
 */
void (kbd_get_scancode)(uint8_t *size, uint8_t bytes[2]);

#endif /* KEYBOARD_H */
