#ifndef __TIME_DEFINES
#define __TIME_DEFINES

#define TIME_UNIT	5 // milliseconds
#define REAL_TIME_UNIT	(TIME_UNIT/1000.0)

// tasks at 5ms
#define SPEED_PERIOD	3 // repeat each n quants of time
#define SPEED_JITTER	2 // wait (shift of) m quants of time to start

#define CONTROLLER_PERIOD	3
#define CONTROLLER_JITTER 0

#define SIMULATOR_PERIOD	3
#define SIMULATOR_JITTER 1

#endif
