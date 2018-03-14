/**
* @file		msg.c
* @version	1.0
* @brief	message queue interface
* @note
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define MSG_MAX_LEN (8192 - sizeof(long int))
typedef struct _msg_queue_st_
{
    long int msg_type;
    char msg[MSG_MAX_LEN];
}msg_queue_st;

/**
* @brief	create message queue
* @param	piMsgId	message id
* @return	return msg create status
* @retval	0	sucess
* @retval	-1	create msg failed
* */
int msg_queue_create(int *piMsgId)
{
	int iMsgId = 0;

	iMsgId = msgget(IPC_PRIVATE, IPC_CREAT|IPC_EXCL|00666);
	if(iMsgId < 0)
	{
		fprintf(stdout, "create msg error:errno=%d, err_msg=%s\n", errno, strerror(errno));
		return -1;
	}

	if(piMsgId)
	{
		*piMsgId = iMsgId;
	}

	return 0;
}

/**
* @brief	send message
* @param	iMsgId	message id
* @param	pMsg	message want to send
* @param	iMsgSize	message size
* @return	return msg send status
* @retval	0	sucess
* @retval	-1	send msg failed
* @retval	-2	message length is less than sizeof(long)
*/
int msg_queue_send(int iMsgId, void *pMsg, int iMsgSize)
{
	int iRet = 0;
	msg_queue_st msg;

	if(iMsgSize <= 0)
	{
		fprintf(stdout, "message length is less than 0\n");
		return -2;
	}

	memset(&msg, 0, sizeof(msg_queue_st));
	memcpy(msg.msg, pMsg, iMsgSize);
	msg.msg_type = iMsgId;
	iRet = msgsnd(iMsgId, &msg, iMsgSize, 0);
	if(iRet < 0)
	{
		fprintf(stdout, "send msg error:errno=%d, err_msg=%s\n", errno, strerror(errno));
		return -1;
	}

	return 0;
}

/**
* @brief	recv message
* @param	iMsgId	message id
* @param	pMsg	save recv message
* @param	iMsgSize	message size
* @return	return msg recv status
* @retval	0	sucess
* @retval	-1	recv msg failed
* @retval	-2	message length is less than sizeof(long)
*/
int msg_queue_recv(int iMsgId, void *pMsg, int iMsgSize)
{
	int iRet = 0;
	msg_queue_st msg;

	if(iMsgSize <= 0)
	{
		fprintf(stdout, "message length is less than 0\n");
		return -2;
	}

	memset(&msg, 0, sizeof(msg_queue_st));
	msg.msg_type = iMsgId;
	iRet = msgrcv(iMsgId, &msg, iMsgSize, 0, 0);
	if(iRet < 0 || iRet > iMsgSize)
	{
		fprintf(stdout, "send msg error:errno=%d, err_msg=%s\n", errno, strerror(errno));
		return -1;
	}

	memcpy(pMsg, msg.msg, iMsgSize);

	return 0;
}

/**
* @brief	delete message queue
* @param	iMsgId	message id
* @return	return delete msg queue status
* @retval	0	sucess
* @retval	-1	delete msg queue failed
*/
int msg_queue_delete(int iMsgId)
{
	if(iMsgId < 0)
	{
		printf("===>[%s, %d]error msg id %d\n", __FUNCTION__, __LINE__, iMsgId);
		return -1;
	}

    if(msgctl(iMsgId, IPC_RMID, 0) == -1)
    {
    	fprintf(stdout, "delete msg error:errno=%d, err_msg=%s\n", errno, strerror(errno));
        return -1;
    }

    return 0;
}

int msg_queue_recv_non_block(int iMsgId, void *pMsg, int iMsgSize)
{
	int iRet = 0;
	msg_queue_st msg;

	if(iMsgSize <= 0)
	{
		fprintf(stdout, "message length is less than 0\n");
		return -2;
	}

	memset(&msg, 0, sizeof(msg_queue_st));
	msg.msg_type = iMsgId;
	iRet = msgrcv(iMsgId, &msg, iMsgSize, 0, IPC_NOWAIT);
	if(iRet < 0 || iRet > iMsgSize)
	{
//		fprintf(stdout, "send msg error:errno=%d, err_msg=%s\n", errno, strerror(errno));
		return -1;
	}

	memcpy(pMsg, msg.msg, iMsgSize);

	return 0;
}
