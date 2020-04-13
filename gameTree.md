# 这是博弈树的思路文档

Step aGoodStep(int depth)
depth:博弈树搜索深度,人为设置;
返回值为结构体,盛放即将下的两颗棋子位置!

int negaMax(int whosTurn, int depth, int alpha, int beta,int simuBoard[19][19]=Board)
minimax算法的改进算法,参见https://en.wikipedia.org/wiki/Negamax
whosTurn:我方执棋;depth:博弈搜索深度;
alpha用于阿尔法剪枝,递归到某max层决策得到的分数最大值低于alpha,剪掉对应上层我方决策(即max层)的余下枝叶;
beta用于贝塔剪枝,递归到某min层决策得到的分数最小值小于beta,剪掉对应上层敌方决策(即min层)的余下枝叶;
simuBoard:当前棋局.
返回值为对该棋局下假设双方都是纯理智,利益最大化情况下,我方在depth步后得到的"最坏情况下的"最好分数!

我需要调用的函数接口为:
generateMove,提供可下的位置,即:使得博弈树展开.需要注意的是,需要对这些可下位置进行静态评估以达到最好的剪枝效果!
evaluate,静态评估棋局分数,需要注意的是,所有情况下评估的分数都是对我方而言的分数!

# PS:negamax算法
   negmax算法相较于minimax算法的改进在于减少了代码的复用率.
   它的估分在min层和max层间会进行正负号的翻转,使得双方都追求分数的最大.
   比如说,某max层评估下层节点的分数为10,9,8 那么,会选取10
         某min层评估下层节点的分数分别为-10,-9,-8,那么,会选取-8
    这样,双方都是取叶节点估分的max作为对应节点的分数,我们便不必将min层和max的函数拆开分别写.
    此方法由Knuth发明.