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

  components  new TimerMilliC() as Timer0, new TimerMilliC() as Timer1, new TimerMilliC() as Timer2, HplAtm128GeneralIOC;


    LedTimeSyncC.Timer0 -> Timer0;
    LedTimeSyncC.Timer1 -> Timer1;
    LedTimeSyncC.Timer2 -> Timer1;    
    LedTimeSyncC.CSEL1 -> HplAtm128GeneralIOC.PortC0;
    LedTimeSyncC.CSEL0 -> HplAtm128GeneralIOC.PortC1;
    LedTimeSyncC.SCK -> HplAtm128GeneralIOC.PortC2;
    LedTimeSyncC.SIN -> HplAtm128GeneralIOC.PortC3;
    LedTimeSyncC.nLATCH -> HplAtm128GeneralIOC.PortC4;
    LedTimeSyncC.BLANK -> HplAtm128GeneralIOC.PortC5;
    LedTimeSyncC.CSEL2 -> HplAtm128GeneralIOC.PortC6;
    LedTimeSyncC.nDSEL -> HplAtm128GeneralIOC.PortC7;

}

