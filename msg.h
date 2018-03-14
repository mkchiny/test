#ifndef MSG_H_
#define MSG_H_

int msg_queue_create(int *piMsgId);
int msg_queue_send(int iMsgId, void *pMsg, int iMsgSize);
int msg_queue_recv(int iMsgId, void *pMsg, int iMsgSize);
int msg_queue_delete(int iMsgId);
int msg_queue_recv_non_block(int iMsgId, void *pMsg, int iMsgSize);

#endif /* MSG_H_ */
