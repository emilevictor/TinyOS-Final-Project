#ifndef TIMESYNC_H
#define TIMESYNC_H

#include "Timer.h"

enum {
  AM_TIMESYNC_MSG = 10
};

typedef nx_struct TimeSyncMsg
{
  nx_uint32_t globalTime;
  nx_uint32_t column;
} TimeSyncMsg;

#endif
