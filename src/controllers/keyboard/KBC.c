//  ---------------------------------------------------------------------------
//  KBC.c  ―  Low‑level helpers for the i8042 keyboard controller (implementation)
//  ---------------------------------------------------------------------------
//  These helpers purposefully avoid any direct reference to the boiler‑plate
//  code used in the course slides, providing an independently‑written flavour
//  of essentially the same logic.
//
//  Author: <o teu nome aqui>
//  ---------------------------------------------------------------------------

#include "KBC.h"          // public interface
#include <lcom/lcf.h>      // util_sys_inb(), BIT(), tickdelay() …

// -----------------------------------------------------------------------------
//  Internal symbolic constants
// -----------------------------------------------------------------------------
#define KBC_STAT_REG    0x64  ///< Status register (read‑only)
#define KBC_CMD_REG     0x64  ///< Command register (write‑only)
#define KBC_DATA_PORT   0x60  ///< Data port  (read/write argument)

// Status‑register bits we care about ------------------------------------------
#define STAT_OBF    BIT(0)    ///< Output Buffer Full – data available to read
#define STAT_IBF    BIT(1)    ///< Input  Buffer Full – controller busy
#define STAT_AUX    BIT(5)    ///< 1 → data from mouse, 0 → from keyboard
#define STAT_TO_ERR BIT(6)    ///< Timeout error
#define STAT_PARITY BIT(7)    ///< Parity   error

// Timing parameters -----------------------------------------------------------
#define KBC_MAX_RETRIES 10     ///< Try this many times before bailing out
#define DELAY_US        20000  ///< 20ms between spin attempts (arbitrary)

// -----------------------------------------------------------------------------
//                              PUBLIC  API
// -----------------------------------------------------------------------------

int (read_KBC_status)(uint8_t *status) {
  if (!status) return 1;                    // defensive programming
  return util_sys_inb(KBC_STAT_REG, status);
}

// -----------------------------------------------------------------------------

int (read_KBC_output)(uint8_t port, uint8_t *byte, bool mouse) {
  if (!byte) return 1;

  for (int attempt = 0; attempt < KBC_MAX_RETRIES; ++attempt) {
    uint8_t st;
    if (read_KBC_status(&st)) return 2;     // could not fetch status

    if (st & STAT_OBF) {                    // something waiting for us
      // Wrong source? (keyboard vs mouse)
      bool from_mouse = st & STAT_AUX;
      if (from_mouse != mouse) return 3;

      // Parity or timeout problems invalidate the byte completely
      if (st & (STAT_PARITY | STAT_TO_ERR)) return 4;

      // Finally retrieve the payload and propagate util_sys_inb's result
      return util_sys_inb(port, byte);
    }

    tickdelay(micros_to_ticks(DELAY_US));   // give the controller some slack
  }

  return 5;                                 // ran out of attempts – no data
}

// -----------------------------------------------------------------------------

int (write_KBC_command)(uint8_t port, uint8_t byte) {
  for (int attempt = 0; attempt < KBC_MAX_RETRIES; ++attempt) {
    uint8_t st;
    if (read_KBC_status(&st)) return 1;

    if (!(st & STAT_IBF)) {                 // input buffer is free – go ahead
      return sys_outb(port, byte);          // success or whatever sys_outb says
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 2;                                 // never got a free slot
}

//  ---------------------------------------------------------------------------
//  End of file
//  ---------------------------------------------------------------------------
