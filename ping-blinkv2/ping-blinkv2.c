#include "contiki.h"
#include "lib/random.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"
#include "sys/node-id.h"
#include "simple-udp.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234
#define MAX_PAYLOAD_LEN	30

static struct simple_udp_connection connection;
uip_ipaddr_t des_ipaddr;
uip_ipaddr_t src_ipaddr;
uip_ipaddr_t nxt_ipaddr;

// Starting contiki program
/*---------------------------------------------------------------------------*/
PROCESS(ping_blink_v2_process, "Ping blink process");
AUTOSTART_PROCESSES(&ping_blink_v2_process);

// Global functions
/*---------------------------------------------------------------------------*/
static void set_global_address(){
    // Source address
    uip_ip6addr(&src_ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&src_ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&src_ipaddr, 0, ADDR_AUTOCONF);

    // Destination address
    uip_ip6addr(&des_ipaddr,0xaaaa,0,0,0,0,0,0,1);
    uip_ip6addr(&nxt_ipaddr,0xaaaa,0,0,0,0x212,0x7401,0x1,0x101);
   
    // Routing
    uip_ds6_route_add(&des_ipaddr,64,&nxt_ipaddr);
}

static void blink(int nb) {
    while (nb>0) {
	nb--;
	leds_on(0xFF);
	clock_delay(10000000);
	leds_off(0xFF);
    }
}

static void receiver() {
    printf("Receive a bonjour \n");
    blink(3); 
}

// Main process
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ping_blink_v2_process, ev, data) {
    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(button_sensor);

    set_global_address();

    simple_udp_register(&connection, UDP_PORT, NULL, UDP_PORT, receiver);

    while(1) {
	PROCESS_WAIT_EVENT_UNTIL(ev==sensors_event && data==&button_sensor); 

	// Blink led
	blink(1);

	// Send data
	printf("Send data \n");
        char buf[MAX_PAYLOAD_LEN] = "bonjour";
        simple_udp_sendto(&connection, buf, strlen(buf) + 1, &des_ipaddr);  
    }

    PROCESS_END();
}
