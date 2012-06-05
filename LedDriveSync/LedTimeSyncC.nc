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


    interface Timer<TMilli> as BoardBlinkTimer;
    interface GeneralIO as interruptTrigger;
  }
}

implementation
{
    uint16_t column;
    uint16_t rowCount;
    uint32_t  oddOrEvenLED;

    #define SAMPLING_FREQUENCY 500
    #define LED_ON 200
    #define SCK_PERIOD 1

  int32_t clockOffset;

  message_t outgoingMessage;

  // define reference (root) node
  #define ROOT_ID 1

  // define timesync interval in MILLIseconds
  #define TIMESYNC_INTERVAL 18
  #define LEDDRV_INTERVAL 1024
  
  void updateClockOffset(uint32_t localTime, uint32_t globalTime)
  {
    // clock offset is defined as: offset = global - local
    clockOffset = globalTime - localTime;

    printf("offset: %ld, global: %ld, local%ld\n\n", clockOffset,globalTime,localTime);
    printfflush();
  }

  event void RadioControl.startDone(error_t error) {
   
    if (TOS_NODE_ID==ROOT_ID)
    {
      // start timer for time synchronization on root node
      call SendTimer.startPeriodic(320*TIMESYNC_INTERVAL);
    }
  }

  event void Boot.booted() {
    uint32_t counterValue;
    //printf("Booted\n");
    //printfflush();

    oddOrEvenLED = 1;

    // start radio
    call RadioControl.start();

    // initial clock offset is zero, will be updated as soon as first timesync message has been received
    clockOffset = 0;

    // start timer to toggle LEDs every second.
    call BlinkTimer.startPeriodic(550);

    /*if (TOS_NODE_ID==ROOT_ID)
    {
      // start timer for time synchronization on root node
      call BoardBlinkTimer.startPeriodic(320);
    }*/

    column = 0;
    rowCount = 0;
    //configure pins as outputs
    call interruptTrigger.makeOutput();
    call interruptTrigger.set();
   // call BoardBlinkTimer.startPeriodic(SAMPLING_FREQUENCY);

  }
//TIMESYNC CODE BELOW =============================================================


  /*
  * When we reach this function, both should be synced.
  */
  event void BlinkTimer.fired() {

    uint32_t local, global, next, delta;
    uint32_t counterValue;

    // get current time
    local = call LocalTime.get();

    // TODO: calculate global time here (see updateClockOffset for definition of clockOffset)
    global = local + clockOffset;

    // set LEDs to the lower 3 bits of the seconds counter
    call Leds.set(1);

    //call Leds.set((global >> 10) & (0x07));

    // TODO: calculate time of next event here

    next = global + 1650 - (global % 1650) + (550*((uint32_t)TOS_NODE_ID-1));

    // calculate delta until next event
    delta = next - global;

    call interruptTrigger.clr(); 
    call interruptTrigger.set();

    // start new timer for next event delta milliseconds relative to local
    call BlinkTimer.startOneShotAt(local, delta);
    call BoardBlinkTimer.startOneShotAt(local,550);
    //printf("Hello\n");



    //printf("local: %lu, next: %lu, global: %lu, delta: %lu\n\n", local, next, global, delta);
    //printfflush();


  }


  event void SendTimer.fired() 
  {
   
    TimeSyncMsg* msg = (TimeSyncMsg*)(call TimeSyncAMSend.getPayload(&outgoingMessage, sizeof(TimeSyncMsg)));

    // TODO: add current timestamp here
    uint32_t localTime = call LocalTime.get();

    // TODO: set globalTime field in TimeSyncMsg to the current localTime of reference node (see TimeSync.h)
    msg->globalTime = localTime;	
    msg->column = column;
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
      column = ((TimeSyncMsg*)payload)->column;
      
      updateClockOffset(localTime, globalTime);
      //call BoardBlinkTimer.startPeriodic(320);

    }

    return msg;
  }


  event void TimeSyncAMSend.sendDone(message_t* ptr, error_t error)
  {
    if (error!=SUCCESS) {
      call Leds.set(1); // sets only red LED
    }

    //call BoardBlinkTimer.startPeriodic(320);

  }

  event void RadioControl.stopDone(error_t error) {}
 


//LED DRIVING CODE ===========================================================================================


  event void BoardBlinkTimer.fired()
  {
    call Leds.set(0);
   //printf("does %d == %d\n",TOS_NODE_ID, (int)oddOrEvenLED);
    //printfflush();
   

  }
}

