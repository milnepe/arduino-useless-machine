/*
  Arduino Useless Machine

  Crazy machine that appears to switch itself off

  Inspired by Arvid Mortensen's brilliant machine
  http://www.lamja.com/?p=451

  Power saving from code by Nick Gammon
  http://www.gammon.com.au/power

  1 April 2016
  Pete Milne
  Version 0.1

  Copywrite 2016 Peter Milne
  Released under GNU GENERAL PUBLIC LICENSE
  Version 3, 29 June 2007
*/
#include <avr/sleep.h>
#include <Servo.h>

const byte ledPin = 13; // LED D13
const byte mosfetPin = 12; // MOSFET gate D12
const byte armServoPin = 10; // servo2 control D10
const byte lidServoPin = 9; // servo1 control D9
const byte switchPin = 2; // interrupt D2

// Lid servo
Servo lidServo; // servo objects
const int lid = 1; // identifies lid in sequences
// presets for lid - adjust as required
const int lidClosed = 1400; // start position
const int lidInt1 = 1650;  // intermediate positions
const int lidInt2 = 1750;
const int lidInt3 = 1800;
const int lidOpen = 1850;  // stop

// Arm servo
Servo armServo;
const int arm = 2; // identifies arm in sequences
// presets for arm - adjust as required
//const int armIn = 1784;
const int armIn = 1900;
const int armInt1 = 1200;
const int armInt2 = 950;   // Almost on the switch
const int armOut = 770;

int seq = 0;  // store sequence number

void setup() {
  pinMode(switchPin, INPUT_PULLUP); // enable the internal pull-up resistor
  pinMode(ledPin, OUTPUT);
  pinMode(mosfetPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED off
  digitalWrite(mosfetPin, LOW); // MOSFET off
  lidServo.attach(lidServoPin);  // attaches the servo on D9 to the servo object
  armServo.attach(armServoPin);

  // disable ADC
  ADCSRA = 0;

  goToSleep();
}

void loop() {
  turnSwitchOff();
  goToSleep();
}

void wake(){
  sleep_disable();
  detachInterrupt (0);
}

// Put processor in deep sleep mode until ISR wakes it
void goToSleep() {

  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Do not interrupt before entering sleep mode, or 
  // ISR will detach interrupts and will not wake processor
  noInterrupts ();

  // Called when pin D2 goes low
  attachInterrupt (0, wake, LOW);
  EIFR = bit (INTF0);  // clear flag for interrupt 0

  // sleep_cpu call guaranteed to be done
  // as the processor executes instruction after
  // interrupts are turned on.
  interrupts ();  // one cycle
  sleep_cpu ();   // one cycle
}

// Simplified sweep function
void Sweep(int servo, int startPos, int stopPos, int msec) {
  int pos = 0;  // servo position
  if (startPos <= stopPos) {
    for (pos = startPos; pos < stopPos; pos += 1) {
      if (servo == 1)
        lidServo.writeMicroseconds(pos);
      else
        armServo.writeMicroseconds(pos);
    }
  }
  else {
    for (pos = startPos; pos >= stopPos; pos -= 1) {
      if (servo == 1)
        lidServo.writeMicroseconds(pos);
      else
        armServo.writeMicroseconds(pos);
    }
  }
  //  delayMicroseconds(usec);
  delay(msec);
}

// Controls sequences, called as soon as processor wakes
void turnSwitchOff() {
  digitalWrite(ledPin, HIGH); // LED on
  digitalWrite(mosfetPin, HIGH); // MOSFET on

  //seq = int(random(0,10)); //If you want random...

  if (seq == 0)Sequence0();
  if (seq == 1)Sequence2();
  if (seq == 2)Sequence0();
  if (seq == 3)Sequence9();
  if (seq == 4)Sequence3();
  if (seq == 5)Sequence5();
  if (seq == 6)Sequence0();
  if (seq == 7)Sequence7();
  if (seq == 8)Sequence0();
  if (seq == 9)Sequence2();
  if (seq == 10)Sequence0();
  if (seq == 11)Sequence10();
  if (seq == 12)Sequence0();
  if (seq == 13)Sequence8();
  if (seq == 14)Sequence0();
  if (seq == 15)Sequence6();
  if (seq == 16)Sequence3();
  if (seq == 17)Sequence4();
  seq++;
  if (seq > 17) seq = 0;

  delay(200);

  digitalWrite(mosfetPin, LOW); // MOSFET off
  digitalWrite(ledPin, LOW); // LED off
}

void Sequence0()
{
  delay(200);
  Sweep(lid, lidClosed, lidOpen, 100);
  Sweep(arm, armIn, armOut, 500);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 200);
}

void Sequence1()
{
  delay(200); // Test only
  Sweep(lid, lidClosed, lidOpen, 1000);
  Sweep(lid, lidOpen, lidClosed, 1000);
}

void Sequence2()
{
  delay(700);
  Sweep(lid, lidClosed, lidInt1, 1000);
  Sweep(lid, lidInt1, lidClosed, 1000);
  Sweep(lid, lidClosed, lidOpen, 500);
  Sweep(arm, armIn, armInt1, 500);
  Sweep(arm, armInt1, armOut, 500);
  Sweep(arm, armOut, armIn, 500);
  Sweep(lid, lidOpen, lidClosed, 200);
}

void Sequence3()
{
  delay(800);
  Sweep(lid, lidClosed, lidInt2, 300);
  Sweep(lid, lidInt2, lidInt3, 120);
  Sweep(lid, lidInt3, lidInt2, 120);
  Sweep(lid, lidInt2, lidInt3, 120);
  Sweep(lid, lidInt3, lidInt2, 120);
  Sweep(lid, lidInt2, lidInt3, 120);
  Sweep(lid, lidInt3, lidInt2, 120);
  Sweep(lid, lidInt2, lidClosed, 300);
  Sweep(lid, lidClosed, lidInt1, 1000);
  Sweep(lid, lidInt1, lidOpen, 1000);
  Sweep(arm, armIn, armInt1, 120);
  Sweep(arm, armInt1, armOut, 500);
  Sweep(arm, armOut, armIn, 500);
  Sweep(lid, lidOpen, lidClosed, 500);
}

void Sequence4()
{
  delay(500);
  Sweep(lid, lidClosed, lidOpen, 200);
  Sweep(arm, armIn, armInt2, 300);
  Sweep(arm, armInt2, armOut, 2000);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 400);
}

void Sequence5()
{
  delay(1000);
  Sweep(lid, lidClosed, lidOpen, 200);
  Sweep(arm, armIn, armOut, 450);
  Sweep(arm, armOut, armInt2, 110);
  Sweep(arm, armInt2, armOut, 110);
  Sweep(arm, armOut, armInt2, 110);
  Sweep(arm, armInt2, armOut, 110);
  Sweep(arm, armOut, armInt2, 110);
  Sweep(arm, armInt2, armOut, 110);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 400);
}

void Sequence6()
{
  delay(1500);
  Sweep(lid, lidClosed, lidOpen, 200);
  Sweep(lid, lidOpen, lidClosed, 1000);
  Sweep(lid, lidClosed, lidOpen, 2000);
  Sweep(arm, armIn, armOut, 500);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 400);
}

void Sequence7()
{
  delay(500);
  Sweep(lid, lidClosed, lidInt1, 200);
  Sweep(lid, lidInt1, lidInt2, 100);
  Sweep(lid, lidInt2, lidInt1, 100);
  Sweep(lid, lidInt1, lidInt2, 100);
  Sweep(lid, lidInt2, lidInt1, 100);
  Sweep(lid, lidInt1, lidClosed, 200);
  Sweep(lid, lidClosed, lidOpen, 100);
  Sweep(arm, armIn, armOut, 450);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 400);
}

void Sequence8()
{
  delay(200);
  Sweep(lid, lidClosed, lidInt1, 200);
  Sweep(lid, lidInt1, lidInt2, 100);
  Sweep(lid, lidInt2, lidInt1, 100);
  Sweep(lid, lidInt1, lidInt2, 100);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidClosed, 200);
  Sweep(lid, lidClosed, lidOpen, 200);
  Sweep(arm, armIn, armOut, 450);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 400);
}

void Sequence9()
{
  delay(1000);
  Sweep(lid, lidClosed, lidInt1, 500);
  Sweep(lid, lidInt1, lidInt2, 1000);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 50);
  Sweep(lid, lidInt2, lidInt3, 50);
  Sweep(lid, lidInt3, lidInt2, 500);
  Sweep(lid, lidInt2, lidInt1, 5000);
  Sweep(lid, lidInt1, lidOpen, 1000);
  Sweep(arm, armIn, armOut, 500);
  Sweep(arm, armOut, armIn, 200);
  Sweep(lid, lidOpen, lidClosed, 400);
}

void Sequence10()
{
  delay(800);
  Sweep(lid, lidClosed, lidOpen, 10000);
  Sweep(arm, armIn, armOut, 3000);
  Sweep(arm, armOut, armIn, 3000);
  Sweep(lid, lidOpen, lidInt1, 10000);
  Sweep(lid, lidInt1, lidClosed, 300);
}

