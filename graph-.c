#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define SIZE 20
#define infinity 9999
typedef struct graph
{
	char node[SIZE];
	int edge[SIZE][SIZE];
	int n;
	int e;
}Graph;
typedef struct cl
{
	int cost[SIZE][SIZE];
	int last[SIZE][SIZE];
}Tool;
bool included[SIZE];
//���ļ���ȡ���ݣ������ڽӾ���
void create(Graph* G)
{
	FILE* fp = fopen("----.txt", "r");
	//���ļ�����Ч���ݶ�ȡ������
	if (fp)
	{
		int i = 0,j=0,flag=0;
		int storeedge[300] = { 0 };
		char storenode[SIZE];
		char temp;
		char temp1;
		char tool, tool1;
		int num;
		temp = fgetc(fp);
		while (temp != EOF)
		{
			if (i == 0)//��������
			{
				temp1 = fgetc(fp);
				if (temp1 <= '9' && temp1 >= '0')//������Ϊ��λ��
				{
					num = 10 * (temp - '0') + temp1 - '0';
					temp = fgetc(fp);
				}
				else
				{
					num = temp - '0';
					temp = temp1;//��һ��
				}
				G->n = num;
				i++;
			}
			else if (i == 1&&temp!=' ')//������
			{
				temp1 = fgetc(fp);
				if (temp1 <= '9' && temp1 >= '0')//����Ϊ��λ��
				{
					num = 10 * (temp - '0') + temp1 - '0';
					temp = fgetc(fp);
				}
				else
				{
					num = temp - '0';
					temp = temp1;
				}
				G->e = num;
				i++;
			}
			else if (temp == ' ' || temp == '\n')
			{
				temp = fgetc(fp);//ֱ��ȡ��һ��
				continue;
			}
			else if (i > 1 && (temp >= 'A' && temp <= 'z'))//ΪӢ���ַ�
			{
				G->node[j++] = temp;//��������
				temp = fgetc(fp);
				if (j == G->n)
					flag = 1;
			}
			else if (flag == 1&&temp<='9'&&temp>='0')//Ϊ��Ȩֵ�����Ϣ
			{
				fgetc(fp);
				tool = fgetc(fp);//�ڶ���edge�±�
				fgetc(fp);
				tool1 = fgetc(fp);//edgeȨֵ
				
				G->edge[temp-'0'][tool-'0'] = tool1-'0';//��ֵ
				G->edge[tool - '0'][temp - '0'] = tool1 - '0';
				temp = fgetc(fp);//��һ��
			}
		}
	}
}
//��ʼ��ͼ
void initGraph(Graph* G)
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			G->edge[i][j] = infinity;
}
//չʾͼ
void showgraph(Graph G)
{
	printf("���㹲%d���������У�",G.n);
	for (int i = 0; i < G.n; i++)
		printf("%c ", G.node[i]);
	printf("\n");
	printf("����%d�����ڽӾ���Ϊ��\n",G.e);
	for(int i=0;i<G.n;i++)
		for (int j = 0; j < G.n; j++)
		{
			if (G.edge[i][j] == infinity)
				printf(" []");
			else
				printf("%3d",G.edge[i][j]);
			if (j == G.n - 1)
				printf("\n");
		}
}
//�ҵ�δ�������㼯���Ѱ������㼯����СȨֵ·
int Mincost(int* cost,Graph G,int point)
{
	int temp;
	temp = infinity;
	int sub = 0;
	for(int i=0;i<G.n;i++)
		if (i != point)
		{
			if (!included[i] && cost[i] < temp)//δ�����Ķ���
			{
				temp = cost[i];
				sub = i;
			}
		}
	return sub;
}
//�����������е㵽�±�Ϊi�Ķ������Сֵ��ǰ��
void Dijkstra(Graph G,int i,char* last,int* cost)
{
	//��ʼ��
	for (int j = 0; j < G.n; j++)
	{
		included[j] = false;
		cost[j] = G.edge[i][j];
		last[j] = i;
	}
	included[i] = true;
	//�ҵ���СȨֵ������(n-1��)
	for (int j = 0; j < G.n; j++)
	{
			int minindex = Mincost(cost,G,i);
			included[minindex] = true;
			for(int k=0;k<G.n;k++)
				if (k != i)
				{
					if (!included[k])//�鿴�ǰ������㼯�͵��Ѱ�������ļ���·���Ƿ�仯
					{
						int sum = cost[minindex] + G.edge[minindex][k];
						if (sum < cost[k])
						{
							cost[k] = sum;
							last[k] = minindex;
						}
					}
				}
	}
}
//��ӡָ�����㵽���ඥ������·��
void shortestroad(int i,Graph G)
{
	int cost[SIZE];
	char last[SIZE];
	int j;
	//����
	Dijkstra(G, i, last, cost);
	//��ӡ���㵽���ඥ�����С·��ֵ
	printf(" %c �����ඥ������·��Ϊ��\n",G.node[i]);
	for (j = 0; j < G.n; j++)
	{
		if(j!=i)
		printf("  %c", G.node[j]);
	}
	printf("\n");
	for (j = 0; j < G.n; j++)
	{
		if (j != i)
		{
			if (cost[j] == infinity)
				printf(" ��");
			else
			{
				printf("%3d", cost[j]);
			}
		}
	}
	//��ӡ·��
	for (int j = 0; j < G.n; j++)
	{
		int index;
		char store[SIZE];
		int k = j;
		if (i != k)
		{
			printf("\n");
			printf("%c��%c�����·�̱�ʾΪ��", G.node[i], G.node[k]);
			if (last[k] == i)//���������ڽӻ�����·��δ���£�
			{
				if (G.edge[i][k] < infinity)//�ڽ�
				{
					printf(" %c->%c ", G.node[i], G.node[k]); 
					continue;
				}
				else//��·
				{
					printf(" %c��%c����ͨ ", G.node[i], G.node[k]);
					continue;
				}
			}
			int pt = SIZE - 1;//�ݻؼ�¼
			while (1)
			{
				store[pt--] = G.node[k];
				k = last[k];
				if (k == i)
				{
					store[pt] = G.node[i];
					break;
				}
			}
			while (pt < SIZE - 1)//�������
			{
				printf(" %c->%c ", store[pt], store[pt + 1]);
				pt++;
			}
		}

	}
}
//����·��
void Floyd(Graph G,Tool* T)
{
	int i, j, k;
	//��ʼ��
	for(i=0;i<G.n;i++)
		for (j = 0; j < G.n; j++)
		{
			T->cost[i][j] = G.edge[i][j];
			if (i == j)//ͬ�����������ʾΪ0
				T->cost[i][j] = 0;

			T->last[i][j] = -1;//·����δ���붥��
		}
	for(k=0;k<G.n;k++)
		for(i=0;i<G.n;i++)
			for (j = 0; j < G.n; j++)
			{
				if (T->cost[i][j] > T->cost[i][k] + T->cost[k][j])//���붥��ʹ·�����
				{
					//����
					T->cost[i][j] = T->cost[i][k] + T->cost[k][j];
					T->last[i][j] = k;
				}
			}
}
//��ӡ·��
void Path(int p1, int p2, Tool* T, Graph G)
{
	int k = T->last[p1][p2];
	if (k != -1)
	{
		Path(p1, k, T, G);//ǰ��Σ�������k��
		printf("->%c", G.node[k]);
		Path(k, p2, T, G);//���Σ�������k��
	}
}
//��ӡ����������������·��������
void two_point_shortest(Graph G, int p1, int p2,Tool* T)
{
	Floyd(G,T);
	if (T->cost[p1][p2] < infinity)
		printf("%c��%c�����·�̳���Ϊ%d\n", G.node[p1], G.node[p2], T->cost[p1][p2]);
	else
		printf("%c��%c����·��\n", G.node[p1], G.node[p2]);
	printf("%c��%c��·��Ϊ��\n", G.node[p1], G.node[p2]);
	printf("%c", G.node[p1]);
	Path(p1,p2,T,G);
	printf("->%c", G.node[p2]);
}

int main()
{
	Graph G;
	Tool T;
	initGraph(&G);
	create(&G);
	showgraph(G);
	shortestroad(4, G);
	printf("\n");
	two_point_shortest(G, 4, 5, &T);
	return 0;
}