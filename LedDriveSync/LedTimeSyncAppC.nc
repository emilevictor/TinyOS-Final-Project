#include "TimeSync.h"

configuration LedTimeSyncAppC 
{ 
} 
implementation { 
  
  components LedTimeSyncC, MainC, LedsC, new TimerMilliC() as SendTimerMilli, new TimerMilliC() as BlinkTimerMilli;

  LedTimeSyncC.Boot -> MainC;
  LedTimeSyncC.Leds -> LedsC;
  LedTimeSyncC.SendTimer -> SendTimerMilli;
  LedTimeSyncC.BlinkTimer -> BlinkTimerMilli;

  components LocalTimeMilliC;
  LedTimeSyncC.LocalTime -> LocalTimeMilliC;

  components TimeSyncMessageC;
  LedTimeSyncC.RadioControl -> TimeSyncMessageC;
  LedTimeSyncC.TimeSyncAMSend -> TimeSyncMessageC.TimeSyncAMSendMilli[AM_TIMESYNC_MSG];
  LedTimeSyncC.Receive -> TimeSyncMessageC.Receive[AM_TIMESYNC_MSG];
  LedTimeSyncC.TimeSyncPacket -> TimeSyncMessageC.TimeSyncPacketMilli;

  components PrintfC, SerialStartC;

//LED DRive

  components  new TimerMilliC() as BoardBlinkTimer, HplAtm128GeneralIOC;


    LedTimeSyncC.BoardBlinkTimer -> BoardBlinkTimer;

    LedTimeSyncC.interruptTrigger -> HplAtm128GeneralIOC.PortC7;


}

