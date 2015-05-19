#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-udp-packet.h"
#include "net/rpl/rpl.h"
#include "net/netstack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Global definition
/*--------------------------------------------------------------------------------*/
#define OUT_PORT 4444
#define IN_PORT 1111
#define MAX_PAYLOAD_LEN 30

static struct uip_udp_conn *connection;
static uip_ipaddr_t destination_ipaddr;
static uip_ipaddr_t source_ipaddr;

// Starting of contiki program
/*--------------------------------------------------------------------------------*/
PROCESS(ping_process, "PING process");
PROCESS(blink_process, "BLINK process");

AUTOSTART_PROCESSES(&ping_process, &blink_process);


// Global functions
/*--------------------------------------------------------------------------------*/
static void set_global_address() {
    uip_ipaddr_t ipaddr;
    
    uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
    
    #if 0
	uip_ip6addr(&source_ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 1);
    #elif 1
	uip_ip6addr(&source_ipaddr, 0xaaaa, 0, 0, 0, 0, 0x00ff, 0xfe00, 1);
    #else
	uip_ip6addr(&source_ipaddr, 0xaaaa, 0, 0, 0, 0x0250, 0xc2ff, 0xfea8, 0xcd1a);
    #endif
}

static void send_packet() {
    static int seq_id;
    char buf[MAX_PAYLOAD_LEN];

    seq_id++;
    printf("ping %s", destination_ipaddr);
    uip_udp_packet_sendto(connection, buf, strlen(buf), &destination_ipaddr, UIP_HTONS(IN_PORT));
}


// Process thread
/*--------------------------------------------------------------------------------*/
PROCESS_THREAD(ping_process, ev, data) {
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(button_sensor);

    set_global_address();
    
    while(1) {
	PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

	// blink led at the moment of send data
	leds_on(0xFF);
	clock_delay(10000000);
	leds_off(0xFF);

	// send data
	connection = udp_new(NULL, UIP_HTONS(OUT_PORT), NULL);
        if(connection == NULL) {
            printf("No connection");
            PROCESS_EXIT();
        }
        udp_bind(connection, UIP_HTONS(IN_PORT));
	send_packet();
    }
    PROCESS_END();
}

PROCESS_THREAD(blink_process, ev, data) {
    PROCESS_BEGIN();

    while(1) {
    PROCESS_YIELD();
	if(uip_newdata()) {          
	    //char *appdata = (char *) uip_appdata;	 
	    printf("blink");
            leds_on(0xFF);
	    /*
            clock_delay(10000000);	
            leds_off(0xFF);
            clock_delay(10000000);	
            leds_on(0xFF);	    
            clock_delay(10000000);	
            leds_off(0xFF);
            clock_delay(10000000);	
            leds_on(0xFF);	   
            clock_delay(10000000);	
            leds_off(0xFF);
	    */
        }
    }
    PROCESS_END();
}
