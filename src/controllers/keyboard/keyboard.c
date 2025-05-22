//  ---------------------------------------------------------------------------
//  keyboard.c ― Runtime driver-like logic for PS/2 keyboard handling
//  ---------------------------------------------------------------------------
//  The goal is to keep the rest of the project oblivious to any controller
//  idiosyncrasies — this module sits on top of the generic KBC helpers and
//  exposes a minimalist FIFO-style interface.
//  ---------------------------------------------------------------------------

#include "keyboard.h"      // public header for this module
#include "KBC.h"           // low-level i8042 wrappers
#include "i8042.h"
#include "i8254.h"

// Minix / LCF
#include <lcom/lcf.h>
#include <lcom/liblm.h>

// ──────────────────────────────────────────────────────────────────────────────
//                               INTERNAL STATE
// ──────────────────────────────────────────────────────────────────────────────
static int       hook_id  = KBD_IRQ;   ///< Value passed to sys_irqsetpolicy()
static uint8_t   buffer[2];            ///< Holds up to two bytes (extended code)
static uint8_t   buf_len  = 0;         ///< 0 → empty, 1 or 2 bytes stored
static bool      ready    = false;     ///< True once a *complete* scancode exists

// Wrapper for util_sys_inb() counter supplied by the LCF
extern uint32_t sys_inb_counter;

// ──────────────────────────────────────────────────────────────────────────────
//                               PUBLIC  API
// ──────────────────────────────────────────────────────────────────────────────

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (!bit_no) return 1;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

// -----------------------------------------------------------------------------

int (kbd_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&hook_id);
}

// -----------------------------------------------------------------------------
//  Re-enable keyboard interrupts after a polling test
// -----------------------------------------------------------------------------
#define READ_CMD_BYTE   0x20
#define WRITE_CMD_BYTE  0x60

int (kbd_restore)(void) {
  uint8_t cmd;
  if (write_KBC_command(KBD_IN_BUF_COM, READ_CMD_BYTE))         return 1;
  if (read_KBC_output (KBD_OUT_BUF, &cmd, false))          return 2;
  cmd |= BIT(0);                                             // turn INT on
  if (write_KBC_command(KBD_IN_BUF_COM, WRITE_CMD_BYTE))        return 3;
  if (write_KBC_command(KBD_OUT_BUF, cmd))                 return 4;
  return 0;
}

// -----------------------------------------------------------------------------
//  Interrupt context — assemble scancodes byte by byte
// -----------------------------------------------------------------------------
void (kbd_ih)(void) {
  uint8_t byte;
  if (read_KBC_output(KBD_OUT_BUF, &byte, false)) return;  // ignore on error

  if (ready) {                 // previous scancode consumed? reset buffer
    ready   = false;
    buf_len = 0;
  }

  if (byte == TWO_BYTE_PREFIX) {
    buffer[0] = byte;
    buf_len   = 1;             // wait for second byte
    return;
  }

  if (buf_len == 1) {
    buffer[1] = byte;          // completes the 0xE0 ** xx sequence
    buf_len   = 2;
  } else {
    buffer[0] = byte;          // single-byte make/break code
    buf_len   = 1;
  }
  ready = true;
}

// -----------------------------------------------------------------------------
//  Non-blocking checks from main loop
// -----------------------------------------------------------------------------

bool (kbd_has_scancode)(void) { return ready; }

// Fetches and clears the internal buffer --------------------------------------
void (kbd_get_scancode)(uint8_t *size, uint8_t bytes[2]) {
  if (!ready || !size || !bytes) return;                    // defensive guard

  for (uint8_t i = 0; i < buf_len; ++i) bytes[i] = buffer[i];
  *size = buf_len;

  // reset for next scancode
  ready   = false;
  buf_len = 0;
}

//  ---------------------------------------------------------------------------
//  End of file
//  ---------------------------------------------------------------------------
