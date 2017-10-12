
#include "io.h"
#include "system.h"
#include "alt_types.h"
#include "sys\alt_irq.h"
#include "timerISR.h"
#include "buttonISR.h"

#include "altera_avalon_pio_regs.h"


/*******************************************************************************
 * int main()                                                                  *
 *                                                                             *
 ******************************************************************************/

int main(void)
{ 
    /* Setup */
	alt_u32 * pCount;
	static alt_u32 count = 0;
	pCount = &count;

    /* Initialize Variables */

    /* Register ISRs */
	alt_irq_register(PUSHBUTTONS_IRQ, (void*)pCount, buttonISR);
	alt_irq_register(INTERVAL_TIMER_IRQ, (void*)pCount, timerISR);
    /* Initialize Timer */
	IOWR(INTERVAL_TIMER_BASE, 2, 0xFFF&INTERVAL_TIMER_FREQ);
	IOWR(INTERVAL_TIMER_BASE, 3, 0xFFF&(INTERVAL_TIMER_FREQ >>16));
	// Start the timer
	IOWR(INTERVAL_TIMER_BASE, 1, 0x7); // Stop = 0, Start = 1, Cont = 1, ITO = 1
	// Enable Pushbutton interrupts
	IOWR(PUSHBUTTONS_BASE, 2, 0xE);
    /* Loop while processing interrupts */
    while( 1 ) 
    {

    }
    return 0; // should never get here
}
