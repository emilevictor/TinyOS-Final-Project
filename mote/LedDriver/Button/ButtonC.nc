	
/**
 * 
 * Button demo application.
 *
 * @author Philipp Sommer
 */

#include "Timer.h"

module ButtonC
{
  uses {
    interface Boot;
    interface Leds;

    interface Timer<TMilli> as Timer0;
    interface Timer<TMilli> as Timer1;
    interface GeneralIO as CSEL1;
    interface GeneralIO as CSEL0;
    interface GeneralIO as SCK;
    interface GeneralIO as SIN;
    interface GeneralIO as nLATCH;
    interface GeneralIO as Blank;
    interface GeneralIO as CSEL2;
    interface GeneralIO as nDSEL;
  }
}
implementation
{
  uint16_t clkCount;
  uint16_t column;
  // sampling frequency in binary milliseconds
  #define SAMPLING_FREQUENCY 1000
  #define CLK_FREQ 5
  
  event void Boot.booted() {
    clkCount = 0;
    column = 0;
    // configure pin as input (INT pin on sensorboard)
    call CSEL1.makeOutput();
    call CSEL0.makeOutput();
    call SCK.makeOutput();
    call SIN.makeOutput();
    call nLATCH.makeOutput();
    call Blank.makeOutput();
    call CSEL2.makeOutput();
    call nDSEL.makeOutput();
    // enable pull-up resistor

    call Timer0.startPeriodic(SAMPLING_FREQUENCY);
    call Timer1.startPeriodic(CLK_FREQ);
  }

  event void Timer0.fired() 
  {
    call Leds.led1Toggle();
    call nDSEL.set();
   // call CSEL0.set(); 
    call Blank.clr();
    call SIN.set(); 
	if(column == 0) {
		 
				call CSEL0.clr();
				call CSEL1.clr();
				call CSEL2.clr();
		column++;
	}
		else if(column == 1) {  
				call	CSEL0.set();
				call	CSEL1.clr();
				call	CSEL2.clr();
		column++;
	}
		else if(column == 2) {
 
				call	CSEL0.clr();
				call	CSEL1.set();
				call	CSEL2.clr();
		column++;
	} else if(column == 3) {
		column = 0;
	}	

    // check status of input (button) and set/clear red LED accordingly
 /*   bool high = call GeneralIO.get();
    if (!high) call Leds.led0On();
    else call Leds.led0Off();
*/
    // toggle green LED
  }
  event void Timer1.fired()

  {
    call Leds.led2Toggle();
    if(clkCount < 32) {
		if(clkCount == 30) {

			call nLATCH.set();
		} else if (clkCount == 31) {
			call nLATCH.clr();
		}	
    	call SCK.toggle();
    	clkCount = clkCount +1;
    }else{
		clkCount = 0;
}	

  }
 

}

