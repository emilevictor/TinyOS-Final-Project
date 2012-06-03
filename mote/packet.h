/*
* The packet definition header	
*/

#ifndef PACKET_H
#define PACKET_H

//mig -target=null -java-classname=RobotMessaging java ../packet.h packet -o $@

//MODE 				nx_uint8_t: 0(auto) 1(manual)
//COMMAND 			nx_uint8_t: 0(forward) 1(clockwise) 2(backward) 3(anticlockwise) 4(stop)
//CMD_DURATION		nx_uint16_t: (number of milliseconds to perform command)
//ACK     			nx_uint8_t: 0(no-ack) 1(ack)
//REQ-INFO 			nx_uint8_t: 0(No info required) 1(info requested)
//CURRENT_X			nx_int8_t: (x co-ordinate (estimated) - calculated by mote, negative if sent by GUI)
//CURRENT_Y			nx_int8_t: (y co-ordinate (estimated) - calculated by mote, negative if sent by GUI)
//HIT_WALL			nx_uint8_t	(1 if wall just hit, 0 otherwise)

enum {
  /* Number of readings per message. If you increase this, you may have to
     increase the message_t size. */
  //NREADINGS = 10,

  /* Default sampling period. */
  //DEFAULT_INTERVAL = 256,

  AM_PACKET = 0x93
};

typedef nx_struct packet {
	nx_uint8_t mode;
	nx_uint8_t cmd;
	nx_uint16_t cmd_duration;
	nx_uint8_t ack;
	nx_uint8_t req_info;
	nx_uint8_t current_x;
	nx_uint8_t current_y;
	nx_uint8_t hit_wall;
  	nx_uint32_t globalTime;
} packet_t;

#endif
