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
#include "simple-udp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Global definition
/*--------------------------------------------------------------------------------*/
#define DES_PORT 1234
#define SRC_PORT 1234
#define MAX_PAYLOAD_LEN 30

static struct simple_udp_conn *connection;
static uip_ipaddr_t destin_ipaddr;
static uip_ipaddr_t source_ipaddr;

// Starting of contiki program
/*--------------------------------------------------------------------------------*/
PROCESS(ping_process, "PING process");
PROCESS(blink_process, "BLINK process");

AUTOSTART_PROCESSES(&ping_process, &blink_process);


// Global functions
/*--------------------------------------------------------------------------------*/
static void set_global_address() {
    // Source address
    uip_ip6addr(&source_ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&source_ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&source_ipaddr, 0, ADDR_AUTOCONF);    

    // Destination address
    uip_ip6addr(&destin_ipaddr, 0xaaaa, 0, 0, 0, 0x0212, 0x7402, 0x0002, 0x0202);
}
static void send_packet() {
    static int seq_id;
    char buf[MAX_PAYLOAD_LEN];

    seq_id++;
    printf("ping %s", destin_ipaddr);
    //uip_udp_packet_sendto(connection, buf, strlen(buf), &destin_ipaddr, UIP_HTONS(IN_PORT));
}
static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
    uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port,
    const uint8_t *data, uint16_t datalen) {
    // Callback
    printf("Callback");
}


// Process thread
/*--------------------------------------------------------------------------------*/
PROCESS_THREAD(ping_process, ev, data) {
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(button_sensor);

    set_global_address();

    // remote IP equal NULL -> packet from anywhere
    simple_udp_register(&connection, SRC_PORT, &destin_ipaddr, DES_PORT, receiver);

    while(1) {
	PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

	// blink led at the moment of send data
	leds_on(0xFF);
	clock_delay(10000000);
	leds_off(0xFF);

	// send data
	char buf[20] = "Ohaio!!!";
	simple_udp_sendto(&connection, buf, strlen(buf) + 1, &destin_ipaddr);

	//connection = udp_new(NULL, UIP_HTONS(OUT_PORT), NULL);
        //if(connection == NULL) {
        //    printf("No connection");
        //    PROCESS_EXIT();
        //}
        //udp_bind(connection, UIP_HTONS(IN_PORT));
	//send_packet();
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
