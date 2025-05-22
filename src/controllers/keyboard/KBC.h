//  ---------------------------------------------------------------------------
//  KBC.h  ―  Auxiliary helpers for the i8042 keyboard controller
//  ---------------------------------------------------------------------------
//  This header exposes a *tiny* public API that hides all low‑level quirks of
//  talking to the keyboard controller (a.k.a. KBC or i8042).  The idea is to
//  offer well‑documented wrappers so that the remaining modules (keyboard,
//  mouse, and the lab3 test‑beds) never have to fiddle with status bits or
//  retry loops directly.
//
//  All routines return 0 on success and a non‑zero error code otherwise.
//  ---------------------------------------------------------------------------
#ifndef KBC_UTILS_H
#define KBC_UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Reads the controller's status register (0x64).
 *
 * @param status  Pointer where the status byte will be stored.
 * @return 0 on success, non‑zero if the underlying util_sys_inb fails.
 */
int (read_KBC_status)(uint8_t *status);

/**
 * @brief Retrieves one byte from the KBC output buffer.
 *
 * This helper blocks (with a bounded number of retries) until the OBF bit is
 * set, filters parity/timeout errors and checks whether the byte originates
 * from the expected source (keyboard or mouse).
 *
 * @param port   I/O port to read (usually 0x60).
 * @param byte   Pointer where the byte read will be placed.
 * @param mouse  true  → expect a mouse byte  (AUX bit == 1)
 *               false → expect a keyboard byte (AUX bit == 0)
 *
 * @return 0 on success, non‑zero on error (invalid source, parity/timeout, ...)
 */
int (read_KBC_output)(uint8_t port, uint8_t *byte, bool mouse);

/**
 * @brief Sends a command or data byte to the KBC.
 *
 * The function waits until the input buffer is free (IBF == 0) before writing.
 *
 * @param port  0x64 to issue a command, 0x60 to send its argument.
 * @param byte  The value to be written.
 *
 * @return 0 on success, non‑zero if an error occurs.
 */
int (write_KBC_command)(uint8_t port, uint8_t byte);

#endif /* KBC_UTILS_H */
