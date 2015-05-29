#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "rules.h"

using namespace std;

#define SERVADDR "2001:660:4701:1001:a60:6eff:fe8a:60c0"

int main(int argc, char *argv[]) {
	int sock;
	socklen_t clilen;
	struct sockaddr_in6 server_addr; 
	struct sockaddr_in6 client_addr;
	char addrbuf[INET6_ADDRSTRLEN];

	/*----------------------------preprocessing---------------------------*/
	// socket
	sock = socket(PF_INET6, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("[failed] creating socket");
		exit(1);
	}

	// address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, SERVADDR, &server_addr.sin6_addr);
	server_addr.sin6_port = htons(PORT);


	// Client phai send HPS lien tuc khi thay het gio, khi nhan duoc DBR thi thoi
	/*----------------------------sending HPS---------------------------*/	
	int msglen = createMSG(HPS, "", "", "", "");	    
	if (sendto(sock, msg, msglen, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("[failed] sendto");
		exit(2);
	}

	/*----------------------------receiving DBR---------------------------*/
	struct message_cmd *msg_cmd = (struct message_cmd *) buv;
	while (1) {	    
            bzero(buv, BUF_SIZE); 
	    clilen = sizeof(client_addr);
	    if (recvfrom(sock, buv, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &clilen) < 0) {
	        perror("[failed] recvfrom");
	        exit(3);
	    }
                                                                                                            
	    if(msg_cmd->code == DBR) {
	        if(!strcmp(SERVADDR, inet_ntop(AF_INET6, &client_addr.sin6_addr, addrbuf, INET6_ADDRSTRLEN))) {
		   printf("Just receive DBR \n");	
		   break;
	        }
	    }
	}

	// gui RPS lien tuc cho toi khi nhan duoc VBR
	/*----------------------------sending RPS---------------------------*/	
	msglen = createMSG(RPS, "", "", "", "");	
	if (sendto(sock, msg, msglen, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        	perror("[failed] sendto");
        	exit(2);
        }

	while(1) {
	/*----------------------------receiving VBR---------------------------*/
	    while(1) {
		bzero(buv, BUF_SIZE);                                                                        
	        if (recvfrom(sock, buv, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &clilen) < 0) {	
                    perror("[failed] recvfrom");
                    exit(3);
                }
	                                                                                                     
	        if(msg_cmd->code == VBR) {
	            printf("Just receive VBR \n");        	    
	            break;
	        }
	    }
	

	/*----------------------------sending VCK---------------------------*/	
	    msglen = createMSG(VCK, "", "", "", "");                                                      
            if (sendto(sock, msg, msglen, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            	perror("[failed] sendto");
            	exit(2);
            }
                                                                                                      
        /*----------------------------receiving PBR---------------------------*/
	    while(1) { 
		bzero(buv, BUF_SIZE);                                                                           
	        if (recvfrom(sock, buv, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &clilen) < 0) {          
                    perror("[failed] recvfrom");
                    exit(3);
                }
                                                                                                              
                if(msg_cmd->code == PBR) {
                    printf("Just receive PBR --- DONE \n ");	           
	            break;
                }
	    }
	}		

	/*----------------------------terminator---------------------------*/	
	close(sock);
	return 0;
}
