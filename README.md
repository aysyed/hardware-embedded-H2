# Great Lakes Sensing System: Event-Driven Trigger (Task H2)

## Overview
This repository contains the system design, validation strategy, and initial testing firmware for the H2 Event-Driven Trigger node of the XG Lab Great Lakes project. To bypass continuous software polling and drastically reduce payload power consumption, this architecture utilizes an ultra-low-power comparator to physically wake the microcontroller only when a statistically significant environmental event occurs. 

## Papers


## Hardware Architecture (DRAFT)
The system seperates the continuous monitoring logic from the main processing unit using the following components:
*   **Microcontroller:** STM32F103x8/B (ARM Cortex-M3, 72 MHz). The MCU maintains deep Sleep, Stop, or Standby modes to minimize power draw.
*   **Hardware Trigger:** LM393B Dual Comparator. Features 200µA supply current, 1µs response time, and an open-drain output compatible with TTL/CMOS logic.
*   **Sensor Node:** SHT45 Temperature & Humidity sensor (from H1)

### Logic Flow Diagram (DRAFT)
```text
[SHT45 Temp/Humidity Sensor]  <-- (From H1)
          │
          │ (Analog Voltage Signal: V_IN)
          ▼
 ┌──────────────────────────┐         ┌──────────────────────────────────┐
 │  LM393B Comparator       │         │  Threshold Voltage (V_REF)       │
 │                          │◄────────┤  (Defined by S1/S2 ML Models)    │
 │  Compares V_IN to V_REF  │         └──────────────────────────────────┘
 └────────┬─────────────────┘
          │
          │ (Digital Interrupt: Open-Drain pulled LOW upon event)
          ▼
 ┌──────────────────────────┐
 │  STM32F103 MCU           │ (Maintains Deep Sleep / Standby Mode)
 │  [EXTI Hardware Pin PA0] │
 └────────┬─────────────────┘
          │ (MCU Wakes Up)
          ▼
[H3 Communications Handoff] 
(Triggers LoRa Transmission Sequence)
