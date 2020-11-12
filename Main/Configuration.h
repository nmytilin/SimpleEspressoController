/*    Configuration
 *   
 *    Default configurations for Espresso Machine
 *  
 */

#ifndef Configuration_h
#define Configuration_h

 // Debug Settings
 #define DEBUG true
 #define DEBUG_DELAY 1000

// Setup Relay Pins
#define HEAT    10
#define PUMP    9
#define TWOWAY  8
#define VALVE   7 // Was pin 13 but is connected to led which blinks on startup triggering the relay


// Pump Delay
#define PUMP_DELAY 500
#define VALVE_DELAY 500
#define TWOWAY_DELAY 500


#endif
