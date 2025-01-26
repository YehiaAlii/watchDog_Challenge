#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <EEPROM.h>

// Define LED pins
const int ledPin1 = 6;
const int ledPin2 = 7;

// Define EEPROM addresses
const int EEPROM_EXECUTION_STATE_ADDR = 0;
const int EEPROM_INTERRUPT_FLAG_ADDR = 8; // Right after the execution state

volatile int executionState; // Track the state of execution
volatile bool interruptOccurred = false; // Flag to track if an interrupt occurred

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.println("Start of setup");

  // Read the interrupt flag from EEPROM as a byte
  byte eepromFlag = EEPROM.read(EEPROM_INTERRUPT_FLAG_ADDR);
  interruptOccurred = eepromFlag; // Convert byte to boolean

  if (interruptOccurred) {
    Serial.println("Recovery from interrupt");
    // Retrieve the last execution state from EEPROM
    EEPROM.get(EEPROM_EXECUTION_STATE_ADDR, executionState);
    // Clear the interrupt flag in EEPROM
    noInterrupts();
    EEPROM.update(EEPROM_INTERRUPT_FLAG_ADDR, 0);
    interrupts();
  } else {
    Serial.println("No interrupts occured");
    executionState = 0;
    noInterrupts();
    EEPROM.update(EEPROM_INTERRUPT_FLAG_ADDR, 0);
    interrupts();
  }

  delay(1000);
  Serial.print("I got the data: ");
  Serial.println(executionState);

  wdt_interrupts(WDTO_4S);
}

void loop() {
  for (int i = executionState; i < 6; i++) { // Repeat 6 times
    digitalWrite(ledPin1, HIGH);
    delay(200); // Using delay() for simplicity
    digitalWrite(ledPin1, LOW);

    digitalWrite(ledPin2, HIGH);
    delay(200);
    digitalWrite(ledPin2, LOW);

    if (i == 3) {
      // Simulate a hang that WDT should catch
      while (1) {
      }
    }

    executionState = i;
    Serial.print("Round Number: ");
    Serial.println(executionState);
    if (executionState == 5) {
      i = -1;
    }
  }
}

// Watchdog Timer Interrupt Service Routine
ISR(WDT_vect) {
  Serial.println("An interrupt occured");
  // Set the flag in EEPROM to indicate an interrupt occurred
  noInterrupts();
  EEPROM.update(EEPROM_INTERRUPT_FLAG_ADDR, 1);
  interrupts();

  executionState = 4;
  noInterrupts();
  EEPROM.update(EEPROM_EXECUTION_STATE_ADDR, executionState);
  interrupts();

  Serial.println("Reset in progress");

  wdt_Init(); // Reset the system
}

void wdt_interrupts(uint8_t value) {
  uint8_t oldSREG = SREG; // Save existing interrupt state
  uint8_t wdt_setting;
  value = (value > 9) ? 9 : value;
  wdt_setting = (value > 7) ? (1 << WDP3) : 0;
  wdt_setting |= (value & 7);

  noInterrupts(); // Disable interrupts
  wdt_reset(); // Reset WDT
  MCUSR &= ~(1 << WDRF); // Clear WDT restarted flag

  WDTCSR |= ((1 << WDCE) | (1 << WDE)); // Start timed sequence
  WDTCSR = (wdt_setting | (1 << WDIE)); // Set new WDT settings and enable interrupt

  SREG = oldSREG; // Restore interrupt state
}

void wdt_Init() {
  cli();
  wdt_reset();
  wdt_enable(WDTO_4S); // Enable WDT with a timeout of 4 seconds
  sei();
}