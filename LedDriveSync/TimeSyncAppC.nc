/**
 * 
 * TimeSync demo application using active messages.
 * 
 * @author Philipp Sommer (CSIRO ICT Centre)
 */

#include "TimeSync.h"

configuration TimeSyncAppC 
{ 
} 
implementation { 
  
  components TimeSyncC, MainC, LedsC, new TimerMilliC() as SendTimerMilli, new TimerMilliC() as BlinkTimerMilli;

  TimeSyncC.Boot -> MainC;
  TimeSyncC.Leds -> LedsC;
  TimeSyncC.SendTimer -> SendTimerMilli;
  TimeSyncC.BlinkTimer -> BlinkTimerMilli;

  components LocalTimeMilliC;
  TimeSyncC.LocalTime -> LocalTimeMilliC;

  components TimeSyncMessageC;
  TimeSyncC.RadioControl -> TimeSyncMessageC;
  TimeSyncC.TimeSyncAMSend -> TimeSyncMessageC.TimeSyncAMSendMilli[AM_TIMESYNC_MSG];
  TimeSyncC.Receive -> TimeSyncMessageC.Receive[AM_TIMESYNC_MSG];
  TimeSyncC.TimeSyncPacket -> TimeSyncMessageC.TimeSyncPacketMilli;

  components PrintfC, SerialStartC;

}

