#ifndef _X86_TIMER_H_
#define _X86_TIMER_H_

/* Common definitions for the 8254 PIT - Programmable Interval Timer.  */
#define PIT_ADDR_C0     0x40  /* port address for counter 0 */
#define PIT_ADDR_C1     0x41  /*   "    "      "     "    1 */
#define PIT_ADDR_C2     0x42  /*   "    "      "     "    2 */
#define PIT_ADDR_MODE   0x43  /* port address of the mode
                                 control register */

/* The format of the mode control register (8 bits):
                                                                                
             
   bit 7 and 6:    counter select bits
   bit 5 and 4:    read/write/latch format bits
   bit 3, 2 and 1:   counter mode bits
   bit 0:      16 binary or 4 decade BCD counter
 */
#define PIT_COUNTER_C0        0x00  /* counter select bit for counter 0 */
#define PIT_COUNTER_C1        0x40  /* counter select bit for counter 1 */
#define PIT_COUNTER_C2        0x80  /* counter select bit for counter 2 */
#define PIT_COUNTER_READBACK  0xf0  /* counter select bit for readback
                                       (8254 only, not 8253) */
                                                                                
             
#define PIT_LATCH_PRESENT     0x00  /* latch present counter value */
#define PIT_LATCH_MSB         0x10  /* read/write of msb only */
#define PIT_LATCH_LSB         0x20  /* read/write of lsb only */
#define PIT_LATCH_BOTH        0x30  /* read/write lsb, followed by write
                                       of msb */

#define PIT_MODE_TERMINAL     0x00  /* mode 0: interrupt on terminal count */
#define PIT_MODE_ONESHOT      0x02  /* mode 1: programmable one-shot */
#define PIT_MODE_RAGE_GEN     0x04  /* mode 2: rate generator */
#define PIT_MODE_SQUARE_WAVE  0x06  /* mode 3: swuare wave rate generator */
#define PIT_MODE_SOFT_STROBE  0x08  /* mode 4: software triggered strobe */
#define PIT_MODE_HARD_STROBE  0x0a  /* mode 5: hardware triggered strobe */

#define PIT_MODE_BINARY       0x00  /* (16 bit) binary counter */
#define PIT_MODE_BCD          0x01  /* (4 decade) BCD counter */
                                                                                
                  
/* Devide this number with the speed of the timer and you get the latch that
   you send to the chip.  */
#define PIT_CLKNUM    1193167

void timer_init(void);

#endif
