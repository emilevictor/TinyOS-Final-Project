#include "Timer.h"
#include "TimeSync.h"
#include "printf.h"


module LedTimeSyncC
{
  uses {

    interface Boot;
    interface Leds;

    interface Timer<TMilli> as SendTimer;
    interface Timer<TMilli> as BlinkTimer;

    interface SplitControl as RadioControl;
    interface Receive;
    interface TimeSyncAMSend<TMilli,uint32_t>;
    interface TimeSyncPacket<TMilli,uint32_t>;
    interface LocalTime<TMilli>;


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

  int32_t clockOffset;

  message_t outgoingMessage;

  // define reference (root) node
  #define ROOT_ID 1

  // define timesync interval in MILLIseconds
  #define TIMESYNC_INTERVAL 1
  #define LEDDRV_INTERVAL 1024
  
  void updateClockOffset(uint32_t localTime, uint32_t globalTime)
  {
    // clock offset is defined as: offset = global - local
    clockOffset = globalTime - localTime;
    printf("offset: %ld, global: %ld, local%ld\n\n", clockOffset,globalTime,localTime);
    printfflush();
  }

  event void Boot.booted() {

    printf("Booted\n");
    printfflush();

    // start radio
    call RadioControl.start();

    // initial clock offset is zero, will be updated as soon as first timesync message has been received
    clockOffset = 0;

    // start timer to toggle LEDs
    call BlinkTimer.startOneShot(1024);

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
       // call Timer0.startPeriodic(SAMPLING_FREQUENCY);

  }
//TIMESYNC CODE BELOW =============================================================

  event void RadioControl.startDone(error_t error) {
       
    if (TOS_NODE_ID==ROOT_ID)
    {
      // start timer for time synchronization on root node
      call SendTimer.startPeriodic(1024*TIMESYNC_INTERVAL);
    }
  }

  event void BlinkTimer.fired() {

    uint32_t local, global, next, delta;

    // get current time
    local = call LocalTime.get();

    // TODO: calculate global time here (see updateClockOffset for definition of clockOffset)
    global = local + clockOffset;

    // set LEDs to the lower 3 bits of the seconds counter
    call Leds.set((global >> 10) & (0x07));

    // TODO: calculate time of next event here

    next = global + 1024 - (global % 1024);

    // calculate delta until next event
    delta = next - global;

    // start new timer for next event delta milliseconds relative to local
    call Timer0.startOneShotAt(local, delta);
	//call Timer0.startPeriodic(delta);

    printf("local: %lu, next: %lu, global: %lu, delta: %lu\n\n", local, next, global, delta);
    printfflush();


  }


  event void SendTimer.fired() 
  {
   
    TimeSyncMsg* msg = (TimeSyncMsg*)(call TimeSyncAMSend.getPayload(&outgoingMessage, sizeof(TimeSyncMsg)));

    // TODO: add current timestamp here
    uint32_t localTime = call LocalTime.get();

    // TODO: set globalTime field in TimeSyncMsg to the current localTime of reference node (see TimeSync.h)
	msg->globalTime = localTime;	

    // broadcast timesync message to single-hop neighbors
    if (call TimeSyncAMSend.send(AM_BROADCAST_ADDR, &outgoingMessage, sizeof(TimeSyncMsg), localTime)!=SUCCESS) {
      call Leds.set(1); // sets only red LED
    }
  }

  event message_t* Receive.receive(message_t* msg, void* payload, uint8_t len)
  {

    if (call TimeSyncPacket.isValid(msg))
    {
      uint32_t localTime = call TimeSyncPacket.eventTime(msg);
      uint32_t globalTime = ((TimeSyncMsg*)payload)->globalTime;
      
      updateClockOffset(localTime, globalTime);
    }

    return msg;
  }


  event void TimeSyncAMSend.sendDone(message_t* ptr, error_t error)
  {
    if (error!=SUCCESS) {
      call Leds.set(1); // sets only red LED
    }
  }

  event void RadioControl.stopDone(error_t error) {}
 


//LED DRIVING CODE ===========================================================================================


    event void Timer0.fired(){
        uint8_t i=0;
        uint16_t row=0;
        call nDSEL.clr();
        call BLANK.clr();

	    if(column == 0){
       	    call CSEL0.clr();
		    call CSEL1.clr();
		    call CSEL2.clr();
		/*			call Leds.led0Off();
					call Leds.led1Off();
					call Leds.led2Off();*/
		    column++;	    
	    }
	    else if(column == 1){  
	        call CSEL0.set();
		    call CSEL1.clr();
		    call CSEL2.clr();
		  /*  call Leds.led0On();
		    call Leds.led1Off();
		    call Leds.led2Off();	    */
            column++;		    
	    }
	    else if(column == 2){
             call CSEL0.clr();
		    call CSEL1.set();
		    call CSEL2.clr();
		 /*   call Leds.led0Off();
		    call Leds.led1On();
		    call Leds.led2Off();		    */
            column++;		    
	    } 
	    else if(column == 3){
            call CSEL0.set();
		    call CSEL1.set();
		    call CSEL2.clr();
	/*	    call Leds.led0On();
		    call Leds.led1On();
		    call Leds.led2Off();		    */
            column++;		    
	    }
	    else if(column == 4){  
	        call CSEL0.clr();
		    call CSEL1.clr();
		    call CSEL2.set();
		 /*   call Leds.led0Off();
		    call Leds.led1Off();
		    call Leds.led2On();		    */
            column++;		    
	    }
	    else if(column == 5){
            call CSEL0.set();
		    call CSEL1.clr();
		    call CSEL2.set();
		 /*   call Leds.led0On();
		    call Leds.led1Off();
		    call Leds.led2On();		    */
            column++;		    
	    } 
	    else if(column == 6){
            call CSEL0.clr();
		    call CSEL1.set();
		    call CSEL2.set();
		 /*   call Leds.led0Off();
		    call Leds.led1On();
		    call Leds.led2On();		    */
            column++;		    
	    }   
	    else if(column == 7){
            call CSEL0.set();
		    call CSEL1.set();
		    call CSEL2.set();
		/*    call Leds.led0On();
		    call Leds.led1On();
		    call Leds.led2On();		    */
            column=0;
	    }	 
  
        call SIN.set();                                                                                                                
        for (i = 0; i < 15; i++){
            if (i == 15){
                call nLATCH.set();
                call SCK.set();
                call SCK.clr();
                call nLATCH.clr();                        
            }
            else{
                call SCK.set();
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

