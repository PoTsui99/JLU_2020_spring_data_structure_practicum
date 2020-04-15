//���겻����
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
//ѧ����ӵ�
#include<vector> // ��Ϊ��ʦ����ʹ��stl
#include<algorithm>

#define BLACK 0
#define WHITE 1
#define EMPTY 2
int currentSize;//��¼ÿ���߷����ɵĸ���
struct Point{ //��ṹ
	int x,y;
};
struct Point_1{ //��ṹ
	int x,y;
	int score;
};
struct Step{ //���ṹ
	Point first,second;
	int value;
};
int Board[19][19]; //�洢������Ϣ����Ԫ��ֵΪ BLACK, WHITE, EMPTY ֮һ
std::vector<Step> moveCondition;//�洢�Ϸ����߷�����ЧԪ�ظ�����currentSize��¼
std::vector<Point_1> validCondition;//�洢�����ߵ����
bool hasNeighbor(int x, int y){
	int direction_x[7] = {0, 1, 2, 3, -1, -2, -3};
	int direction_y[7] = {0, 1, 2, 3, -1, -2, -3};
	for(int i  = 0; i < 7; i++)
		for(int  j = 1; j < 7; j++)
			if (Board[x + direction_x[i]][y + direction_y[j]] != 2)
				return true;
	for(int  i = 1; i < 7; i++)
			if (Board[x + direction_x[i]][y] != 2)
				return true;
	return false;
}

int getValue(int x, int y, int computerSide){

}

bool sortByM1( const Step &v1, const Step &v2)//ע�⣺�������Ĳ���������һ��Ҫ��vector��Ԫ�ص�����һ��
{
    return v1.value < v2.value;//��������
}

// ������
void generateMove(int computerSide){
	std::vector<Step>::iterator it;
	std::vector<Point_1>::iterator it_1;
    for (it = moveCondition.begin(); it != moveCondition.end(); ){
        it = moveCondition.erase(it);
    }
	for (it_1 = validCondition.begin(); it_1 != validCondition.end(); ){
        it_1 = validCondition.erase(it_1);
    }
	currentSize = 0;
	//Ѱ�ҿ����µĵ�
	for(int i  = 0; i < 19; i++)
		for(int  j = 1; j < 19; j++){
				if (Board[i][j] == 2){
					if(hasNeighbor(i, j)){
						int temp = getValue(i, j, computerSide);
						Point_1 temp_struct;
						temp_struct.x = i;
						temp_struct.y = j;
						temp_struct.score = temp;
						validCondition.push_back(temp_struct);
					}
				}
		}
	//�������µĵ���ϣ�Ҫ������ӳ��Ϊһ��
	for(int i  = 0; i < validCondition.size(); i++)
		for(int  j = i + 1; j < validCondition.size(); j++){
			Step temp;
			temp.first.x = validCondition[i].x;
			temp.first.y = validCondition[i].y;
			temp.second.x = validCondition[j].x;
			temp.second.y = validCondition[j].y;
			temp.value = validCondition[i].score + validCondition[j].score;
			moveCondition.push_back(temp);
		}
	//��moveCondition�����Ԫ�ذ�����������������
	std::sort(moveCondition.begin(),moveCondition.end(),sortByM1);
}


int main()
{
	Step step;//��ʱ���ṹ
	char message[256];//ͨ����Ϣ����
    int computerSide;//����ִ����ɫ
    int start=0;//�Ծֿ�ʼ���
	srand(int(time(0)));
	//�˴����ó�ʼ������
    //...
    
	while (1)	//������ѭ��
	{
		fflush(stdout);//��Ҫɾ������䣬�������������
		scanf("%s", message);//��ȡƽ̨������Ϣ
        //��������
		if (strcmp(message, "name?") == 0)//���սƽ̨���Ͷ���
		{
			fflush(stdin);
			/***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
			/*******/		printf("name Eureka\n");		/**ֻ�޸�����壬��Ҫɾ��name�ո�****/
			/***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
		}
		else if (strcmp(message, "new") == 0)//���������,��������
		{
			int i, j;
			scanf("%s", message);//��ȡ����ִ����ɫ
			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //ִ��
			else  computerSide = WHITE;   //ִ��

			for (i = 0; i<19; ++i)   //��ʼ�����
				for (j = 0; j<19; ++j)
					Board[i][j] = EMPTY;
			start = 1;

			if (computerSide == BLACK)
			{
				/**********���ɵ�һ���ŷ�����������step�ṹ�У���������Ϊ(step.first.x,step.first.y)**********/
				/****************************���·������룬���滻�ҵ�ʾ������******************************/

				// ��һ��������ʵûɶ�û���,ֱ���ֶ��Ż�
				step.first.x = 9;
				step.first.y = 9;


				/******************************����������һ���������*******************************************/

				Board[step.first.x][step.first.y] = computerSide;//����������
				printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//����ŷ�
			}
		}
		else if(strcmp(message,"move")==0)//����,���������,��������
		{
			scanf("%s", message);//��ȡ���������ŷ�
            fflush(stdin); 
			step.first.x=message[0]-'A';		step.first.y=message[1]-'A';
			step.second.x=message[2]-'A';		step.second.y=message[3]-'A';
            //�����������
			Board[step.first.x][step.first.y] = 1 - computerSide;
			if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.x][step.second.y] = 1 - computerSide;

            /**********************************************************************************************************/
			/***�������ӵ����꣬������step�ṹ�У���1������(step.first.x,step.first.y)����2������(step.first.x,step.first.y)*****/
			/**************************************���·������룬���滻�ҵ�ʾ������*****************************************/
			
			//���ɵ�1������λ��step.first.x��step.first.y
			int x, y;
			x = rand() % 19; y = rand() % 19;
			while (Board[x][y] != EMPTY)
			{
				x = rand() % 19;	y = rand() % 19;
			}
			step.first.x = x;
			step.first.y = y;
			Board[step.first.x][step.first.y] = computerSide;

			//���ɵ�2������λ��step.second.x��step.second.y	
			x = rand() % 19;	y = rand() % 19;
			while (Board[x][y] != EMPTY)
			{
				x = rand() % 19;	y = rand() % 19;
			}
			step.second.x = x;
			step.second.y = y;
			Board[step.second.x][step.second.y] = computerSide;
			
			/*****************************************������������******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n",step.first.x+'A',step.first.y+'A',step.second.x+'A',step.second.y+'A');//����ŷ�
		}
        else if (strcmp(message, "error") == 0)//�ŷ�����
        {
            fflush(stdin);
        }
		else if (strcmp(message, "end") == 0)//�Ծֽ���
		{
            fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//�˳�����
		{
            fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
	return 0;
}

 