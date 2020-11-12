/*   Basic Gicar Box Replacement:
 *    Utilizes factory autofill probe (Magister ES40)
 *    Utilizes 4 Relay module (Heater, Pump, 2 Way solenoid, 3 Way solenoid)
 *    Utilizes Arduino pro mini
 *  
 *   USE AT YOUR ON RISK.
 *   I am posting this project for EDUCATIONAL USE ONLY.
 *   This project involves electricity, high heat, and water/steam.
 *   I will not be held liable damages and/or injuries resulting from the use of this code or from copying this project.
 *   
 *   This is a fork of:
 *   Andreja Premium Control Board Replacement 
 *   Written By Anthony DiPilato
 *   
 *   It has been simplified to have no buzzer or float switches for plumbed in models.
 *   
 */

// Configuration
#include "Configuration.h"

// Include Sensors class
#include "Sensors.h"

// Setup sensor pins
Sensors boiler(3);  //Autofill sensor 
Sensors shot(2);    //Brew shot button


// Status for heater valve and pump
int heat_state = 1;
int pump_state = 0; 
int valve_state = 0;
int twoway_state = 0;

long pump_time = 0;
long valve_time =0;
long twoway_time =0;

void setup() {
    Serial.begin(9600);
  
    // Output mode for relays
    pinMode(PUMP, OUTPUT);       
    pinMode(VALVE, OUTPUT);
    pinMode(HEAT, OUTPUT);
    pinMode(TWOWAY, OUTPUT);

    // Initialize relays
    // Pump and value off, Heat on
    // Heat is on N/O relay
    digitalWrite(PUMP, HIGH);
    digitalWrite(VALVE, HIGH);  
    digitalWrite(HEAT, HIGH);
    digitalWrite(TWOWAY, HIGH);
  
}

// Serial Output for debugging
void debug_output(){
    Serial.print("Boiler: ");
    Serial.println(boiler.value);
    Serial.print("Switch: ");
    Serial.println(shot.value);
    Serial.print("Pump state: ");
    Serial.println(pump_state);
    delay(250);
}
//each of the following classes control the relay states

void toggle_heat(int state){
    if(state == heat_state){
        return;
    }
    switch(state){
        case 0:
            digitalWrite(HEAT, LOW); // LOW is on, N/O relay
            break;
        case 1:
            digitalWrite(HEAT, HIGH);
            break;
    }
    heat_state = state;
    return;
}

void toggle_pump(int state){
  if(state == pump_state){
    return;
  }
  if(state == 0){
      if ((millis() - pump_time) > PUMP_DELAY) {
          // Turn off pump before valve
          digitalWrite(PUMP, HIGH); // High is off
          //digitalWrite(VALVE, HIGH);
          pump_state = state;
      }
  }else{
      // Turn on valve before pump
      //digitalWrite(VALVE, LOW);
      digitalWrite(PUMP, LOW);
      pump_state = state;
  }
  return;
}

void toggle_valve(int state){
  if(state == valve_state){
    return;
  }
  if(state == 0){
      if ((millis() - valve_time) > VALVE_DELAY) {
          // Turn off pump before valve
          //digitalWrite(PUMP, HIGH); // High is off
          digitalWrite(VALVE, HIGH);
          valve_state = state;
      }
  }else{
      // Turn on valve before pump
      digitalWrite(VALVE, LOW);
      //digitalWrite(PUMP, LOW);
      valve_state = state;
  }
  return;
}

void toggle_twowaysol(int state){
  if(state == twoway_state){
    return;
  }
  if(state == 0){
      if ((millis() - twoway_time) > TWOWAY_DELAY) {
          // Turn off pump before valve
          //digitalWrite(PUMP, HIGH); // High is off
          digitalWrite(TWOWAY, HIGH);
          twoway_state = state;
      }
  }else{
      // Turn on valve before pump
      digitalWrite(TWOWAY, LOW);
      //digitalWrite(PUMP, LOW);
      twoway_state = state;
  }
  return;
}


//In these functions the actual switching of the relays is happening.
void check_sensors(){
    // Read Sensors
    boiler.check();
    // If boiler water is low turn on pump
    switch(boiler.value){
      case 0:
        toggle_twowaysol(1);
        toggle_pump(1);
        toggle_heat(1);
        break;
      case 1:
        toggle_pump(0);
        toggle_twowaysol(0);
        toggle_heat(0);
        break;
    }    
}

void check_button(){
    // Read Button
    shot.check();
    // If button is pressed start dose
    switch(shot.value){
      case 0:
          if(TWOWAY==LOW);{
            toggle_valve(1);
            toggle_pump(1);
          }
        break;
      case 1:
        toggle_pump(0);
        toggle_valve(0);
        break;
    }    
}

// Main Loop
void loop() {
    // read sensors
    check_sensors();
    check_button();
    // Debug Output
    if(DEBUG == true){ debug_output();}
}
