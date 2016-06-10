#include "func.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/********************************************************************
*��¼��������
********************************************************************/
SOCKET  connectToServer()
{
	WSADATA wsaData;
	SOCKET sockClient;			//�ͻ���Socket
	SOCKADDR_IN addrServer;		//����˵�ַ
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sockClient = socket(AF_INET, SOCK_STREAM, 0);	//�½��ͻ���socket

	//����Ҫ���ӵķ���˵�ַ
	addrServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.19"); 
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(10002);		//���Ӷ˿�10002

	//���ӵ������
	int ret = connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
	if (ret)
	{
		printf("���������Ӵ������������������\n");
		Sleep(3000);
		exit(-1);
	}
	return sockClient;
}

/************************************************************************/
/* �����û���Ϣ�жϷ���ĸ������                                                                     */
/************************************************************************/
int travUserMesg(char *userMessage)    //�����û���Ϣ
{
	if (userMessage == NULL)
	{
		printf("�û���Ϣָ��Ϊ�գ�\n");
		
		return -1;
	}

	while (*userMessage != '\0')							//�����û���
	{
		if (isalpha(*userMessage) || isdigit(*userMessage)) //�ж���ĸ������
		{
			userMessage++;
		}
		else
		{
			//system("cls");
			printf("�û���Ϣ�����ڴ����֡�����ĸ���������!");
			return -1;
		}
	}
	return 1;
}

/*��δ������ڳ��ֵݹ�ĵ�Ч�ʱ�����
void judgeUserMessageOverEight(int userMessageLen)
{
	int choose = 0;
	int length = userMessageLen;
	if (length > 8)
	{
		printf("\n���������Ϣ���󣡳���8λ��\n");
		printf("\n��ѡ�񣺰�1���������档��������˳���");
		fflush(stdin);
		scanf("%d", &choose);
		fflush(stdin);
		if (choose == 1)
		{
			system("cls");
			logToSystem();
		}
		else
		{
			exit(0);
		}
	}
}
*/

/********************************************************************
*��ʼ������������Ϣ�Ľṹ��
********************************************************************/
void initStruct(logM * plogM)
{
	memset(plogM->headFlag, 0, sizeof(plogM->headFlag));
	memset(plogM->userMessage, 0, sizeof(plogM->userMessage));
}
/************************************************************************/
/* ��ʼ���û�������Ϣ�ṹ��                                                                     */
/************************************************************************/
void initUserALLMsgStruct(userMsg * pUserMsg)
{
	memset(pUserMsg->userName, 0, sizeof(pUserMsg->userName));
	memset(pUserMsg->del, 0, sizeof(pUserMsg->del));

	memset(pUserMsg->mail, 0, sizeof(pUserMsg->mail));
	memset(pUserMsg->job, 0, sizeof(pUserMsg->job));
	memset(pUserMsg->createPerson, 0, sizeof(pUserMsg->createPerson));
	memset(pUserMsg->createTime, 0, sizeof(pUserMsg->createTime));
	memset(pUserMsg->role, 0, sizeof(pUserMsg->role));
	memset(pUserMsg->password, 0, sizeof(pUserMsg->password));
}
/************************************************************************/
/* ������������ص�headflag��Ϣ�����ݷ�����Ϣȷ�ϴ�����                              */
/************************************************************************/
void processServerReMsg(logM *receiveServerMsg, SOCKET sockClient)
{
	logM *pSend = receiveServerMsg;
	SOCKET sok = sockClient;

	if (1 == pSend->headFlag[0])  //�жϷ�����������Ϣ
	{
		system("cls");
		printf("\n\n\n\n               ��¼�ɹ�");
		Sleep(2000);
		system("cls");

		int choose = -1;       //��½�ɹ�֮�����ϵͳ����˵������й���
		while (choose)			//ѭ���û�ѡ�� 1.�û�����  2.��ɫ����  3.Ȩ�޹��� 
		{
			manageMenu();
			fflush(stdin);

			printf("\n��ѡ����:");
			scanf("%d", &choose);
			if (getchar() != '\n')
			{
				printf("��������������ѡ���ţ�\n");
				continue;
			}

			if (choose == 1 && strncmp(userInfo, "0000", 4))		//�û�����
			{
				userManage(sok);
			}
		
			else if (2 == choose && strncmp(userInfo+ 4, "0000", 4))   //��ɫ����
			{
				roleManage(sok);
			}

			else if (3 == choose && strncmp(userInfo + 8, "00", 2))
			{
				permissionManage(sok);
			}
			else if (0 != choose)
			{
				printf("��������������ѡ���ţ�\n");
			}
		}
	}
	else  //������������Ϣʧ��
	{
		printf("\n��¼ʧ�ܣ�");
		Sleep(2000);
		exit(0);
	}
}


////////////////////////////////�û�����//////////////////////////////////
void addUser(SOCKET sockClient)
{
	SOCKET sok = sockClient;
	userMsg *pUserMsg = (userMsg *)calloc(1, sizeof(userMsg));
	char *allMsg = (char *)calloc(1024, sizeof(char));

	logM *pRe = (logM *)calloc(1, sizeof(logM));		
	//��½֮����������ظ��ͻ���headflag[1]=1;���������������ӵ��û�����Ϣ

	logM *pFromServer = (logM *)calloc(1, sizeof(logM)); //��ʱ���������ܷ��������ص��û���ӳɹ������Ϣ

	char *pum = NULL;
	initUserALLMsgStruct(pUserMsg);

	pRe->headFlag[0] = 2;	//�û�����
	pRe->headFlag[1] = 1;	//�����û��ı�־
	while (1)
	{
		printf("\n\n\n\n       �������û���:");
		scanf("%s", pUserMsg->userName);
		pum = pUserMsg->userName;
		if (1 == travUserMesg(pum))
		{
			strcat(allMsg, pUserMsg->userName);
			strcat(allMsg, ":");
			break;
		}
		else
		{
			printf("�����ʽ���ԣ����������룺\n");
		}
		
		system("cls");
	}
	int index;
	while (1)
	{
		printf("\n\n\n\n       ������绰:");
		scanf("%s", pUserMsg->del);
		if (strlen(pUserMsg->del) == 11)
		{
			for (index = 0; index < 11; index++)
			{
				if (isdigit(pUserMsg->del[index]))
				{
					;
				}
				else
				{
					break;
				}
			}
			if (index == 11)
			{
				strcat(allMsg, pUserMsg->del);
				strcat(allMsg, ":");
				break;
			}
			else
			{
				printf("�����ʽ���ԣ����������룺\n");
			}
		}
		else
		{
			printf("�����ʽ���ԣ����������룺\n");
		}
		
		system("cls");
	}
	
	while (1)
	{
		printf("\n\n\n\n       ����������:");
		scanf("%s", pUserMsg->mail);
		char *p = strstr(pUserMsg->mail, "@");
		if (p)
		{
			char *tmp = pUserMsg->mail + strlen(pUserMsg->mail) - 4;
			if (strncmp(tmp, ".com", 4) == 0)
			{
				strcat(allMsg, pUserMsg->mail);
				strcat(allMsg, ":");
				break;
			}
		}
		else
		{
			printf("�����ʽ���ԣ����������룺\n");
		}
		system("cls");
	}
	
	while (1)
	{
		printf("\n\n\n\n       ������ְλ:");
		scanf("%s", pUserMsg->job);
		int length = strlen(pUserMsg->job);
		if (length > 0 && length < 20)
		{
			for (index = 0; index < length; index++)
			{
				if (!isalpha(pUserMsg->job[index]))
				{
					break;
				}
			}
			if (index == length)
			{
				strcat(allMsg, pUserMsg->job);
				strcat(allMsg, ":");
				break;
			}
		}
		else
		{
			printf("�����ʽ���ԣ����������룺\n");
		}
		
		system("cls");
	}
	

	/*printf("\n\n\n\n       �����봴����:");
	scanf("%s", pUserMsg->createPerson);*/
	
	//��ȡ������
	strcat(allMsg, userInfo + 10);
	strcat(allMsg, ":");
	system("cls");

	
	
	//printf("\n\n\n\n       �����봴��ʱ��:");
	//scanf("%s", pUserMsg->createTime);
	//��ȡ����ʱ��
	getSystemTime(pUserMsg->createTime);
	strcat(allMsg, pUserMsg->createTime);
	strcat(allMsg, ":");
	system("cls");

	logM *roleList = listExistRole(sok);
	if (getNameOrRoleInput(roleList->userMessage, pUserMsg->role) == 1)
	{
		strcat(allMsg, pUserMsg->role);
		strcat(allMsg, ":");
	}

	char password[10] = { 0 };
	while (1)
	{
		printf("\n\n\n\n       ����������:");
		judgePasswd(password);
		pum = password;
		if (travUserMesg(pum) == 1)
		{
			printf("     ���ٴ�ȷ������:");
			judgePasswd(pUserMsg->password);
			pum = pUserMsg->password;
			if (travUserMesg(pum) == 1)
			{
				if (strcmp(password, pUserMsg->password) == 0)
				{
					strcat(allMsg, pUserMsg->password);
					break;
				}
			}
		}
	}
	//system("cls");

	strcpy(pRe->userMessage, allMsg);

	printf("\n�����û���Ϣȷ��\n�����û���ʶ��headFlag[1]:%d\n�û���Ϣ��%s\n", pRe->headFlag[1], pRe->userMessage);
	send(sockClient, (char *)pRe, sizeof(*pRe), 0);			//��������

	recv(sockClient, (char *)pFromServer, sizeof(*pFromServer), 0);			//���ܷ��������ص�����
	printf("received server :%d %s\n", pFromServer->headFlag[1], pFromServer->userMessage);

	if (1 == pFromServer->headFlag[1])
	{
		printf("����û��ɹ�!\n");
	}
	else
	{
		printf("����û�ʧ�ܣ������������룬������ӣ�\n");
	}

	free(pUserMsg);
	free(allMsg);
	free(pRe);
	free(pFromServer);
}
void deleteUser(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯ����Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ���û���Ϣ

	logM *nameList = listExistUserName(soc);
	char queryName[20] = { 0 };
	if (getNameOrRoleInput(nameList->userMessage, queryName) == 1)
	{
		free(nameList);
		strcpy(plogM->userMessage, queryName);
	}

		
	plogM->headFlag[0] = 2;
	plogM->headFlag[1] = 2;
		send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//��������
		printf("\n�����û���ȷ��headFlag[1]:%d %s\n", plogM->headFlag[1], plogM->userMessage);

		recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//���ܷ��������ص�����
		printf("received server :%d %s\n", plogMRe->headFlag[1], plogMRe->userMessage);

		if (2 == plogMRe->headFlag[1])
		{
			printf("ɾ���ɹ���\n");
		}
		else
		{
			printf("ɾ��ʧ�ܣ��������Ĳ�����\n");
		}

	free(plogM);
	free(plogMRe);
}
void changeUser(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯ����Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ���û���Ϣ

	userMsg *pUserMsg = (userMsg *)calloc(1, sizeof(userMsg)); //����������޸���Ϣ
	char *allMsg = (char *)calloc(1024, sizeof(char));			//�����޸���Ϣ��

		//�޸��û��������
		//�г������û���
	logM *nameList = listExistUserName(soc);
	if (getNameOrRoleInput(nameList->userMessage, pUserMsg->userName) == 1)
	{
		strcat(allMsg, pUserMsg->userName);
		strcat(allMsg, ":");
	}
		////����Ҫ�޸ĵ��û��������޸���Ϣ
		//char queryName[20] = { 0 };
		//plogM->headFlag[1] = 3;
		////memset(plogMRe, 0, sizeof(logM));

		////����Ҫ�޸ĵ��û���
		//printf("\n\n\n\n       ������Ҫ�޸ĵ��û���:");
		//scanf("%s", pUserMsg->userName);
		
		system("cls");

		int index;
		while (1)
		{
			printf("\n\n\n\n       ������绰:");
			scanf("%s", pUserMsg->del);
			if (strlen(pUserMsg->del) == 11)
			{
				for (index = 0; index < 11; index++)
				{
					if (isdigit(pUserMsg->del[index]))
					{
						;
					}
					else
					{
						break;
					}
				}
				if (index == 11)
				{
					strcat(allMsg, pUserMsg->del);
					strcat(allMsg, ":");
					break;
				}
				else
				{
					printf("�����ʽ���ԣ����������룺\n");
				}
			}
			else
			{
				printf("�����ʽ���ԣ����������룺\n");
			}

			system("cls");
		}

		while (1)
		{
			printf("\n\n\n\n       ����������:");
			scanf("%s", pUserMsg->mail);
			char *p = strstr(pUserMsg->mail, "@");
			if (p)
			{
				char *tmp = pUserMsg->mail + strlen(pUserMsg->mail) - 4;
				if (strncmp(tmp, ".com", 4) == 0)
				{
					strcat(allMsg, pUserMsg->mail);
					strcat(allMsg, ":");
					break;
				}
			}
			else
			{
				printf("�����ʽ���ԣ����������룺\n");
			}
			system("cls");
		}

		while (1)
		{
			printf("\n\n\n\n       ������ְλ:");
			scanf("%s", pUserMsg->job);
			int length = strlen(pUserMsg->job);
			if (length > 0 && length < 20)
			{
				for (index = 0; index < length; index++)
				{
					if (!isalpha(pUserMsg->job[index]))
					{
						break;
					}
				}
				if (index == length)
				{
					strcat(allMsg, pUserMsg->job);
					strcat(allMsg, ":");
					break;
				}
			}
			else
			{
				printf("�����ʽ���ԣ����������룺\n");
			}

			system("cls");
		}

		/*printf("\n\n\n\n       �����봴����:");
		scanf("%s", pUserMsg->createPerson);

		strcat(allMsg, pUserMsg->createPerson);
		strcat(allMsg, ":");
		system("cls");

		printf("\n\n\n\n       �����봴��ʱ��:");
		scanf("%s", pUserMsg->createTime);

		strcat(allMsg, pUserMsg->createTime);
		strcat(allMsg, ":");
		system("cls");*/

		logM *roleList = listExistRole(soc);
		char *p, *start;
		while (1)
		{
			p = start = roleList->userMessage;
			printf("       �������ɫ:");
			scanf("%s", pUserMsg->role);
			while (*p)
			{
				p = strchr(start, ':');
				if (strncmp(start, pUserMsg->role, p - start) == 0)
				{
					strcat(allMsg, pUserMsg->role);
					strcat(allMsg, ":");
					free(roleList);
					//system("cls");
					break;
				}
				p++;
				start = p;
			}
			if (*p)
			{
				break;
			}
		}

		char *pum;
		char password[10] = { 0 };
		while (1)
		{
			printf("\n\n\n\n       ����������:");
			memset(password, 0, sizeof(password));
			judgePasswd(password);
			pum = password;
			if (travUserMesg(pum) == 1)
			{
				printf("     ���ٴ�ȷ������:");
				memset(pUserMsg->password, 0, sizeof(pUserMsg->password));
				judgePasswd(pUserMsg->password);
				pum = pUserMsg->password;
				if (travUserMesg(pum) == 1)
				{
					if (strcmp(password, pUserMsg->password) == 0)
					{
						strcat(allMsg, pUserMsg->password);
						break;
					}
				}
			}
		}

		strcpy(plogMRe->userMessage, allMsg);
		plogMRe->headFlag[0] = 2;
		plogMRe->headFlag[1] = 3;

		send(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);			//��������
		printf("\n�����û���ȷ��headFlag[1]:%d %s\n", plogMRe->headFlag[1], plogMRe->userMessage);

		recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//���ܷ��������ص�����
		printf("received server :%d %s\n", plogMRe->headFlag[1], plogMRe->userMessage);

		if (3 == plogMRe->headFlag[1])
		{
			printf("�޸ĳɹ���\n");
		}
		else
		{
			printf("�޸�ʧ�ܣ��������Ĳ�����\n");
		}

	free(plogM);
	free(plogMRe);
	free(pUserMsg);
	free(allMsg);
}
void queryUser(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯ����Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ���û���Ϣ

	logM *nameList = listExistUserName(soc);
	char queryName[20] = { 0 };
	//����Ҫ��ѯ���û������г����û�����ϸ��Ϣ
	if (getNameOrRoleInput(nameList->userMessage, queryName) == 1)
	{
		strcpy(plogM->userMessage, queryName);
	}
	plogM->headFlag[0] = 2;
	plogM->headFlag[1] = 4;
	plogM->headFlag[2] = 2;
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);	//��������
		printf("\n�����û���ȷ��headFlag[2]:%d %s\n", plogM->headFlag[2], plogM->userMessage);

		recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);	//���ܷ��������ص�����
		printf("received server :%d %s\n", plogMRe->headFlag[2], plogMRe->userMessage);

		strcat(plogMRe->userMessage, ":");
		if (2 == plogMRe->headFlag[2])
		{
			char *oneUserMsg = plogMRe->userMessage;
			char *pOum = oneUserMsg;
			char buf[40] = { 0 };

			printf("�û���Ϣ��ϸ��Ϣ:\n");
			while (*oneUserMsg != '\0')
			{
				if (*oneUserMsg == ':')
				{
					strncpy(buf, pOum, oneUserMsg - pOum);
					buf[oneUserMsg - pOum] = '\0';

					printf("���û���Ϣ�� %s\n", buf);
					pOum = pOum + (oneUserMsg - pOum) + 1;
				}
				oneUserMsg++;
			}
		}

	free(plogM);
	free(plogMRe);
}
logM *listExistUserName(SOCKET sockClient)
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯ����Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ���û���Ϣ

	userMsg *pUserMsg = (userMsg *)calloc(1, sizeof(userMsg)); //����������޸���Ϣ
	char *allMsg = (char *)calloc(1024, sizeof(char));			//�����޸���Ϣ��

	plogM->headFlag[0] = 2;	//�û������־
	plogM->headFlag[1] = 4;	//��ѯ��־
	plogM->headFlag[2] = 1;	//�����ѯ��־
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ����

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//���ܷ��������ص������ж��Ƿ�ɲ�

	if (1 == plogMRe->headFlag[2])
	{
		//�г��Ѵ��������û���
		char *allUserName = plogMRe->userMessage;
		char *pAum = allUserName;
		char buf[20] = { 0 };

		printf("�����û���Ϣ:\n");
		while (*allUserName != '\0')
		{
			if (*allUserName == ':')
			{
				strncpy(buf, pAum, allUserName - pAum);
				buf[allUserName - pAum] = '\0';
				printf("�û����� %s\n", buf);
				pAum = pAum + (allUserName - pAum) + 1;
			}
			allUserName++;
		}
	}
	else
	{
		printf("ɾ��������������г������û�����\n");
	}
	free(plogM);
	return plogMRe;
}

////////////////////////////////��ɫ����////////////////////////////////////
void getPrivilege(char *buf, int i)
{
	char temp[10];
	while (1)
	{
		memset(temp, 0, sizeof(buf));
		printf("%s", permissionMessg[i]);

		fgets(temp, 5, stdin);
		if (strlen(temp) == 2 && temp[1] == '\n')
		{
			if (temp[0] == '1' || temp[0] == '0')
			{
				buf[i] = temp[0];
				break;
			}
		}
		printf("�����ʽ����ȷ������������\n");
		fflush(stdin);
	}
}
void addRole(SOCKET sockClient)				//���ӽ�ɫ��
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ

	char roleName[100] = { 0 };		//�ݴ��ɫ����
	char roleNameAndPermission[1024] = { 0 };   //�ݴ�Ҫ��������������Ϣ
	

	if (!soc)
	{
		printf("socket error��");
	}

	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 1;

	while (1)
	{
		printf("\n�������ɫ���ƣ�1-10λ��ĸ�����֣���");
		fgets(roleName, 20, stdin);
		if (strlen(roleName) <= 10 && strlen(roleName) > 0)
		{
			roleName[strlen(roleName) - 1] = '\0';
			if (travUserMesg(roleName) == 1)
			{
				break;
			}
		}
		else
			printf("�����ʽ����ȷ������������\n");
	}
	strcat(roleNameAndPermission, roleName);
	strcat(roleNameAndPermission, ":");
	fflush(stdin);

	printf("\n��ѡ��Ҫ������ӵĽ�ɫ�����Ȩ�ޣ�");
	printf("\n����1��ʾ�����Ȩ�ޣ�0��ʾ�������Ȩ�ޣ�\n");

	int i = 0;
	char buf[15] = { 0 };
	while (i < 10)
	{
		getPrivilege(buf, i);
		i++;
	}
	
	strcat(roleNameAndPermission, buf);

	strcpy(plogM->userMessage, roleNameAndPermission);
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ����
	printf("\n���ӽ�ɫȷ�Ϸ��ͣ�\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	printf("�յ����������ص��ַ�����\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);
	
	if (1 == plogMRe->headFlag[1])
	{
		printf("\n��ɫ��ӳɹ���");
	}
	else
	{
		printf("\n��ɫ���ʧ�ܣ�");
	}

	free(plogM);
	free(plogMRe);
}
void deleteRole(SOCKET sockClient)			//ɾ����ɫ
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ
	char roleName[100] = { 0 };

	if (!soc)
	{
		printf("socket error��");
	}

	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 2;

	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, roleName) == 1)
	{
		strcat(plogM->userMessage, roleName);
	}


	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ����
	printf("\nɾ����ɫȷ�ϣ�\n heafFlag[1]=%d   %s", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	printf("\n���������أ�\n heafFlag[1]=%d   %s", plogMRe->headFlag[1], plogMRe->userMessage);

	if (2 == plogMRe->headFlag[1])
	{
		printf("\nɾ���ɹ���\n");
	}
	else
	{
		printf("\nɾ��ʧ�ܣ�\n");
	}
}
void changeRole(SOCKET sockClient)			//�޸Ľ�ɫ
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ

	char roleName[100] = { 0 };		//�ݴ��ɫ����
	char roleNameAndPermission[1024] = { 0 };   //�ݴ�Ҫ��������������Ϣ
	char changeName[200] = { 0 };

	//����Ҫ�޸ĵĽ�ɫ��
	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 3;
	memset(plogMRe, 0, sizeof(logM));

	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, roleName) == 1)
	{
		strcat(roleNameAndPermission, roleName);
		strcat(roleNameAndPermission, ":");
	}

	fflush(stdin);

	printf("\n�����¸���ý�ɫȨ�ޡ�\n");
	printf("\n����1��ʾ�����Ȩ�ޣ�0��ʾ�������Ȩ�ޣ�\n");

	int i = 0;
	char buf[15] = { 0 };
	while (i < 10)
	{
		getPrivilege(buf, i);
		i++;
	}

	strcat(roleNameAndPermission, buf);

	strcpy(plogM->userMessage, roleNameAndPermission);
	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ����
	printf("\n���ӽ�ɫȷ�Ϸ��ͣ�\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	printf("�յ����������ص��ַ�����\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);
	strcpy(userInfo, plogMRe->userMessage);
	if (3 == plogMRe->headFlag[1])
	{
		printf("\n��ɫ�޸ĳɹ���\n");
	}
	else
	{
		printf("\n��ɫ�޸�ʧ�ܣ�\n");
	}

	free(plogM);
	free(plogMRe);
}
void queryRole(SOCKET sockClient)			//��ѯ��ɫ
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ
		
	//����Ҫ��ѯ�Ľ�ɫ�����г����û�����ϸ��Ϣ
	char queryName[200] = { 0 };

	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 4;
	plogM->headFlag[2] = 2;
	memset(plogMRe, 0, sizeof(logM));

	logM *roleList = listExistRole(soc);
	if (getNameOrRoleInput(roleList->userMessage, queryName) == 1)
	{
		strcpy(plogM->userMessage, queryName);
	}

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//��������
	printf("\n�����û���ȷ��headFlag[2]:%d %s\n", plogM->headFlag[2], plogM->userMessage);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);		//���ܷ��������ص�����
	printf("received server :%d %s\n", plogMRe->headFlag[2], plogMRe->userMessage);

	strcat(plogMRe->userMessage, ":");
	if (2 == plogMRe->headFlag[2])
	{
		char *oneRolePermission = plogMRe->userMessage;

		printf("��ɫ��ϸ��Ϣ:\n");
		for (int i = 0; oneRolePermission[i] != ':'; i++)
		{
			if (oneRolePermission[i] == '1')
			{
				printf("%s\n", permissionMessg[i]);
			}	
		}
	}
	else
	{
		printf("\n��ѯʧ�ܣ�\n");
	}
	
	free(plogM);
	free(plogMRe);
}
logM *listExistRole(SOCKET sockClient)		//�г���ǰ���н�ɫ��
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ

	if (!soc)
	{
		printf("socket error��");
	}
	plogM->headFlag[0] = 3;
	plogM->headFlag[1] = 4;
	plogM->headFlag[2] = 1;

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ��ɫ
	printf("\n���ӽ�ɫ��ȷ�ϣ�\n heafFlag[1]=%d\n", plogMRe->headFlag[2]);

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	printf("\n���������أ�\n heafFlag[1]=%d\n", plogMRe->headFlag[2]);

	if (1 == plogMRe->headFlag[2])			//�����ѯ��ɫ�ɹ�
	{
		//�г��Ѵ������н�ɫ����
		char *allRoleName = plogMRe->userMessage;
		char *pArn = allRoleName;
		char buf[20] = { 0 };		//�ݴ��ɫһ������

		printf("���н�ɫ��Ϣ:\n");
		while (*allRoleName != '\0')
		{
			if (*allRoleName == ':')
			{
				strncpy(buf, pArn, allRoleName - pArn);
				buf[allRoleName - pArn] = '\0';
				printf("��ɫ���� %s\n", buf);
				//memset(buf, 0, sizeof(buf));
				pArn = pArn + (allRoleName - pArn) + 1;
			}
			allRoleName++;
		}
	}
	else
	{
		printf("\n�����ѯ��ɫ��Ϣʧ�ܣ�");
	}

	free(plogM);
	//free(plogMRe);
	return plogMRe;
}

////////////////////////////////Ȩ�޹���////////////////////////////////////
void changePermission(SOCKET sockClient)			//�޸�Ȩ��
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ

	if (!soc)
	{
		printf("socket error��");
	}
		
	char permissionNum[20] = { 0 };
	printf("ϵͳ����Ȩ���б�:\n");
	listExistPermission();


	printf("\n\n\nϵͳ����Ȩ���б�:\n");
	queryPermission(soc);
	int i = 0;
	while (i < 10)
	{
		getPrivilege(permissionNum, i);
		i++;
	}

	plogM->headFlag[0] = 4;	//Ȩ�޹����־
	plogM->headFlag[1] = 1;	//�����޸ı�־
	strcpy(plogM->userMessage, permissionNum);

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ����

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	printf("�յ����������ص��ַ�����\n heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);
	strcpy(userInfo, plogMRe->userMessage);
	if (1 == plogMRe->headFlag[1])
	{
		printf("�޸ĳɹ���\n");
	}
	else
	{
		printf("�޸�ʧ�ܣ�\n");
	}

	free(plogM);
	free(plogMRe);
}
void queryPermission(SOCKET sockClient)				//��ѯȨ��
{
	SOCKET soc = sockClient;
	logM *plogM = (logM *)calloc(1, sizeof(logM));		//����������������ѯȨ�޵���Ϣ
	logM *plogMRe = (logM *)calloc(1, sizeof(logM));	//���ܷ������ش���״̬��Ϣ��Ȩ����Ϣ


	if (!soc)
	{
		printf("socket error��");
	}
	plogM->headFlag[0] = 4;	//Ȩ�޹����־
	plogM->headFlag[1] = 2;	//�����ѯ��־

	send(sockClient, (char *)plogM, sizeof(*plogM), 0);			//���������ѯ����

	recv(sockClient, (char *)plogMRe, sizeof(*plogMRe), 0);
	printf("�յ����������ص��ַ�����heafFlag[1]=%d %s", plogMRe->headFlag[1], plogMRe->userMessage);

	if (2 == plogMRe->headFlag[1])
	{
		//�г��Ѵ�������Ȩ��
		char *pAp = plogMRe->userMessage;  //��0 1�ַ���
		int len = strlen(pAp);


		printf("����Ȩ����Ϣ:\n");
		for (int i = 0; i < len; i++)
		{
			if (pAp[i] != '0')
			{
				printf("%s\n", permissionMessg[i]);
			}
		}
	}
	else
	{
		printf("����ʧ�ܣ�\n");
	}

	free(plogM);
	free(plogMRe);

}
void listExistPermission()			//�г���ǰ����Ȩ����
{
	for (int i = 0; i < 10; i++)
	{
		printf("%s\n", permissionMessg[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////
void judgePasswd(char *password)
{
	char c;
	int a = 0;
	for (int i = 0; i < 100; i++)
	{
		c = _getch();
		if (c == 13)
		{
			break;
		}
		if ((c <= 90 && c >= 65) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
		{
			printf("*");
			password[i] = c;
		}
		while (c == 8 && i > 0)
		{
			printf("\b \b");
			i--;
			c = _getch();
			a++;
		}
		while (a >= 1)
		{
			c = _getch();
			if ((c <= 90 && c >= 65) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
			{
				printf("*");
				password[i] = c;
				a--;
				if (a < 1)
				{
					i--;
				}
				i++;
			}	
		}
	}
}
void userManage(SOCKET sockClient)
{
	SOCKET sok = sockClient;

	int chooseU = -1;
	while (chooseU)		//ѭ���û����� ��4��ѡ��
	{
		userManageMenu();
		printf("\n��ѡ����:");
		scanf("%d", &chooseU);

		if (getchar() != '\n')  //�ų�1A  ������
		{
			printf("��������������ѡ���ţ�\n");
			continue;
		}

		if (chooseU == 1 && userInfo[0] == '1')		//�����û�
		{
			printf("��Ҫ��д����Ϣ���£�");
			userAllMessageMenu();
			addUser(sok);
		}
		else if (2 == chooseU && userInfo[1] == '1')	//ɾ���û�
		{
			deleteUser(sok);
		}
		else if (3 == chooseU && userInfo[2] == '1')	//�޸��û�
		{
			changeUser(sok);
		}
		else if (4 == chooseU && userInfo[3] == '1')	//��ѯ�û�
		{
			printf("��ѯ�û���\n");
			queryUser(sok);
			printf("��ѯ�û�����\n");
		}
		else if (0 != chooseU)
		{
			printf("���������������������룺\n");

		}
	}
}
void roleManage(SOCKET sockClient)
{
	SOCKET sok = sockClient;
	int chooseR = -1;
	fflush(stdin);
	while (chooseR)
	{
		printf("��ɫ����˵���\n");
		roleManageMenu();
		printf("\n��ѡ����:");
		scanf("%d", &chooseR);

		if (getchar() != '\n')  //�ų�1A  ������
		{
			printf("��������������ѡ���ţ�\n");
			continue;
		}

		if (chooseR == 1 && userInfo[4] == '1')		//���ӽ�ɫ
		{
			printf("\n��Ҫ��д����Ϣ���£�");
			addRole(sok);
		}
		else if (2 == chooseR && userInfo[5] == '1')	//ɾ����ɫ
		{
			deleteRole(sok);
		}
		else if (3 == chooseR && userInfo[6] == '1')	//�޸Ľ�ɫ
		{
			changeRole(sok);
		}
		else if (4 == chooseR && userInfo[7] == '1')	//��ѯ��ɫ
		{
			queryRole(sok);
		}
		else if (0 != chooseR)
		{
			printf("���������������������룺\n");

		}
	}
}
void permissionManage(SOCKET sockClient)
{
	SOCKET sok = sockClient;
	int chooseP = -1;

	while (chooseP)
	{
		printf("Ȩ�޹���˵���\n");
		permissionManageMenu();
		fflush(stdin);

		printf("\n��ѡ����:");
		scanf("%d", &chooseP);

		if (getchar() != '\n')  //�ų�1A  ������
		{
			printf("��������������ѡ���ţ�\n");
			continue;
		}
		if (chooseP == 1 && userInfo[8] == '1')		//�޸�Ȩ��
		{
			printf("�޸�Ȩ�ޣ�");
			changePermission(sok);
		}

		else if (2 == chooseP && userInfo[9] == '1')	//��ѯȨ��
		{
			printf("��ѯȨ�ޣ�\n");
			queryPermission(sok);
			printf("��ѯ�û�����\n");

		}
		else if (0 != chooseP)
		{
			printf("���������������������룺\n");
		}
	}

}
void getSystemTime(char *str)
{
	time_t nowtime;
	struct tm *timeinfo;
	time(&nowtime);
	timeinfo = localtime(&nowtime);
	int year, month, day;
	year = timeinfo->tm_year + 1900;
	month = timeinfo->tm_mon + 1;
	day = timeinfo->tm_mday;
	sprintf(str, "%d-%d-%d", year, month, day);
}

int getNameOrRoleInput(char *list, char *msg)
{
	char *p, *start;
	char buf[20];
	while (1)
	{
		p = start = list;
		printf("       �����������б��е�����:");
		scanf("%s", msg);
		while (*p)
		{
			memset(buf, 0, sizeof(buf));
			p = strchr(start, ':');
			strncpy(buf, start, p - start);
			if (strcmp(buf, msg) == 0)
			{
				return 1;
			}
			p++;
			start = p;
		}
	}
}