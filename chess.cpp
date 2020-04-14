//国标不香吗
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
//学生后加的
#include<vector> // 因为老师允许使用stl
#include<algorithm>
#include <cstring>
#include <memory>
#include <limits.h>

using namespace std;
//== debug用
#include <fstream> //用以重定向
#define cin fin
#define cout fout
ifstream fin("test.in");
ofstream fout("test.out");
//==以上debug用


#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define INF INT_MAX
#define nonSenseInt -1
#define C6 1000000
#define A5 100000
#define S5 50000
//#define A4 50000
#define S4 10000
#define A3 10000
#define I3 5000
#define S3 1000
#define A2 100
#define S2 10
#define re(i,a,b) for(int i=a;i<b;++i)
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b


struct Point{ //点结构
    int x,y;
};
struct Point_1{ //点结构
    int x,y;
    int score;
};
struct Step{ //步结构
    Point first,second;
    int value;
};
// global:
int Board[19][19]; //存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一
int mySide=BLACK; // 方便起见,增加我方
// definition:
void copyStep(Step to, Step from);
bool sortByM1(const Step &v1, const Step &v2);
bool isInRange(int x, int y);
int evaluate(int computerside, int simuBoard[19][19]);
void ROW(int path[8], int m, int n,int color,int sim[19][19]);
void COL(int path[8], int m, int n,int color, int sim[19][19]);
void Diagonal(int path[8], int m, int n,int color, int sim[19][19]);
void exDiagonal(int path[8], int m, int n,int color, int sim[19][19]);
int compare7(int path[7]);
int compare8(int path[8]);
void numberReturn( int color, int CS[8],int simuBoard[19][19]);
int placeNotEmpty(int simuBoard[19][19]);
//int whoWin(int side = mySide, int simuBoard[19][19]);
bool hasNeighbor(int x, int y, int simuBoard[19][19]);
int getValue(int x, int y, int computerSide, int simuBoard[19][19]);
vector<Step>* generateMove(int computerSide, int simuBoard[19][19]);
int negaMax(int whosTurn, int depth, int alpha, int beta,int simuBoard[19][19]);
Step aGoodStep(int depth);

//横向储存
void ROW(int path[8], int m, int n,int color,int sim[19][19]=Board)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m][n + i];
        if (path[i] == 2)
        {
            path[i] = 0;
        }
        else if (path[i] == color)
        {
            path[i] = 1;
        }
        else
        {
            path[i] = -1;
        }
    }
}
void COL(int path[8], int m, int n,int color, int sim[19][19]=Board)
{
    for (int i = 0; i < 6; i++)
    {
        path[i] = sim[m + i][n];
        if (path[i] == 2)
        {
            path[i] = 0;
        }
        else if (path[i] == color)
        {
            path[i] = 1;
        }
        else
        {
            path[i] = -1;
        }
    }
}
void Diagonal(int path[8], int m, int n,int color, int sim[19][19]=Board)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m + i][n + i];
        if (path[i] == 2)
        {
            path[i] = 0;
        }
        else if (path[i] == color)
        {
            path[i] = 1;
        }
        else
        {
            path[i] = -1;
        }
    }
}
void exDiagonal(int path[8], int m, int n,int color, int sim[19][19]=Board)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m + i][n - i];
        if (path[i] == 2)
        {
            path[i] = 0;
        }
        else if (path[i] == color)
        {
            path[i] = 1;
        }
        else
        {
            path[i] = -1;
        }
    }
}
int compare7(int path[7])
{
    int sum1[7] = { 0,1,1,1,1,1,1 };
    int sum2[7] = { 1,1,1,1,1,1,0 };
    int sum3[7] = { 1,1,1,1,1,1,1 };
    int r = 0;
    r = equal(path, path + 7, sum1);
    if (r == 1) return 0;
    r = equal(path, path + 7, sum2);
    if (r == 1) return 0;
    r = equal(path, path + 7, sum3);
    if (r == 1) return 0;
    //七连和六连
    int A_5_1[7] = { 0,1,1,1,1,1,0 };
    r = equal(path, path + 7, A_5_1);
    if (r == 1) return 1;
    return -1;
}
int compare8(int path[8])
{
    int A5_2[8] = { 0,0,1,1,1,1,0,1 };
    int A5_3[8] = { 1,0,1,1,1,1,0,0 };
    int r = 0;
    r = equal(path, path + 8, A5_2);
    if (r == 1) return 1;
    r = equal(path, path + 8, A5_3);
    if (r == 1) return 1;
    int S5_1[8] = { -1,1,1,1,1,1,0 };
    int S5_2[8] = { 0,1,1,1,1,1,-1 };
    r = equal(path, path + 8, S5_1);
    if (r == 1) return 2;
    r = equal(path, path + 8, S5_2);
    if (r == 1) return 2;
    if (path[0] == -1 || path[1] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 2; i < 8; i++)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 5 && n == 1) return 2;
    }
    if (path[7] == -1 || path[6] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 5; i >= 8; i--)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 5 && n == 1) return 2;
    }
    //眠五检测完毕

    int A4[8] = { 0,0,1,1,1,1,0,0 };
    r = equal(path, path + 8, A4);
    if (r == 1) return 2;
    //活四检测完毕

    if (path[0] == -1 || path[7] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 1; i < 7; i++)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 4 && n == 2) return 3;
    }
    //眠四检测完成

    int A3_1[8] = { 0,0,1,1,1,0,0,0 };
    int A3_2[8] = { 0,0,1,1,0,1,0,0 };
    int A3_3[8] = { 0,0,1,0,1,1,0,0 };
    int A3_4[8] = { 0,0,0,1,1,1,0,0 };
    r = equal(path, path + 8, A3_1);
    if (r == 1) return 3;
    r = equal(path, path + 8, A3_2);
    if (r == 1) return 3;
    r = equal(path, path + 8, A3_3);
    if (r == 1) return 3;
    r = equal(path, path + 8, A3_4);
    if (r == 1) return 3;
    //活三检测完毕

    if (path[0] == -1 || path[7] == -1)
    {
        if (path[1] == 0 && path[6] == 0)
        {
            int num = 0;
            int n = 0;
            for (int i = 2; i < 6; i++)
            {
                if (path[i] == 1) num++;
                if (path[i] == 0) n++;
            }
            if (num == 3 && n == 1) return 4;
        }
    }
    //朦胧三判断完毕

    if (path[0] == -1 || path[2] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 2; i < 8; i++)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 3 && n == 3) return 5;
    }
    if (path[7] == -1 || path[6] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 5; i >= 0; i--)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 3 && n == 3) return 5;
    }
    //眠三判断完毕
    int m1 = 0;
    int m0 = 0;
    for (int i = 0; i < 8; i++)
    {
        if (path[i] == 1) m1++;
        if (path[i] == 0) m0++;
    }
    if (m1 == 2 && m0 == 4) return 6;
    //活二检测完毕

    if (path[0] == -1 || path[1] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 2; i < 8; i++)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 2 && n == 4) return 7;
    }
    if (path[0] == -1 || path[7] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 2; i < 7; i++)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 2 && n == 4) return 7;
    }
    if (path[7] == -1 || path[6] == -1)
    {
        int num = 0;
        int n = 0;
        for (int i = 0; i < 6; i++)
        {
            if (path[i] == 1) num++;
            if (path[i] == 0) n++;
        }
        if (num == 2 && n == 4) return 7;
    }
    //眠二检测完毕
    return -1;
}
void numberReturn( int color, int CS[8],int simuBoard[19][19]=Board)
{
    int number = 0;
    int num = 0;
    int r = 0;
    int path[8] = { 0,0,0,0,0,0,0,0 };
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            ROW(path, i, j, color, simuBoard);
            r = compare8(path);
            if (r != -1)
            {
                CS[r]++;
            }
        }

    }
    //横向判断
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            COL(path, i, j, color, simuBoard);
            r = compare8(path);
            if (r != -1)
            {
                CS[r]++;
            }
        }

    }
    //纵向判断
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            Diagonal(path, i, j,color), simuBoard;
            r = compare8(path);
            if (r != -1)
            {
                CS[r]++;
            }
        }

    }
    //斜向判断
    for (int i = 8; i < 19; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            exDiagonal(path, i, j,color, simuBoard);
            r = compare8(path);
            if (r != -1)
            {
                CS[r]++;
            }
        }
    }
    return;
}
int evaluate(int computerside, int simuBoard[19][19] = Board)//整体局面估分
{
    int CS[8] = { 0,0,0,0,0,0,0,0 };
    int value[8] = { 1000000,100000,80000,10000,5000,1000,100,10 };
    int path[7] = { 0,0,0,0,0,0,0 };
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                path[k] = simuBoard[i][j + k];
            }
            int r = compare7(path);
            if (r != -1) CS[r]++;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                path[k] = simuBoard[i + k][j];
            }
            int r = compare7(path);
            if (r != -1) CS[r]++;
        }
    }
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                path[k] = simuBoard[i + k][j + k];
            }
            int r = compare7(path);
            if (r != -1) CS[r]++;
        }
    }
    for (int i = 7; i < 19; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                path[k] = simuBoard[i - k][j + k];
            }
            int r = compare7(path);
            if (r != -1) CS[r]++;
        }
    }
    numberReturn( computerside, CS,simuBoard);
    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        score = score + CS[i] * value[i];
    }
    return score;
}

void copyStep(Step to, Step from){ // 对Step进行数值拷贝
    to.first.x = from.first.x;
    to.first.y = from.first.y;
    to.second.x = from.second.x;
    to.second.y = from.second.y;
}
bool isInRange(int x, int y){
    return (x>=0&&x<19&&y>=0&&y<19);
}
int placeNotEmpty(int simuBoard[19][19] = Board){
    int cnt = 0;
    re(i,0,19)
        re(j,0,19){
            if(simuBoard[i][j] != EMPTY){
                ++cnt;
            }
        }
    return cnt;
}

int whoWin(int side = mySide, int simuBoard[19][19] = Board){ // 返回值1:side方赢;2:side的反方赢
    /* (0,0) (0,1) (0,2)....
     * .
     * .
     * (18,0) (18,1)........(18,18)
     * */
    int dir[4][2] = {0, 1, 1, 0, 1, 1, 1, -1};

    int cnt = 0;
    re(i, 0, 19)
        re(j, 0, 19) { // 每点遍历
            if (simuBoard[i][j] == mySide) { // 看我方执棋是否六连
                re(k, 0, 4) { // 四个方向
                    cnt = 0;
                    re(l, 0, 6) { //第几颗连子 
                        if (isInRange(i + l * dir[k][0], j + l * dir[k][1])) {
                            if (simuBoard[i + k * dir[k][0]][j + k * dir[k][1]] != mySide){
                                break;
                            }else ++cnt; // 连子数+1
                        } else break; // 出现界外
                        if(cnt == 6)
                            return 1;
                    }
                }
            }
            else if (simuBoard[i][j] == 1 - mySide) {
                re(k, 0, 4) {// 四个方向
                    cnt = 0;
                    re(l, 0, 6) {
                        if (isInRange(i + l * dir[k][0], j + k * dir[k][1])) {
                            if (simuBoard[i + l * dir[k][0]][j + l * dir[k][1]] != 1 - mySide){
                                break;
                            }else ++cnt; // 连子数+1
                        } else break; // 出现界外
                        if(cnt == 6)
                            return -1;
                    }
                }
            }
        }
    return 0; // 未决出胜负
}

bool hasNeighbor(int x, int y, int simuBoard[19][19] = Board){ // 存储合法的走法
    int direction_x[7] = {0, 1, 2, 3, -1, -2, -3};
    int direction_y[7] = {0, 1, 2, 3, -1, -2, -3};
    for(int i  = 0; i < 7; i++)
        for(int  j = 1; j < 7; j++)
            if (((y + direction_y[j]) >= 0) && ((y + direction_y[j]) < 19) && ((x + direction_x[i]) >= 0) && ((x + direction_x[i]) < 19) && (simuBoard[x + direction_x[i]][y + direction_y[j]] != EMPTY))
                return true;
    for(int  i = 1; i < 7; i++)
        if (((x + direction_x[i]) < 19) && ((x + direction_x[i]) >= 0) && (simuBoard[x + direction_x[i]][y] != EMPTY))
            return true;
    return false;
}
int getValue(int x, int y, int computerSide, int simuBoard[19][19] = Board){ // 棋盘修改为局部变量
    if(computerSide == BLACK){
        simuBoard[x][y] = BLACK;
        int score = evaluate(computerSide,simuBoard);
        simuBoard[x][y] = EMPTY;
        return score;
    }
         simuBoard[x][y] = WHITE;
        int score = evaluate(computerSide,simuBoard);
        simuBoard[x][y] = EMPTY;
        return score;

}
bool sortByM1(const Step &v1, const Step &v2){ //注意：本函数的参数的类型一定要与vector中元素的类型一致
    return v1.value < v2.value;//升序排列
}
// 两步的
vector<Step>* generateMove(int computerSide, int simuBoard[19][19] = Board){
    // std::vector<Step>::iterator it;
    std::vector<Point_1> validCondition;
    std::vector<Point_1>::iterator ite;
    // for (it = moveCondition.begin(); it != moveCondition.end(); ){
    //     it = moveCondition.erase(it);
    // }
    // for (it_1 = validCondition.begin(); it_1 != validCondition.end(); ){
    //     it_1 = validCondition.erase(it_1);
    // }
    //寻找可以下的点
    vector<Step>* toReturn = new vector<Step>;
    for(int i  = 0; i < 19; i++)
        for(int  j = 0; j < 19; j++){//我不知道之前为啥写成1，但看起来j初始值为0更对一点
            if (simuBoard[i][j] == 2){
                if(hasNeighbor(i, j, simuBoard)){
                    int temp = getValue(i, j, computerSide, simuBoard);
                    Point_1 temp_struct;
                    temp_struct.x = i;
                    temp_struct.y = j;
                    temp_struct.score = temp;
                    validCondition.push_back(temp_struct);
                }
            }
        }
    //将可以下的点组合，要将两步映射为一步
    for(int i  = 0; i < validCondition.size(); i++)
        for(int  j = i + 1; j < validCondition.size(); j++){
            Step temp;
            temp.first.x = validCondition[i].x;
            temp.first.y = validCondition[i].y;
            temp.second.x = validCondition[j].x;
            temp.second.y = validCondition[j].y;
            temp.value = validCondition[i].score + validCondition[j].score;
            toReturn->push_back(temp);
        }
    //将toReturn里面的元素按评估分数升序排列
    std::sort(toReturn->begin(),toReturn->end(),sortByM1);
    return toReturn;
}
int negaMax(int whosTurn, int depth, int alpha, int beta,int simuBoard[19][19]=Board){
    // TODO: 出现平局、胜负情况下的判定返回,即没有child的情况
    // FIXME: 确定whoWin的参数是mySide还是whosTurn
    //平局
    if(whoWin(whosTurn,simuBoard)==0&&placeNotEmpty(simuBoard)==19*19)
        return 0;
        // FIXME: 返回值取决于评估值的上限
    else if(whoWin(whosTurn,simuBoard)==1){
        return 10000;
    }
        // FIXME: 返回值取决于评估值的下限
    else if(whoWin(whosTurn,simuBoard)==-1){
        return (-1)*10000;
    }
    if(depth == 0){ // 叶节点
        return ((whosTurn==mySide)?1:-1)*evaluate(whosTurn,simuBoard);
    }

    int highestScore = (-1)*INF;
    int negaMaxValue = nonSenseInt;
    auto subBoard = new int[19][19];
    memcpy(subBoard,simuBoard,sizeof(int[19][19]));
    // FIXME: 可能会由于generate函数错误导致连带错误
    vector<Step>* toMove = generateMove(whosTurn,simuBoard);
    re(i,0,(*toMove).size()){
        // move
        subBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = whosTurn;
        subBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = whosTurn;
        negaMaxValue = (-1)*negaMax(1-whosTurn, depth-1,(-1)*alpha,(-1)*beta,subBoard);
        highestScore = max(highestScore,negaMaxValue);
        alpha = max(alpha,negaMaxValue);
        // beta = min(beta,negaMaxValue);
        if(alpha>=beta){ // 隐式剪枝
            return alpha;
        }
        // unmove
        subBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = EMPTY;
        subBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = EMPTY;
    }
    return highestScore;
}
Step aGoodStep(int depth){
//    Step move;
    int highestScore = (-1)*INT_MAX;
    int alpha = (-1)*INF;
    int beta = INF;
    int possibleScore = nonSenseInt;
    Step candidateMove; // 存储暂时评分最高的一步
    auto simuBoard = new int[19][19];
    memcpy(simuBoard,Board,sizeof(int[19][19]));
    vector<Step>* toMove = generateMove(mySide,simuBoard); // 可下的全部位子

    // FIXME: 这里的逻辑其实不是特别清楚
    re(i,0,(*toMove).size()){ // 对全部可下位子进行评估(非静态)
        // move
        simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = mySide;
        simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = mySide;
//        possibleScore = (-1)*negaMax(1-mySide,depth,alpha,beta,simuBoard);
        possibleScore = negaMax(1-mySide,depth,alpha,beta,simuBoard);
        if(possibleScore>highestScore){
            highestScore = possibleScore;
            copyStep(candidateMove,(*toMove)[i]);
        }
        // unmove
        simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = EMPTY;
        simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = EMPTY;
    }

    free(simuBoard);

}

int main()
{
    Step step;//临时步结构
    srand(int(time(0)));

    re(i,0,19)  // 从文件test.in中直接读取棋面
        re(j,0,19){
            cin >> Board[i][j];
        }
    mySide = BLACK; // 直接默认我方执黑,看决策程序能否给出正确的决策结果
    // TODO: 决定搜索深度
    int depth = 3;
    Step toMove;
    toMove = aGoodStep(depth);
    vector<Step>* ptr = generateMove(mySide,Board);
//    toMove.first.x = (*ptr)[0].first.x;
//    toMove.first.y = (*ptr)[0].first.y;
//    toMove.second.x = (*ptr)[0].second.x;
//    toMove.second.y = (*ptr)[0].second.y;
    Board[toMove.first.x][toMove.first.y] = mySide;
    Board[toMove.second.x][toMove.second.y] = mySide;

    cout << "move:" << toMove.first.x+'A' << toMove.first.y+'A' << toMove.second.x+'A' << toMove.second.y+'A' << endl << endl;//输出着法
    re(i,0,19){
            re(j,0,19){
            cout << Board[i][j] << ' ';
        }
        cout << endl;
    }


    return 0;
}

