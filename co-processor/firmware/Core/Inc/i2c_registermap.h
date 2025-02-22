#ifndef __I2C_REGISTERS_H
#define __I2C_REGISTERS_H

#define DUMMY 0x00

// Chip ID and Firmware Version
#define CHIPID01  0x01
#define CHIPID02  0x02
#define FWVERSMA  0x03
#define FWVERSMI  0x04
#define FWVERSPA  0x05

// Interrupt Flags
#define INTFLAGS  0x09

// Rotary Switches
#define RSDITHER  0x0A
#define RSPALETT  0x0B
#define RSEXPOSU  0x0C

// Battery Voltage in mV
#define VBATTERY  0x0D

// Trigger Button States
#define TRIGGERA  0x10
#define TRIGGERB  0x11

// Trigger Button Counts, high-low-byte
#define TRIACNTA  0x12
#define TRIACNTB  0x13
#define TRIBCNTA  0x14
#define TRIBCNTB  0x15

// Display Output Registers
#define DISPLRST  0x20
#define DISPLCSA  0x21
#define DISPLCSB  0x22

#define LASTREG 0xFF

#endif