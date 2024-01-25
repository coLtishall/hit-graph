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
//从文件读取数据，建立邻接矩阵
void create(Graph* G)
{
	FILE* fp = fopen("----.txt", "r");
	//把文件中有效数据读取进数组
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
			if (i == 0)//读顶点数
			{
				temp1 = fgetc(fp);
				if (temp1 <= '9' && temp1 >= '0')//顶点数为两位数
				{
					num = 10 * (temp - '0') + temp1 - '0';
					temp = fgetc(fp);
				}
				else
				{
					num = temp - '0';
					temp = temp1;//下一个
				}
				G->n = num;
				i++;
			}
			else if (i == 1&&temp!=' ')//读边数
			{
				temp1 = fgetc(fp);
				if (temp1 <= '9' && temp1 >= '0')//边数为两位数
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
				temp = fgetc(fp);//直接取下一个
				continue;
			}
			else if (i > 1 && (temp >= 'A' && temp <= 'z'))//为英文字符
			{
				G->node[j++] = temp;//赋给顶点
				temp = fgetc(fp);
				if (j == G->n)
					flag = 1;
			}
			else if (flag == 1&&temp<='9'&&temp>='0')//为边权值相关信息
			{
				fgetc(fp);
				tool = fgetc(fp);//第二个edge下标
				fgetc(fp);
				tool1 = fgetc(fp);//edge权值
				
				G->edge[temp-'0'][tool-'0'] = tool1-'0';//赋值
				G->edge[tool - '0'][temp - '0'] = tool1 - '0';
				temp = fgetc(fp);//下一个
			}
		}
	}
}
//初始化图
void initGraph(Graph* G)
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			G->edge[i][j] = infinity;
}
//展示图
void showgraph(Graph G)
{
	printf("顶点共%d个，顶点有：",G.n);
	for (int i = 0; i < G.n; i++)
		printf("%c ", G.node[i]);
	printf("\n");
	printf("边有%d条，邻接矩阵为：\n",G.e);
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
//找到未包含顶点集到已包含顶点集的最小权值路
int Mincost(int* cost,Graph G,int point)
{
	int temp;
	temp = infinity;
	int sub = 0;
	for(int i=0;i<G.n;i++)
		if (i != point)
		{
			if (!included[i] && cost[i] < temp)//未包含的顶点
			{
				temp = cost[i];
				sub = i;
			}
		}
	return sub;
}
//计算其余所有点到下标为i的顶点的最小值和前件
void Dijkstra(Graph G,int i,char* last,int* cost)
{
	//初始化
	for (int j = 0; j < G.n; j++)
	{
		included[j] = false;
		cost[j] = G.edge[i][j];
		last[j] = i;
	}
	included[i] = true;
	//找到最小权值并更新(n-1次)
	for (int j = 0; j < G.n; j++)
	{
			int minindex = Mincost(cost,G,i);
			included[minindex] = true;
			for(int k=0;k<G.n;k++)
				if (k != i)
				{
					if (!included[k])//查看非包含顶点集和到已包含顶点的集合路径是否变化
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
//打印指定顶点到其余顶点的最短路径
void shortestroad(int i,Graph G)
{
	int cost[SIZE];
	char last[SIZE];
	int j;
	//计算
	Dijkstra(G, i, last, cost);
	//打印顶点到其余顶点的最小路径值
	printf(" %c 到其余顶点的最短路长为：\n",G.node[i]);
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
				printf(" 无");
			else
			{
				printf("%3d", cost[j]);
			}
		}
	}
	//打印路径
	for (int j = 0; j < G.n; j++)
	{
		int index;
		char store[SIZE];
		int k = j;
		if (i != k)
		{
			printf("\n");
			printf("%c到%c的最短路程表示为：", G.node[i], G.node[k]);
			if (last[k] == i)//两个顶点邻接或者无路（未更新）
			{
				if (G.edge[i][k] < infinity)//邻接
				{
					printf(" %c->%c ", G.node[i], G.node[k]); 
					continue;
				}
				else//无路
				{
					printf(" %c与%c不连通 ", G.node[i], G.node[k]);
					continue;
				}
			}
			int pt = SIZE - 1;//溯回记录
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
			while (pt < SIZE - 1)//正向输出
			{
				printf(" %c->%c ", store[pt], store[pt + 1]);
				pt++;
			}
		}

	}
}
//计算路径
void Floyd(Graph G,Tool* T)
{
	int i, j, k;
	//初始化
	for(i=0;i<G.n;i++)
		for (j = 0; j < G.n; j++)
		{
			T->cost[i][j] = G.edge[i][j];
			if (i == j)//同个顶点间距离表示为0
				T->cost[i][j] = 0;

			T->last[i][j] = -1;//路径上未加入顶点
		}
	for(k=0;k<G.n;k++)
		for(i=0;i<G.n;i++)
			for (j = 0; j < G.n; j++)
			{
				if (T->cost[i][j] > T->cost[i][k] + T->cost[k][j])//加入顶点使路径变短
				{
					//更新
					T->cost[i][j] = T->cost[i][k] + T->cost[k][j];
					T->last[i][j] = k;
				}
			}
}
//打印路径
void Path(int p1, int p2, Tool* T, Graph G)
{
	int k = T->last[p1][p2];
	if (k != -1)
	{
		Path(p1, k, T, G);//前半段（不包括k）
		printf("->%c", G.node[k]);
		Path(k, p2, T, G);//后半段（不包括k）
	}
}
//打印任意两个顶点间最短路径主函数
void two_point_shortest(Graph G, int p1, int p2,Tool* T)
{
	Floyd(G,T);
	if (T->cost[p1][p2] < infinity)
		printf("%c到%c的最短路程长度为%d\n", G.node[p1], G.node[p2], T->cost[p1][p2]);
	else
		printf("%c到%c间无路径\n", G.node[p1], G.node[p2]);
	printf("%c到%c间路径为：\n", G.node[p1], G.node[p2]);
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