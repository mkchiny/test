#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "Userdefine.h"

typedef struct _stalker_server_
{
	char acName[32];
	char acUrl[1024];
}STALKER_SERVER;

typedef struct _stalker_server_list_
{
	int iTotal;
	char acMac[32];
	STALKER_SERVER *pstStalker;
}STALKER_SERVER_LIST;

/*
{
    "mac": "11:11:11:11:11:11",
    "server_list": [
        {
            "name": "Stalker0",
            "url": "http://www.baidu.com"
        },
        {
            "name": "Stalker1",
            "url": "http://www.baidu.com"
        },
        {
            "name": "Stalker2",
            "url": "http://www.baidu.com"
        },
        {
            "name": "Stalker3",
            "url": "http://www.baidu.com"
        }
    ]
}
 * */
#define STALKER_SERVER_LIST_FILE "stalker_server_list.txt"
#define MY_MALLOC malloc
#define MY_FREE free
void ParseStalkerServerList(char *pcBuffer, STALKER_SERVER_LIST *pstStalkerList)
{
	cJSON *root_json = NULL;
	cJSON *pstJsonTemp = NULL;

	if(pcBuffer == NULL || pstStalkerList == NULL)
	{
		return;
	}

	root_json = cJSON_Parse((const char*)pcBuffer);
	if(root_json != NULL)
	{
		int iArraySize = 0;
		STALKER_SERVER *pstStalkerServer = NULL;

		pstJsonTemp = cJSON_GetObjectItem(root_json, "mac");
		if(pstJsonTemp && pstJsonTemp->type == cJSON_String)
		{
			snprintf(pstStalkerList->acMac, sizeof(pstStalkerList->acMac), "%s", pstJsonTemp->valuestring);
		}

		pstJsonTemp = cJSON_GetObjectItem(root_json, "server_list");
		if(pstJsonTemp && pstJsonTemp->type == cJSON_Array)
		{
			int iIndex = 0;
			cJSON *pstTemp = NULL;
			cJSON *pstArrayItem = NULL;

			iArraySize = cJSON_GetArraySize(pstJsonTemp);
			if(iArraySize > 0)
			{
				pstStalkerServer = (STALKER_SERVER *)MY_MALLOC(iArraySize * sizeof(STALKER_SERVER));
				if(pstStalkerServer)
				{
					memset(pstStalkerServer, 0, iArraySize * sizeof(STALKER_SERVER));
				}
			}

			if(pstStalkerServer)
			{
				STALKER_SERVER *pstStalkerTemp = NULL;
				for(iIndex = 0; iIndex < iArraySize; iIndex++)
				{
					pstArrayItem = cJSON_GetArrayItem(pstJsonTemp, iIndex);

					pstStalkerTemp = &(pstStalkerServer[iIndex]);

					pstTemp = cJSON_GetObjectItem(pstArrayItem, "name");
					if(pstTemp && pstTemp->type == cJSON_String)
					{
						snprintf(pstStalkerTemp->acName, sizeof(pstStalkerTemp->acName), "%s", pstTemp->valuestring);
					}

					pstTemp = cJSON_GetObjectItem(pstArrayItem, "url");
					if(pstTemp && pstTemp->type == cJSON_String)
					{
						snprintf(pstStalkerTemp->acUrl, sizeof(pstStalkerTemp->acUrl), "%s", pstTemp->valuestring);
					}
				}
			}
		}
		pstStalkerList->iTotal = iArraySize;
		pstStalkerList->pstStalker = pstStalkerServer;

		cJSON_Delete(root_json);
	}
}

char *ConvertStalkerServerListToJsonData(STALKER_SERVER_LIST *pstStalkerList)
{
	cJSON *root = NULL;
	cJSON *array = NULL;
	cJSON *obj = NULL;
	int iItem = 0;
	char *json = NULL;
	char *data = NULL;
	int iJsonLen = 0;

	if(pstStalkerList == NULL)
	{
		return data;
	}

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "mac", (const char *)(pstStalkerList->acMac));

	array = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "server_list", array);
	for(iItem = 0; iItem < pstStalkerList->iTotal; iItem++)
	{
		obj = cJSON_CreateObject();

		cJSON_AddItemToArray(array, obj);
		cJSON_AddStringToObject(obj, "name", pstStalkerList->pstStalker[iItem].acName);
		cJSON_AddStringToObject(obj, "url", pstStalkerList->pstStalker[iItem].acUrl);
	}

	json = cJSON_Print(root);
	cJSON_Minify(json);
	iJsonLen = strlen(json);
	data = (char *)MY_MALLOC(iJsonLen + 1);
	memset(data, 0, iJsonLen + 1);
	strcat(data, json);
	memcpy(data, json, iJsonLen);

	cJSON_Delete(root);

	return data;
}

void SaveStalkerServerListToFlash(STALKER_SERVER_LIST *pstStalkerList)
{
	char *pcJsonData = NULL;
	FILE *fp = NULL;

	if(pstStalkerList == NULL)
	{
		return;
	}

	pcJsonData = ConvertStalkerServerListToJsonData(pstStalkerList);
	if(pcJsonData)
	{
		fp = fopen(STALKER_SERVER_LIST_FILE, "w");
		if(fp)
		{
			fwrite(pcJsonData, strlen(pcJsonData), 1, fp);
			fclose(fp);
		}
		MY_FREE(pcJsonData);
		pcJsonData = NULL;
	}
}

BOOL LoadStalkerList(STALKER_SERVER_LIST *pstStalkerList)
{
	FILE *fp = NULL;
	char *pcBuffer = NULL;
	int iBufLen = 0;
	BOOL bRet = FALSE;

	if(pstStalkerList == NULL)
	{
		return bRet;
	}

	fp = fopen(STALKER_SERVER_LIST_FILE, "r");
	if(fp == NULL)
	{
		return bRet;
	}

	fseek(fp, 0, SEEK_END);
	iBufLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(iBufLen > 0)
	{
		pcBuffer = (char *)MY_MALLOC(iBufLen);
		if(pcBuffer)
		{
			memset(pcBuffer, 0, iBufLen);
		}
	}

	if(pcBuffer)
	{
		fread(pcBuffer, iBufLen, 1, fp);
	}

	ParseStalkerServerList(pcBuffer, pstStalkerList);
	if(strlen(pstStalkerList->acMac) > 0)
	{
		bRet = TRUE;
	}

	if(pcBuffer)
	{
		MY_FREE(pcBuffer);
		pcBuffer = NULL;
	}

	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}

	return bRet;
}

void LoadDefaultStalkerList(STALKER_SERVER_LIST *pstStalkerList)
{
	if(pstStalkerList == NULL)
	{
		return;
	}

	snprintf(pstStalkerList->acMac, sizeof(pstStalkerList->acMac), "11:11:11:11:11:11");
	if(pstStalkerList->pstStalker)
	{
		MY_FREE(pstStalkerList->pstStalker);
		pstStalkerList->pstStalker = NULL;
	}
	pstStalkerList->iTotal = 0;
}

void AddStalkerServerToList(STALKER_SERVER stStalkerServer, STALKER_SERVER_LIST *pstStalkerList)
{
	int iTotalServer = 0;
	STALKER_SERVER *pstStalker = NULL;

	if(pstStalkerList == NULL)
	{
		return;
	}

	iTotalServer = pstStalkerList->iTotal;
	pstStalker = (STALKER_SERVER *)MY_MALLOC((iTotalServer + 1) * sizeof(STALKER_SERVER));
	if(pstStalker)
	{
		memset(pstStalker, 0, (iTotalServer + 1) * sizeof(STALKER_SERVER));

		snprintf(pstStalker[iTotalServer].acName, sizeof(pstStalker[iTotalServer].acName), "%s", stStalkerServer.acName);
		snprintf(pstStalker[iTotalServer].acUrl, sizeof(pstStalker[iTotalServer].acUrl), "%s", stStalkerServer.acUrl);
		if(iTotalServer > 0)
		{
			memcpy(pstStalker, pstStalkerList->pstStalker, iTotalServer * sizeof(STALKER_SERVER));
			MY_FREE(pstStalkerList->pstStalker);
		}
		pstStalkerList->pstStalker = pstStalker;
		pstStalkerList->iTotal = iTotalServer + 1;
	}
}

int main(int argc, char *argv[])
{
	STALKER_SERVER_LIST stStalkerServerList;
	STALKER_SERVER stStalkerServer;
	int iIndex = 0;

	memset(&stStalkerServerList, 0, sizeof(STALKER_SERVER_LIST));
	if(LoadStalkerList(&stStalkerServerList) == FALSE)
	{
		LoadDefaultStalkerList(&stStalkerServerList);
	}

	memset(&stStalkerServer, 0, sizeof(STALKER_SERVER));
	snprintf(stStalkerServer.acName, sizeof(stStalkerServer.acName), "Stalker%d", iIndex);
	snprintf(stStalkerServer.acUrl, sizeof(stStalkerServer.acUrl), "http://www.baidu.com");
	AddStalkerServerToList(stStalkerServer, &stStalkerServerList);
	iIndex++;

	memset(&stStalkerServer, 0, sizeof(STALKER_SERVER));
	snprintf(stStalkerServer.acName, sizeof(stStalkerServer.acName), "Stalker%d", iIndex);
	snprintf(stStalkerServer.acUrl, sizeof(stStalkerServer.acUrl), "http://www.baidu.com");
	AddStalkerServerToList(stStalkerServer, &stStalkerServerList);
	iIndex++;

	memset(&stStalkerServer, 0, sizeof(STALKER_SERVER));
	snprintf(stStalkerServer.acName, sizeof(stStalkerServer.acName), "Stalker%d", iIndex);
	snprintf(stStalkerServer.acUrl, sizeof(stStalkerServer.acUrl), "http://www.baidu.com");
	AddStalkerServerToList(stStalkerServer, &stStalkerServerList);
	iIndex++;

	memset(&stStalkerServer, 0, sizeof(STALKER_SERVER));
	snprintf(stStalkerServer.acName, sizeof(stStalkerServer.acName), "Stalker%d", iIndex);
	snprintf(stStalkerServer.acUrl, sizeof(stStalkerServer.acUrl), "http://www.baidu.com");
	AddStalkerServerToList(stStalkerServer, &stStalkerServerList);
	iIndex++;

	SaveStalkerServerListToFlash(&stStalkerServerList);
	printf("mac=%s\n", stStalkerServerList.acMac);
	printf("iTotal=%d\n", stStalkerServerList.iTotal);
	for(iIndex = 0; iIndex < stStalkerServerList.iTotal; iIndex++)
	{
		printf("name=%s url=%s\n", stStalkerServerList.pstStalker[iIndex].acName, stStalkerServerList.pstStalker[iIndex].acUrl);
	}

	if(stStalkerServerList.pstStalker)
	{
		free(stStalkerServerList.pstStalker);
		stStalkerServerList.pstStalker = NULL;
	}

	return 0;
}





