	
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
    interface Timer<TMilli> as Timer2;     
    interface GeneralIO as CSEL1;
    interface GeneralIO as CSEL0;
    interface GeneralIO as SCK;
    interface GeneralIO as SIN;
    interface GeneralIO as nLATCH;
    interface GeneralIO as BLANK;
    interface GeneralIO as CSEL2;
    interface GeneralIO as nDSEL;
  }
}
implementation
{
    uint16_t column;
    uint16_t rowCount;

    #define SAMPLING_FREQUENCY 500
    #define LED_ON 200
    #define SCK_PERIOD 1
  
    event void Boot.booted() {
        column = 0;
        rowCount = 0;
        //configure pins as outputs
        call CSEL1.makeOutput();
        call CSEL0.makeOutput();
        call SCK.makeOutput();
        call SIN.makeOutput();
        call nLATCH.makeOutput();
        call BLANK.makeOutput();
        call CSEL2.makeOutput();
        call nDSEL.makeOutput();
        call Timer0.startPeriodic(SAMPLING_FREQUENCY);
    }

    event void Timer0.fired(){
        uint8_t i=0;
        uint16_t row=0;
        call nDSEL.clr();
        call BLANK.clr();
        
        /*if (rowCount == 0){
            row = 0x0001;
        }
        else if (rowCount == 1){
            row = 0x0002;           
        }
        else if (rowCount == 2){
            row = 0x0004;                   
        }
        else if (rowCount == 3){
            row = 0x0008;                    
        }
        else if (rowCount == 4){
            row = 0x0010;                   
        }
        else if (rowCount == 5){
            row = 0x0020;                  
        }
        else if (rowCount == 6){
            row = 0x0040;                  
        }
        else if (rowCount == 7){
            row = 0x0080;                   
        }
        else if (rowCount == 8){
            row = 0x0100;                  
        }
        else if (rowCount == 9){
            row = 0x0200;                   
        }
        else if (rowCount == 10){
            row = 0x0400;               
        }
        else if (rowCount == 11){
            row = 0x0800;                              
        }
        else if (rowCount == 12){
            row = 0x1000;                 
        }
        else if (rowCount == 13){
            row = 0x2000;                   
        }
        else if (rowCount == 14){
            row = 0x4000;
        }  */      
	    
	    if(column == 0){
       	    call CSEL0.clr();
		    call CSEL1.clr();
		    call CSEL2.clr();
		    call Leds.led0Off();
		    call Leds.led1Off();
		    call Leds.led2Off();
		    column++;	    
	    }
	    else if(column == 1){  
	        call CSEL0.set();
		    call CSEL1.clr();
		    call CSEL2.clr();
		    call Leds.led0On();
		    call Leds.led1Off();
		    call Leds.led2Off();	    
            column++;		    
	    }
	    else if(column == 2){
             call CSEL0.clr();
		    call CSEL1.set();
		    call CSEL2.clr();
		    call Leds.led0Off();
		    call Leds.led1On();
		    call Leds.led2Off();		    
            column++;		    
	    } 
	    else if(column == 3){
            call CSEL0.set();
		    call CSEL1.set();
		    call CSEL2.clr();
		    call Leds.led0On();
		    call Leds.led1On();
		    call Leds.led2Off();		    
            column++;		    
	    }
	    else if(column == 4){  
	        call CSEL0.clr();
		    call CSEL1.clr();
		    call CSEL2.set();
		    call Leds.led0Off();
		    call Leds.led1Off();
		    call Leds.led2On();		    
            column++;		    
	    }
	    else if(column == 5){
            call CSEL0.set();
		    call CSEL1.clr();
		    call CSEL2.set();
		    call Leds.led0On();
		    call Leds.led1Off();
		    call Leds.led2On();		    
            column++;		    
	    } 
	    else if(column == 6){
            call CSEL0.clr();
		    call CSEL1.set();
		    call CSEL2.set();
		    call Leds.led0Off();
		    call Leds.led1On();
		    call Leds.led2On();		    
            column++;		    
	    }   
	    else if(column == 7){
            call CSEL0.set();
		    call CSEL1.set();
		    call CSEL2.set();
		    call Leds.led0On();
		    call Leds.led1On();
		    call Leds.led2On();		    
            column=0;
        	/*if (rowCount == 14){
        	    rowCount=0;
        	}
        	else{
        	    rowCount++;                  
        	}  */          		    
	    }	 
  
        call SIN.set();                                                                                                                
        for (i = 0; i < 15; i++){
            /*if (0x0001&(row>>i)){
                call SIN.set();
            }
            else{
                call SIN.clr();
            }*/
            if (i == 15){
                call nLATCH.set();
                call SCK.set();
               // call Timer2.startPeriodic(SCK_PERIOD);               
                call SCK.clr();
                call nLATCH.clr();                        
            }
            else{
                call SCK.set();
                //call Timer2.startPeriodic(SCK_PERIOD);                
                call SCK.clr();
            }
        }
        
        call Timer1.startPeriodic(LED_ON);	
        call SIN.clr();
        call Timer1.stop();	    
    }
  
    event void Timer1.fired(){
        call nDSEL.set();          
        call BLANK.set();      
    }
    event void Timer2.fired(){
        uint8_t i=0;
        for (i = 0; i < 4; i++){
            //do nothing
        }     
    }    
}

