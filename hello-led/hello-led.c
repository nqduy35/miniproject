#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include <stdio.h>

PROCESS(p_hello_led, "Hello led process");

AUTOSTART_PROCESSES(&p_hello_led);

// define
PROCESS_THREAD(p_hello_led, ev, data) {

    PROCESS_BEGIN();

    SENSORS_ACTIVATE(button_sensor);
    printf("Hello led !!!");

    PROCESS_WAIT_EVENT_UNTIL(ev==sensors_event && data==&button_sensor);		
    if(ev == sensors_event) {
        printf("Button pressed");
        leds_toggle(LEDS_ALL);
    }
	
    PROCESS_END();
}
