#include <stdio.h>
#include <string.h>

#include "msg.h"

typedef struct _msg_st_
{
	int id;
    char acMsg[1024];
}MSG_ST;

int main(void)
{
	int iMsgId = 0;
	MSG_ST stMsg;

	if(msg_queue_create(&iMsgId) == -1)
	{
		return -1;
	}

	do
	{
		memset(&stMsg, 0, sizeof(MSG_ST));
		snprintf(stMsg.acMsg, sizeof(stMsg.acMsg), "Hello");
		stMsg.id = 78;
		if(msg_queue_send(iMsgId, &stMsg, sizeof(MSG_ST)) == -1)
		{
			break;
		}

		memset(&stMsg, 0, sizeof(MSG_ST));
		snprintf(stMsg.acMsg, sizeof(stMsg.acMsg), "asdfasdf234");
		stMsg.id = 23;
		if(msg_queue_send(iMsgId, &stMsg, sizeof(MSG_ST)) == -1)
		{
			break;
		}

		memset(&stMsg, 0, sizeof(MSG_ST));
		if(msg_queue_recv(iMsgId, &stMsg, sizeof(MSG_ST)) == -1)
		{
			break;
		}
		printf("===>[%s, %d]%d %s\n", __FUNCTION__, __LINE__, stMsg.id, stMsg.acMsg);

		memset(&stMsg, 0, sizeof(MSG_ST));
		if(msg_queue_recv(iMsgId, &stMsg, sizeof(MSG_ST)) == -1)
		{
			break;
		}

		printf("===>[%s, %d]%d %s\n", __FUNCTION__, __LINE__, stMsg.id, stMsg.acMsg);

		memset(&stMsg, 0, sizeof(MSG_ST));
		msg_queue_recv(iMsgId, &stMsg, sizeof(MSG_ST));
		printf("===>[%s, %d]%d %s\n", __FUNCTION__, __LINE__, stMsg.id, stMsg.acMsg);
	}while(0);


	msg_queue_delete(iMsgId);

	return 0;
}



