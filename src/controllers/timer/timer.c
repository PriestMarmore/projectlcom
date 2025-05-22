#include "timer.h"

unsigned int global_counter = 0;
int hook_id = 1;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */

  if(timer > 2 || freq == 0){
    return 1; // Invalid
  }

  uint16_t divisor = TIMER_FREQ / freq;

  if(divisor > 0xFFFF){
    return 1; // Low Frequency
  }

  uint8_t lsb, msb;
  if(util_get_LSB(divisor, &lsb) || util_get_MSB(divisor, &msb)){
    return 1; // Error getting LSB or/and MSB
  }

  uint8_t control_word;
  if(timer_get_conf(timer, &control_word)){
    return 1; // Error reading the current configuration
  }

  // Let's now preserve the 4 last bits of the word
  uint8_t new_control_word = (control_word & 0x0F) | TIMER_LSB_MSB;

  // We'll select the timer based on the timer argument
  switch(timer){
    case 0:
      new_control_word |= TIMER_SEL0;
      break;
    case 1:
    new_control_word |= TIMER_SEL1;
    break;
    case 2:
    new_control_word |= TIMER_SEL2;
    break;
    default:
      return 1; // I don't think this is necessary but still
  }

  // Write new control word to the control register
  if(sys_outb(TIMER_CTRL, new_control_word)){
    return 1;
  }

  // Let's write both LSB and MSB of the divisor to the timer data port
  if(sys_outb(TIMER_0 + timer, lsb)){
    return 1; // Error writing LSB
  }
  if(sys_outb(TIMER_0 + timer, msb)){
    return 1; // Error writing MSB
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  // printf("%s is not yet implemented!\n", __func__);
  /*
  must call sys_irqsetpolicy() kernal call
  is IRQ_REENABLE so that the interrupt acknowledge the interrupt
  */

  if(bit_no == NULL){
    return 1;
  }

  hook_id = *bit_no;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK){
    return 1;
  }

  *bit_no = BIT(*bit_no);
  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  // printf("%s is not yet implemented!\n", __func__);

  /*
  Call sys_irqrmpolicy()
  */

  if(sys_irqrmpolicy(&hook_id) != OK){
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  // printf("%s is not yet implemented!\n", __func__);

  global_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */

  if(timer > 2){
    return 1;
  }

  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_STATUS_;

  if(sys_outb(TIMER_CTRL, cmd)){
    return 1;
  }

  return util_sys_inb(TIMER_0 + timer, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */

  union timer_status_field_val conf;

  switch(field){

    case tsf_all:
      conf.byte = st;
      break;

    case tsf_initial:
      if((st & TIMER_LSB_MSB) == 0){
        conf.in_mode = INVAL_val;
      }
      else if((st & (BIT(4) | BIT(5))) == BIT(4)){
        conf.in_mode = LSB_only;
      }
      else if((st & (BIT(4) | BIT(5))) == BIT(5)){
        conf.in_mode = MSB_only;
      }
      else{
        conf.in_mode = MSB_after_LSB;
      }
      break;

    case tsf_mode:
      conf.count_mode = (st >> 1) & 0x07; // We're extracting the bits 1-3 using a mask
      break;
  
    case tsf_base:
      conf.bcd = st & TIMER_BCD;
      break;

    default:
      return 1; // Invalid
  }

  return timer_print_config(timer, field, conf);  // LCOM function
}
