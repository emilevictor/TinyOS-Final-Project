
configuration ButtonAppC 
{ 
} 
implementation { 
  
  components ButtonC, MainC, LedsC, new TimerMilliC() as Timer0, new TimerMilliC() as Timer1,	HplAtm128GeneralIOC;

  ButtonC.Boot -> MainC;
  ButtonC.Leds -> LedsC;
  ButtonC.Timer0 -> Timer0;
  ButtonC.Timer1 -> Timer1;
  ButtonC.CSEL1 -> HplAtm128GeneralIOC.PortC0;
  ButtonC.CSEL0 -> HplAtm128GeneralIOC.PortC1;
  ButtonC.SCK -> HplAtm128GeneralIOC.PortC2;
  ButtonC.SIN -> HplAtm128GeneralIOC.PortC3;
  ButtonC.nLATCH -> HplAtm128GeneralIOC.PortC4;
  ButtonC.Blank -> HplAtm128GeneralIOC.PortC5;
  ButtonC.CSEL2 -> HplAtm128GeneralIOC.PortC6;
  ButtonC.nDSEL -> HplAtm128GeneralIOC.PortC7;



}

