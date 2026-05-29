#include <Arduino.h>

// H2: Hardware Interrupt Wake-Up Logic Validation
// Validates STM32 response to the LM393B comparator trigger

const int COMPARATOR_INT_PIN = PA0; // STM32 External Interrupt Pin
volatile bool eventTriggered = false;
volatile unsigned long lastInterruptTime = 0;

// Interrupt Service Routine (ISR) executed when LM393B pulls LOW
void thresholdCrossedISR() {
    unsigned long interruptTime = millis();
    
    // Software debounce: Ignore interrupts within 200ms
    if (interruptTime - lastInterruptTime > 200) {
        eventTriggered = true;
        lastInterruptTime = interruptTime;
    }
}

void setup() {
    Serial.begin(115200);
    
    // LM393B uses an open-drain output; internal pull-up is required
    pinMode(COMPARATOR_INT_PIN, INPUT_PULLUP);
    
    // Attach interrupt to trigger on a FALLING edge
    attachInterrupt(digitalPinToInterrupt(COMPARATOR_INT_PIN), thresholdCrossedISR, FALLING);
    
    Serial.println("System initialized. Entering standby test mode...");
}

void loop() {
    if (eventTriggered) {
        Serial.println(">>> EVENT DETECTED: Hardware threshold crossed. <<<");
        
        // Handoff to H3. Wake LoRa module and execute wireless transmission.
        
        eventTriggered = false;
        Serial.println("Returning to deep sleep state...");
    }
}
