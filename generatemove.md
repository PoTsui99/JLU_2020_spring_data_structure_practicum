这是走法模块函数的文档：</br>
分为你们需要调用的generateMove函数 和该函数内部调用的hasNeighbor函数，getValue函数，sort函数</br>

bool hasNeighbor(int x,int y)</br>
hasNeighbor函数思路：</br>
接受x,y坐标，</br>
在该点拓展查询四周是否有邻居</br>
把所有周围有邻居的空位认为是合适的位子,返回true</br>
有邻居的定义是：想个四步以内至少有一个不为空的点即可</br>
所以需要一个判断周围是否有邻居的函数</br>

int getValue(int x ,int y ,int computerSide)</br>
getValue函数思路：</br>
接受x，y坐标，和当前走棋方</br>
Alpha Beta剪枝的效率是取决于每一层节点的顺序的。 我们肯定是无法精确排序的，因为每一个节点的值并不能直接计算出来，需要递归计算子节点</br>
 但是我们依然能对节点进行大致的一个排序。前面说过了，只要有一个大致的排序 其实就能很好的提升剪枝效率。</br>
那么如何排序呢？就是给所有待搜索的位置进行打分，按照分数的高低来排序。注意这个打分算法是对某一个空位进行打分.是静态评估单个点的函数</br>

void generateMove(int computerSide)</br>
generateMove函数思路:</br>
传入的参数为 当前走棋方</br>
moveCondition数组的一个元素代表一个走法</br>
通过修改currentSize记录数组有多少可用元素 </br>
遍历整个棋盘，发现为空的点，在判断是否有邻居，如果有邻居就可以认为是合理的下棋点</br>
然后将这些下棋点存起来，存的时候同时存这些点的估值</br>
两两组合，形成一个Step，然后把这些Step在赋值value</br>
按这些value进行排序</br>




