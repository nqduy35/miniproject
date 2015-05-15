#include "contiki.h"
#include <stdio.h>

PROCESS(p_hello_led, "Hello led process");

AUTOSTART_PROCESSES(&p_hello_led);

// define
PROCESS_THREAD(p_hello_led, ev, data) {
	PROCESS_BEGIN();
	printf("Hello led !!!");
	while(1) {
		PROCESS_WAIT_EVENT();
		leds_toggle(LEDS_ALL);
	}
	PROCESS_BEGIN();
}
