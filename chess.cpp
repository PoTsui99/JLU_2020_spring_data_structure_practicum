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
#include <fstream>
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define INF INT_MAX
#define nonSenseInt -1
// #define C6 1000000
// #define A5 100000
// #define S5 50000
// //#define A4 50000
// #define S4 10000
// #define A3 10000
// #define I3 5000
// #define S3 1000
// #define A2 100
// #define S2 10
#define re(i,a,b) for(int i=a;i<b;++i)
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

using namespace std;
fstream fout("debug.out");
#define cout fout


struct Point { //点结构
    int x, y;
};
struct Point_1 { //点结构
    int x, y;
    int score;
};
struct Step { //步结构
    Point first, second;
    int value;
};
// global:
int Board[19][19]; //存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一
int mySide = BLACK; // 方便起见,增加我方
Step toGo;
int maxDepth = 3;
// definition:
// void initialize(Step& S);
void copyStep(Step& to, Step& from);
bool sortByM1(const Step& v1, const Step& v2);
bool isInRange(int x, int y);
void ROW(int path[8], int m, int n, int color, int sim[19][19]);
void COL(int path[8], int m, int n, int color, int sim[19][19]);
void Diagonal(int path[8], int m, int n, int color, int sim[19][19]);
void exDiagonal(int path[8], int m, int n, int color, int sim[19][19]);
int compare7(int path[7]);
int compare8(int path[8]);
void numberReturn(int color, int CS[8], int simuBoard[19][19]);
int placeNotEmpty(int simuBoard[19][19] = Board);
int whoWin(int side = mySide, int simuBoard[19][19] = Board);
bool hasNeighbor(int x, int y, int simuBoard[19][19]);
int getValue(int x, int y, int computerSide, int simuBoard[19][19]);
vector<Step>* generateMove(int computerSide, int simuBoard[19][19] = Board);
int negaMax(int whosTurn, int depth, int alpha, int beta, int simuBoard[19][19]);
void aGoodStep(int depth);
void ROW(int path[8], int sim[19][19], int m, int n, int color);
void COL(int path[8], int sim[19][19], int m, int n, int color);
void Diagonal(int path[8], int sim[19][19], int m, int n, int color);
void exDiagonal(int path[8], int sim[19][19], int m, int n, int color);
int compare7(int path[7]);
int compare8(int path[8]);
void numberReturn(int simuBoard[19][19], int color, int CS[8]);
int ifwin(int simuBoard[19][19], int computerside);
int evaluate(int computerside, int myBoard[19][19]);//整体局面估分
void ROW(int path[8], int sim[19][19], int m, int n, int color)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m][n + i];
    }
}
void COL(int path[8], int sim[19][19], int m, int n, int color)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m + i][n];
    }
}
void Diagonal(int path[8], int sim[19][19], int m, int n, int color)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m + i][n + i];
    }
}
void exDiagonal(int path[8], int sim[19][19], int m, int n, int color)
{
    for (int i = 0; i < 8; i++)
    {
        path[i] = sim[m + i][n - i];
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
        for (int i = 5; i >= 0; i--)
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
void numberReturn(int simuBoard[19][19], int color, int CS[8])
{
    int number = 0;
    int num = 0;
    int r = 0;
    int path[8] = { 0,0,0,0,0,0,0,0 };
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            ROW(path, simuBoard, i, j, color);
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
            COL(path, simuBoard, i, j, color);
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
            Diagonal(path, simuBoard, i, j, color);
            r = compare8(path);
            if (r != -1)
            {
                CS[r]++;
            }
        }

    }
    //斜向判断
    for (int i = 0; i < 11; i++)
    {
        for (int j = 7; j < 19; j++)
        {
            exDiagonal(path, simuBoard, i, j, color);
            r = compare8(path);
            if (r != -1)
            {
                CS[r]++;
            }
        }
    }
    return;
}
int compare6(int path[6], int color, int CS[8])
{
    int r = 0;
    int num = 0;
    int number = 0;
    for (int i = 0; i < 6; i++)
    {
        if (path[i] == color) path[i] = 1;
        else if (path[i] == 2) path[i] = 0;
        else path[i] = -1;
    }
    int S5[6] = { 1,1,1,1,1,0 };
    r = equal(path, path + 6, S5);
    if (r == 1) return 2;
    if (path[5] == 0)
    {
        for (int i = 0; i < 5; i++)
        {
            if (path[i] == -1) num++;
            if (path[i] == 1) number++;
        }
        if (number + num < 5) return -1;
        if (number == 5) return 2;
        if (number == 4) return 3;
        if (number == 3) return 5;
        if (number == 2) return 7;
        if (number == 1) return 7;
        if (number == 0) return -1;
    }
    num = 0;
    number = 0;
    for (int i = 0; i < 5; i++)
    {
        if (path[i] == -1) num++;
        if (path[i] == 1) number++;
    }
    if (number == 5) return 2;
    if (number == 4) return 3;
    if (number == 3) return 5;
    if (number == 2) return 7;
    if (number == 1) return 7;
    if (number == 0) return -1;
    return -1;
}
int final_evaluate(int computerside, int simuBoard[19][19] = Board)//整体局面估分
{
    int CS[8] = { 0,0,0,0,0,0,0,0 };
    int value[8] = { 10000000,10000000,500000,70000,9000,1100,550,60 };
    int path[7] = { 0,0,0,0,0,0,0 };
    int path6[6] = { 0,0,0,0,0,0 };
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            if (simuBoard[i][j] == computerside) simuBoard[i][j] = 1;
            else if (simuBoard[i][j] == 2) simuBoard[i][j] = 0;
            else simuBoard[i][j] = -1;
        }
    }
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
    for (int i = 0; i < 12; i++)
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
    for (int i = 0; i < 12; i++)
    {
        for (int j = 7; j < 19; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                path[k] = simuBoard[i + k][j - k];
            }
            int r = compare7(path);
            if (r != -1) CS[r]++;
        }
    }
    for (int i = 0; i < 19; i++)
    {
        int j = 0;
        for (int k = 0; k < 6; k++)
        {
            path6[k] = simuBoard[i][j + k];
        }
        int r = compare6(path6, computerside, CS);
        if (r != -1) CS[r]++;
        j = 13;
        for (int k = 0; k < 6; k++)
        {
            path6[k] = simuBoard[i][j + k];
        }
        r = compare6(path6, computerside, CS);
        if (r != -1) CS[r]++;
    }
    for (int i = 0; i < 19; i++)
    {
        int j = 0;
        for (int k = 0; k < 6; k++)
        {
            path6[k] = simuBoard[j + k][i];
        }
        int r = compare6(path6, computerside, CS);
        if (r != -1) CS[r]++;
        j = 13;
        for (int k = 0; k < 6; k++)
        {
            path6[k] = simuBoard[j + k][i];
        }
        r = compare6(path6, computerside, CS);
        if (r != -1) CS[r]++;
    }
    for (int i = 0; i < 13; i++)
    {
        int j = 0;
        for (int k = 0; k < 6; k++)
        {
            path6[k] = simuBoard[i + k][j + k];
        }
        int r = compare6(path6, computerside, CS);
        if (r != -1) CS[r]++;
    }
    numberReturn(simuBoard, computerside, CS);
    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        score = score + CS[i] * value[i];
    }
    return score;
}
int evaluate(int computerside, int simuBoard[19][19] = Board)
{
    int myscore = final_evaluate(computerside, simuBoard);
    int yourcolor = 0;
    if (computerside == 1) yourcolor = 0;
    else yourcolor = 1;
    int yourscore = final_evaluate(yourcolor, simuBoard);
    return myscore - yourscore;
}//我方估分-对方估分

void copyStep(Step& to, Step& from) { // 对Step进行数值拷贝
    to.first.x = from.first.x;
    to.first.y = from.first.y;
    to.second.x = from.second.x;
    to.second.y = from.second.y;
}
bool isInRange(int x, int y) {
    return (x >= 0 && x < 19 && y >= 0 && y < 19);
}
int placeNotEmpty(int simuBoard[19][19]) {
    int cnt = 0;
    re(i, 0, 19)
        re(j, 0, 19) {
            if (simuBoard[i][j] != EMPTY) {
                ++cnt;
            }
        }
    return cnt;
}
int whoWin(int side, int simuBoard[19][19]) { // 返回1为我方赢,-1为地方赢
    /* (0,0) (0,1) (0,2)....
     * .
     * .
     * (18,0) (18,1)........(18,18)
     * */
    int dir[4][2] = { 0, 1, 1, 0, 1, 1, 1, -1 };

    int cnt = 0;
    re(i, 0, 19)
        re(j, 0, 19) { // 遍历
            if (simuBoard[i][j] == side) { // 发现我方子
                re(k, 0, 4) { // 四个方向
                    cnt = 0;
                    re(l, 0, 6) { // 循环,查看六连否
                        if (isInRange(i + l * dir[k][0], j + l * dir[k][1])) {
                            if (simuBoard[i + l * dir[k][0]][j + l * dir[k][1]] != side) {
                                break;
                            }
                            else ++cnt; // 计数器+1
                        }
                        else break; // 超界
                        if (cnt == 6)
                            return 1;
                    }
                }
            }
            else if (simuBoard[i][j] == 1 - side) {
                re(k, 0, 4) {
                    cnt = 0;
                    re(l, 0, 6) {
                        if (isInRange(i + l * dir[k][0], j + l * dir[k][1])) {
                            if (simuBoard[i + l * dir[k][0]][j + l * dir[k][1]] != 1 - side) {
                                break;
                            }
                            else ++cnt;
                        }
                        else break;
                        if (cnt == 6)
                            return -1;
                    }
                }
            }
        }
    return 0; // 未分胜负
}
bool hasNeighbor(int x, int y, int simuBoard[19][19] = Board) { // 存储合法的走法
    int direction_x[7] = { 0, 1, 2, 3, -1, -2, -3 };
    int direction_y[7] = { 0, 1, 2, 3, -1, -2, -3 };
    for (int i = 0; i < 7; i++)
        for (int j = 1; j < 7; j++)
            if (((y + direction_y[j]) < 19) && ((x + direction_x[i]) < 19) && ((x + direction_x[i]) >= 0) && (((y + direction_y[j]) >=0 ))&& (simuBoard[x + direction_x[i]][y + direction_y[j]] != EMPTY))
                return true;
    for (int i = 1; i < 7; i++)
        if (((x + direction_x[i]) < 19) && ((x + direction_x[i]) >= 0)&& (simuBoard[x + direction_x[i]][y] != EMPTY))
            return true;
    return false;
}
int getValue(int x, int y, int computerSide, int simuBoard[19][19] = Board) { // 棋盘修改为局部变量
    if (computerSide == BLACK) {
        simuBoard[x][y] = 0;
        int score = evaluate(computerSide, simuBoard);
        simuBoard[x][y] = 2;
        return score;
    }
    simuBoard[x][y] = 1;
    int score = evaluate(computerSide, simuBoard);
    simuBoard[x][y] = 2;
    return score;

}
bool sortByM1(const Step& v1, const Step& v2) { //注意：本函数的参数的类型一定要与vector中元素的类型一致
    return v1.value < v2.value;//升序排列
}
// 两步的
vector<Step>* generateMove(int computerSide, int simuBoard[19][19]) {
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
    for (int i = 0; i < 19; i++)
        for (int j = 1; j < 19; j++) {
            if (simuBoard[i][j] == 2) {
                if (hasNeighbor(i, j, simuBoard)) {
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
    for (int i = 0; i < validCondition.size(); i++)
        for (int j = i + 1; j < validCondition.size(); j++) {
            Step temp;
            temp.first.x = validCondition[i].x;
            temp.first.y = validCondition[i].y;
            temp.second.x = validCondition[j].x;
            temp.second.y = validCondition[j].y;
            temp.value = validCondition[i].score + validCondition[j].score;
            toReturn->push_back(temp);
        }
    //将toReturn里面的元素按评估分数升序排列
    std::sort(toReturn->begin(), toReturn->end(), sortByM1);
    return toReturn;
}
int negaMax(int whosTurn, int depth, int alpha, int beta, int simuBoard[19][19]) {
    cout << "进行一次negamax" << endl;
    int flag = whoWin(whosTurn, simuBoard);
    // 检测到平局
    if (flag == 0 && placeNotEmpty(simuBoard) == 19 * 19)
        return 0;
        // 检测到胜局
    else if (flag == 1) {
        return INF;
    }
        // 检测到输局
    else if (whoWin(whosTurn, simuBoard) == -1) {
        return (-1) * INF;
    }
    if (depth == 0) { // 叶节点
        int rt = ((whosTurn == mySide) ? 1 : -1) * evaluate(mySide, simuBoard);
        return rt;
    }

    // int highestScore = (-1) * INF;
    int negaMaxValue = nonSenseInt;
    //auto subBoard = new int[19][19];
    //memcpy(subBoard, simuBoard, sizeof(int[19][19]));
    vector<Step>* toMove = generateMove(whosTurn, simuBoard);
    re(i, 0, (*toMove).size()) {
        // move
        simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = whosTurn;
        simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = whosTurn;
        negaMaxValue = (-1) * negaMax(1 - whosTurn, depth - 1, (-1) * beta, (-1) * alpha, simuBoard);
        // highestScore = max(highestScore, negaMaxValue);
        if (negaMaxValue > alpha) {
            alpha = negaMaxValue;
            if (depth == maxDepth) {
                copyStep(toGo, (*toMove)[i]);
            }
        }
        if (alpha >= beta) { // 隐式剪枝
            return alpha;
        }
        // unmove
        simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = EMPTY;
        simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = EMPTY;
    }
    return alpha;
}
void aGoodStep(int depth) {
    //    Step move;
    //int highestScore = (-1) * INT_MAX;
    int alpha = (-1) * INF;
    int beta = INF;
    //int possibleScore = nonSenseInt;
    //Step candidateMove; // 存储暂时评分最高的一步
    auto simuBoard = new int[19][19];
    memcpy(simuBoard, Board, sizeof(int[19][19]));
    vector<Step>* toMove = generateMove(mySide, simuBoard); // 可下的全部位子
    copyStep(toGo, (*toMove)[0]);
    negaMax(mySide, depth, alpha, beta, simuBoard);
    //    re(i,0,(*toMove).size()){
    //        cout << "当前评估局面落子:";
    //        cout << "(" << (*toMove)[i].first.x << "," << (*toMove)[i].first.y << ")";
    //        cout << "(" << (*toMove)[i].second.x << "," << (*toMove)[i].second.y << ")";
    //        cout << " value: " << (*toMove)[i].value;
    //        cout << endl;
    //    }

    //re(i, 0, (*toMove).size()) { // 对全部可下位子进行评估(非静态)
    //    // move
    //    cout << (*toMove)[i].first.x << "&" << (*toMove)[i].first.y << "&" << (*toMove)[i].second.x << "&" << (*toMove)[i].second.y << endl;
    //    simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = mySide;
    //    simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = mySide;
    //    //这些局面实际上是min层
    //    possibleScore = (-1) * negaMax(1 - mySide, depth, alpha, beta, simuBoard);
    //    cout << "这个局面的估值为: " << possibleScore << endl;
    //    if (possibleScore > highestScore) {
    //        highestScore = possibleScore;
    //        copyStep(candidateMove, (*toMove)[i]);
    //    }
    //    //cout << endl << "还剩" << (*toMove).size() - i - 1 << "个未评估." << endl;
    //    /*cout << "当前candidateMove: (" << candidateMove.first.x << "," << candidateMove.first.y << ")";
    //    cout << ", (" << candidateMove.second.x << "," << candidateMove.second.y << ")" << endl;*/
    //    // unmove
    //    simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = EMPTY;
    //    simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = EMPTY;
    //}
    delete(simuBoard);
    /*Step* rt = new Step(candidateMove);

    return rt;*/
}
bool isAvailable(int x,int y,int dx,int dy,int cnt) {
    re(i,1,6-cnt+1) {
        if(!isInRange(x-i*dx,y-i*dx)||Board[x-i*dx][y-i*dy]!=EMPTY)
            return false;
    }
    return true;
}
void findStep(int side = mySide, int simuBoard[19][19] = Board) {
    int dir[4][2] = {0, 1, 1, 0, 1, 1, 1, -1};

    int cnt = 0;
    int maxConnect = 0;
    Step mem;
    // initialize
    re(i, 0, 19)re(j, 0, 18) {
            if (Board[i][j] == EMPTY && Board[i][j + 1] == EMPTY) {
                mem.first.x = mem.second.x = i;
                mem.first.y = j;
                mem.second.y = j + 1;
                goto loopOut;
            }
        }
    loopOut:
    re(i, 0, 19)re(j, 0, 19) { // 遍历
            if (simuBoard[i][j] == mySide) { // 发现我方子
                re(k, 0, 4) { // 四个方向
                    cnt = 0;
                    re(l, 0, 5) { // 循环,查看连子,最多五个连子
                        if (isInRange(i + l * dir[k][0], j + l * dir[k][1])) {
                            if (simuBoard[i + l * dir[k][0]][j + l * dir[k][1]] != side) { // 不连了怎么样
                                // 必胜
                                if (cnt == 4) {
                                    if (isInRange(i - 1 * dir[k][0], j - 1 * dir[k][1]) &&
                                        isInRange(i - 2 * dir[k][0], j - 2 * dir[k][1]) &&
                                        Board[i - 1 * dir[k][0]][j - 1 * dir[k][1]] == EMPTY &&
                                        Board[i - 2 * dir[k][0]][j - 2 * dir[k][1]] == EMPTY) {
                                        toGo.first.x = i - 1 * dir[k][0];
                                        toGo.first.y = j - 1 * dir[k][1];
                                        toGo.second.x = i - 2 * dir[k][0];
                                        toGo.second.y = j - 2 * dir[k][1];
                                        return;
                                    }

                                }
                                if (cnt == 5) {
                                    if (isInRange(i - 1 * dir[k][0], j - 1 * dir[k][1]) &&
                                        Board[i - 1 * dir[k][0]][j - 1 * dir[k][1]] == EMPTY) {
                                        toGo.first.x = i - 1 * dir[k][0];
                                        toGo.first.y = j - 1 * dir[k][1];
                                        return;
                                    }
                                }
                                if (isAvailable(i, j, dir[k][0], dir[k][1], cnt)) {
                                    if (cnt > maxConnect) {
                                        maxConnect = cnt;
                                        mem.first.x = i - 1 * dir[k][0];
                                        mem.first.y = j - 1 * dir[k][1];
                                        mem.second.x = i - 2 * dir[k][0];
                                        mem.second.y = j - 2 * dir[k][1];
                                    }
                                }

                            } else ++cnt; // 在界内且是我方,计数器+1
                        } else { // 超界了怎么样
                            if (cnt == 4) {
                                if (isInRange(i - 1 * dir[k][0], j - 1 * dir[k][1]) &&
                                    isInRange(i - 2 * dir[k][0], j - 2 * dir[k][1]) &&
                                    Board[i - 1 * dir[k][0]][j - 1 * dir[k][1]] == EMPTY &&
                                    Board[i - 2 * dir[k][0]][j - 2 * dir[k][1]] == EMPTY) {
                                    toGo.first.x = i - 1 * dir[k][0];
                                    toGo.first.y = j - 1 * dir[k][1];
                                    toGo.second.x = i - 2 * dir[k][0];
                                    toGo.second.y = j - 2 * dir[k][1];
                                    return;
                                }

                            }
                            if (cnt == 5) {
                                if (isInRange(i - 1 * dir[k][0], j - 1 * dir[k][1]) &&
                                    Board[i - 1 * dir[k][0]][j - 1 * dir[k][1]] == EMPTY) {
                                    toGo.first.x = i - 1 * dir[k][0];
                                    toGo.first.y = j - 1 * dir[k][1];
                                    return;
                                }
                            }
                            if (isAvailable(i, j, dir[k][0], dir[k][1], cnt)) {
                                if (cnt > maxConnect) {
                                    maxConnect = cnt;
                                    mem.first.x = i - 1 * dir[k][0];
                                    mem.first.y = j - 1 * dir[k][1];
                                    mem.second.x = i - 2 * dir[k][0];
                                    mem.second.y = j - 2 * dir[k][1];
                                }
                            }
                        }
                    }
                }
            } //else if (simuBoard[i][j] == 1 - side) {
//                re(k, 0, 4) {
//                    cnt = 0;
//                    re(l, 0, 6) {
//                        if (isInRange(i + l * dir[k][0], j + l * dir[k][1])) {
//                            if (simuBoard[i + l * dir[k][0]][j + l * dir[k][1]] != 1 - side) {
//                                break;
//                            } else ++cnt;
//                        } else break;
//
//                    }
//                }
//            }
        }
    copyStep(toGo,mem);
    return;

}

int main()
{
    Step step;//临时步结构
    char message[256];//通信消息缓冲
    int computerSide;//己方执棋颜色
    int start=0;//对局开始标记
    srand(int(time(0)));
    //此处放置初始化代码
    //...

    while (1)	//程序主循环
    {
        fflush(stdout);//不要删除此语句，否则程序会出问题
        scanf("%s", message);//获取平台命令消息
        //分析命令
        if (strcmp(message, "name?") == 0)//向对战平台发送队名
        {
            fflush(stdin);
            /***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
            /*******/		printf("name 令狐冲\n");		/**只修改令狐冲，不要删除name空格****/
            /***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
        }
        else if (strcmp(message, "new") == 0)//建立新棋局
        {
            int i, j;
            scanf("%s", message);//获取己方执棋颜色
            fflush(stdin);
            if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执黑
            else  computerSide = WHITE;   //执白

            for (i = 0; i<19; ++i)   //初始化棋局
                for (j = 0; j<19; ++j)
                    Board[i][j] = EMPTY;
            start = 1;

            if (computerSide == BLACK)
            {
                /**********生成第一手着法，并保存在step结构中，落子坐标为(step.first.x,step.first.y)**********/
                /****************************在下方填充代码，并替换我的示例代码******************************/

                // 开局无需修改
                step.first.x = 9;
                step.first.y = 9;


                /******************************在上面填充第一步行棋代码*******************************************/

                Board[step.first.x][step.first.y] = computerSide;//处理己方行棋
                printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
            }
        }
        else if(strcmp(message,"move")==0)//行棋,本程序核心
        {
            scanf("%s", message);//获取对手行棋着法
            fflush(stdin);
            step.first.x=message[0]-'A';		step.first.y=message[1]-'A';
            step.second.x=message[2]-'A';		step.second.y=message[3]-'A';
            //处理对手行棋
            Board[step.first.x][step.first.y] = 1 - computerSide;
            if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.x][step.second.y] = 1 - computerSide;

            /**********************************************************************************************************/
            /***生成落子的坐标，保存在step结构中，第1子下在(step.first.x,step.first.y)，第2子下在(step.first.x,step.first.y)*****/
            /**************************************在下方填充代码，并替换我的示例代码*****************************************/
            mySide = computerSide;
            //vector<Step>* candidate = generateMove(computerSide,Board);
            //copyStep(step,(*candidate)[0]);
            //aGoodStep(1);
            findStep();
            copyStep(step,toGo);
            //生成第1子落子位置step.first.x和step.first.y
//            int x, y;
//            x = rand() % 19; y = rand() % 19;
//            while (Board[x][y] != EMPTY)
//            {
//                x = rand() % 19;	y = rand() % 19;
//            }
//            step.first.x = x;
//            step.first.y = y;
            Board[step.first.x][step.first.y] = computerSide;

            //生成第2子落子位置step.second.x和step.second.y
//            x = rand() % 19;	y = rand() % 19;
//            while (Board[x][y] != EMPTY)
//            {
//                x = rand() % 19;	y = rand() % 19;
//            }
//            step.second.x = x;
//            step.second.y = y;
            Board[step.second.x][step.second.y] = computerSide;

            /*****************************************在上面填充代码******************************************************/
            /**********************************************************************************************************/

            printf("move %c%c%c%c\n",step.first.x+'A',step.first.y+'A',step.second.x+'A',step.second.y+'A');//输出着法
        }
        else if (strcmp(message, "error") == 0)//着法错误
        {
            fflush(stdin);
        }
        else if (strcmp(message, "end") == 0)//对局结束
        {
            fflush(stdin);
            start = 0;
        }
        else if (strcmp(message, "quit") == 0)//退出引擎
        {
            fflush(stdin);
            printf("Quit!\n");
            break;
        }
    }
    return 0;
}
