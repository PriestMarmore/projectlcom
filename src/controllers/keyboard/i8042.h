#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
#include <lcom/lcf.h>
#include <minix/sysutil.h>

// Generic Stuff
#define KBD_IRQ     1
#define DELAY       20000
#define WAIT_KBC    tickdelay(micros_to_ticks(DELAY_US))
#define MAX_TRIES   10

// I/O
#define KBD_STATUS_PORT     0x64
#define KBD_OUT_BUF         0x60
#define KBD_IN_BUF_COM      0x64
#define KBD_IN_BUF_ARG      0x60

// KBC
#define READ_CMD_BYTE       0x20
#define ENABLE_INTERRUPTS 1

// Masks
#define OUT_FULL_BUF	        BIT(0)
#define IN_FULL_BUF	            BIT(1)
#define TIME_OUT                BIT(6)
#define PARITY_ERROR            BIT(7)

// Scancodes
#define MAKE_CODE           BIT(7)
#define INC_BYTES           0xE0
#define BREAK_CODE          0x81

#endif /* _LCOM_I8042_H */
