#九宫格怎么玩？
>很久很久以前，一直有人问我，九宫格要怎么玩？那时候就想做一个能教别人怎么玩的游戏？嗯，加油！

## 先总结下自己玩的经验
标准数独解题之旅 http://www.sudokufans.org.cn/forums/topic/8/<br>
参考讲解方式<br>
盗用图 然后用自己的方法解一次<br>
通用数独名词定义 http://www.sudokufans.org.cn/forums/topic/7/<br>

白话版本
扫一遍 发现最多的数字开始扫视
宫内结合横竖排查数字 找到可解的继续循环

进一步 对每格子的可能出现的数字进行判断  如果有两个格子数字相同 或者三个格子数字相同 排除其他格子的可能性

猜测 

总结出来
<img src = post-2-1273929055.png><\img>
方法一：直观排除法 
1）只有一个可能性

大家之前已阅读过数独的规则：在每个单元中，每个数字只能出现一次，那么也就意味着，如果一行已经出现了一个1，这行的其他格就不再有1，利用这个观点，引发出摒除法。


2）余数法

前面我们提到，一格受其所在单元中其他20格的牵制，假如这20格里面已经出现了1-8这8个数字，我们就可以断定这格一定是未出现的唯一数字9。

第二招：限定法 只能出现在哪里 进而排除
1）X-Wing

2）区块摒除法