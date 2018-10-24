/****************************************************************************
 * main.c
 * openacousticdevices.info
 * June 2017
 *****************************************************************************/

#include <stdbool.h>

#include "audioMoth.h"

/* Firmware version and description */

static uint8_t firmwareVersion[AM_FIRMWARE_VERSION_LENGTH] = {1, 0, 0};

static uint8_t firmwareDescription[AM_FIRMWARE_DESCRIPTION_LENGTH] = "Example-Firmware";

/* Required time zone handler */

void AudioMoth_timezoneRequested(int8_t *timezone) { }

/* Required interrupt handles */

void AudioMoth_handleSwitchInterrupt(void) { }
void AudioMoth_handleMicrophoneInterrupt(int16_t sample) { }
void AudioMoth_handleDirectMemoryAccessInterrupt(bool primaryChannel, int16_t **nextBuffer) { }

/* Required USB message handlers */

void AudioMoth_usbFirmwareVersionRequested(uint8_t **firmwareVersionPtr) {

    *firmwareVersionPtr = firmwareVersion;

}

void AudioMoth_usbFirmwareDescriptionRequested(uint8_t **firmwareDescriptionPtr) {

    *firmwareDescriptionPtr = firmwareDescription;

}

void AudioMoth_usbApplicationPacketRequested(uint32_t messageType, uint8_t *transmitBuffer, uint32_t size) { }
void AudioMoth_usbApplicationPacketReceived(uint32_t messageType, uint8_t *receiveBuffer, uint8_t *transmitBuffer, uint32_t size) { }


/* Utility functions for flashing the LEDs */

int flash_length = 250; // milliseconds

void flash_green() {
	AudioMoth_setGreenLED(true);
	AudioMoth_delay(flash_length);
	AudioMoth_setGreenLED(false);
}

void flash_red() {
	AudioMoth_setRedLED(true);
	AudioMoth_delay(flash_length);
	AudioMoth_setRedLED(false);
}

void flash_none() {
	AudioMoth_setRedLED(false);
	AudioMoth_setGreenLED(false);
	AudioMoth_delay(flash_length);
}

void flash_binary_char(unsigned char n) {
	/* Flash LEDs to display binary representation of n, from highest bit to lowest */
	for (unsigned char mask = 128; mask != 0; mask = mask/2){
		bool bit = n & mask;
		if (bit) {
			flash_red();
			flash_none();
		} else {
			flash_green();
			flash_none();
		}
	}
}

/* Main function */

int main(void) {

    /* Initialise device */

    AudioMoth_initialise();

    /* Check the switch position */

    AM_switchPosition_t switchPosition = AudioMoth_getSwitchPosition();

    if (switchPosition == AM_SWITCH_USB) {

        /* Handle the case that the switch is in USB position. Waits in low energy state until USB disconnected or switch moved  */

        AudioMoth_handleUSB();

    } else {

        /* Flash both LEDs twice to say we're turned on */

    	AudioMoth_setGreenLED(true);
        AudioMoth_setRedLED(true);

        AudioMoth_delay(3000);

        /* Turn off the watchdog timer because we're going to do a long operation */
        AudioMoth_stopWatchdog();

        AudioMoth_setRedLED(false);
        AudioMoth_setGreenLED(false);

        AudioMoth_delay(3000);

        AudioMoth_setGreenLED(true);
        AudioMoth_setRedLED(true);

        AudioMoth_delay(3000);

        AudioMoth_setRedLED(false);
        AudioMoth_setGreenLED(false);

        AudioMoth_delay(3000);

        /* Start counting up from 0 and display each number in binary by flashing the LEDs */
        /* Continue forever when counter wraps around */

    	unsigned char counter = 0;

    	while(true) {
    		flash_binary_char(counter);
    		AudioMoth_delay(1000);
    		++counter;
    	}
    }

}
