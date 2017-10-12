/*
 * buttonISR.c
 *
 *  Created on: 
 *      Author: 
 */

#include "buttonISR.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "io.h"
#include "alt_types.h"
#include "lcd.h"
#include "HexDisplay.h"

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
	alt_u8 timer_setting;
    /* Cast context It is important that this be declared 
     * volatile to avoid unwanted compiler optimization.
     */
    alt_u32* pCount = (alt_u32*)context;
    /* Store the value in the PUSHBUTTONS's edge capture register in *context. */
    alt_u8 buttons = IORD (PUSHBUTTONS_BASE, 3);
    /* Reset the PUSHBUTTONS's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);

    /* Act upon the interrupt */
	switch (buttons) {

	case 2:
		timer_setting = IORD(INTERVAL_TIMER_BASE, 1);
		if (timer_setting&0x8) { // it is off
			timer_setting = 0x7; // turn it on 011b = 0x7
			LCD_cursor(0,0);
			LCD_text("Running ...            ");
		} else { // it is on
			timer_setting = 0xB; // turn it off 1011b = 0xB
			LCD_cursor(0,0);
			LCD_text("Paused ...            ");
		}
		IOWR(INTERVAL_TIMER_BASE, 1, timer_setting);
		break;
	case 4:
		*pCount=0;
		HexDisplay((alt_u32*) HEX3_HEX0_BASE, *pCount);
		LCD_cursor(0,1);
		LCD_text("Key 2 pressed         ");
		break;

	case 8:
		toggleBCDHEX();
		LCD_cursor(0,1);
		LCD_text("Key 3 pressed         ");
		break;
	default:
		break;
	}

    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
