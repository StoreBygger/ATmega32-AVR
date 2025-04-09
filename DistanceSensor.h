/*
 * DistanceWithLCDtest.c
 *
 * Created: 24.01.2025 17:51:34
 * Author : LAB
 */ 

#define F_CPU 1000000UL
#define trig PD5
#define echo PD6

#define portName PORTD
#define pinName PIND
#define DDName DDRD

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void initUltrasonic() {
	DDName |= (1 << trig);   // Set TRIG as output
	DDName &= ~(1 << echo);  // Set ECHO as input
}

void triggerPulse() {
	portName |= (1 << trig);  // Set TRIG HIGH
	_delay_us(10);         // Wait 10µs
	portName &= ~(1 << trig); // Set TRIG LOW
}

uint16_t measureDistance() {
	triggerPulse();  // Send trigger pulse
	
	TCNT1 = 0;
	
	uint16_t start = 0, stop = 0;

	// Wait for echo to go HIGH (start timing)
	while (!(pinName & (1 << echo)));
	start = TCNT1;  // Reset Timer1

	// Wait for echo to go LOW (stop timing)
	while (pinName & (1 << echo));

	stop = TCNT1;
	uint16_t time = stop - start;  // Read timer value

	// Convert time to distance (cm)
	uint16_t distance = time / 58;

	return distance;  // Return calculated distance in cm
}

void initTimer1() {
	TCCR1B |= (1 << CS10); // No prescaler (1 µs per tick at 1MHz)
}