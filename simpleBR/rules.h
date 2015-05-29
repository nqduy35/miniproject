/* 	
* HPS : Hello Point Sync
* DBR : Data to Board Router
* RPS : Repair demand to Point Sync
* VBR : Version to Board Router 
* VCK : VBR acknowledge
* PBR : Permission to Board Router
*/
#define BUF_SIZE 1024
#define DATA_SIZE 64
#define PORT 4466

enum MSG_CODE {HPS, DBR, RPS, VBR, VCK, PBR};

char msg[BUF_SIZE];
char buv[BUF_SIZE];

struct message_cmd {
	enum MSG_CODE code;
	int arg1_len;
	char arg1[DATA_SIZE];
	int arg2_len;
	char arg2[DATA_SIZE];
	int arg3_len;
	char arg3[DATA_SIZE];
	int arg4_len;
	char arg4[DATA_SIZE];
};

int createMSG(MSG_CODE code, char* var1, char* var2, 
		char* var3, char* var4) { 
    bzero(msg, BUF_SIZE);
    struct message_cmd *msg_temp = (struct message_cmd *) msg; 
    msg_temp->code = code;
                                        
    strcpy(msg_temp->arg1,var1);
    msg_temp->arg1_len = sizeof(msg_temp->arg1);
                                                            
    strcpy(msg_temp->arg2,var2);
    msg_temp->arg2_len = sizeof(msg_temp->arg2);
                                                                        
    strcpy(msg_temp->arg3,var3);
    msg_temp->arg3_len = sizeof(msg_temp->arg3);    

	strcpy(msg_temp->arg4,var4);
    msg_temp->arg4_len = sizeof(msg_temp->arg4);    

    return (sizeof(msg_temp->code) + 
			sizeof(msg_temp->arg1_len) + 
			sizeof(msg_temp->arg2_len) + 
			sizeof(msg_temp->arg3_len) +
			sizeof(msg_temp->arg4_len) + 
			msg_temp->arg4_len + msg_temp->arg1_len + 
			msg_temp->arg2_len + msg_temp->arg3_len);
}
