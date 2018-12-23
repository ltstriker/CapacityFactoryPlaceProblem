# 工厂选址问题

## 项目地址
- [项目地址](https://github.com/ltstriker/CapacityFactoryPlaceProblem)  

- [解文件](https://github.com/ltstriker/CapacityFactoryPlaceProblem/tree/master/FactoryPlaceProblemTest/data)为以res结尾的txt文件，而最后数字为解的方法（0为贪心，1为模拟退火）  

- [结果文件](https://github.com/ltstriker/CapacityFactoryPlaceProblem/tree/master/FactoryPlaceProblemTest)为total_res开头的txt文件，sa为模拟退火，另一个为贪心。

## 介绍  
Suppose there are n facilities and m customers. We wish to choose:   
    (1) which of the n facilities to open   
    (2) the assignment of customers to facilities   

The objective is to minimize the sum of the opening cost and the assignment cost.   
The total demand assigned to a facility must not exceed its capacity. 

## 数据集  
There are currently 71 data files. The format of these data files is:

|J| |I|  
s_1 f_1  
s_2 f_2  
  ...  
s_|J| f_|J|  
d_1 d_2 d_3 … d_|I|  
c_{11} c_{12} c_{13} … c_{1|I|}   
c_{21} c_{22} c_{23} … c_{2|I|}   
   ...    ...    ...   ....  
c_{|J|1} c_{|J|2} c_{|J|3} … c_{|J||I|}   

where:

|J| is the number of potential facility locations;  
|I| is the number of customers;  
s_j (j=1,...,|J|) is the capacity of facility j;  
f_j (j=1,...,|J|) is the fixed cost of opening facility j;  
d_i (i=1,...,|I|) is the demand of customer i;  
c_{ji} (j=1,...,|J|), (i=1,...,|I|) is the cost of allocating all the demand of customer i to facility j.  

---

## 解法  

#### 目标  

- 花费最少  

- 约束  
  * 工厂可以有一些不开  
  * 所有客户的需求都要满足  
  * 工厂满足的需求之和不能超过容量


- 假设需求是不可切分的  

#### 贪心算法  
先假设全部工厂都要开。    

然后不断选择额外开销最小的一个c，如果这个顾客还没被选择而且这个工厂还有足够的容量，那么就让他选择这个工厂。  

最后回头看看有没有工厂没有被选到的，没有被选到的就关掉。  

这样的话，因为每次找最小的工厂都要customer * customer * factory * factory的寻找，开销还是太大了一些。

改成先是无视容量，都选择对每个顾客的最小的那个开销;  
这样的话，每轮只需要customer * customer * factory的开销。  
然后计算每个工厂的容量是否足够。
选出不足够的工厂，对于每个工厂依次删除掉消耗与需求之比最大的那个顾客（同时把顾客对这个工厂的开销改成int_max，以避免再次选中)，直到这个工厂容量足够（更新工厂容量为剩余容量。  
然后再次回到开头，对于还没选择的顾客去选工厂。直到工厂都满足容量要求。  
这个时候应该就要想到是否存在某个顾客的需求恰好得不到满足的情况，这个理论上当然存在的啦。但是。。。从实际来看并没有遇到这个问题  

这个时候解得的结果，看着还行，跟网上看到的结果相差了大概10~60之间，一般在25左右。  
而运算时间的话则是比较的快，大部分算例都在一秒钟之内解决，而少部分为1秒到两秒之间  

#### 模拟退火  

仍然先假设全部工厂都开启  

先随机找个初始解（目前是使用上面的贪心方法先找个解）

然后就是估价函数，直接用花销就好了

邻域操作的话（如果容量不支持这次操作则取消）  

- 一个顾客转移到另一个工厂  
- 两个顾客交换工厂  

然后通过1.0 / (1 + exp((newCost - cost) / T))计算其容许的变差了的新解的通过概率。然后进行更新。  

为了能够取得搜索过的最好的解，增加了回溯功能。  
然后收敛条件设定为一定步数内都没有更新最好解。  


这个时候解得的结果较差，只有很少的算例能够取得更好的结果。由于回溯，大部分都是回到了贪心算到的结果之中。  
时间的话大多在1分钟以内，最大的去到了两分钟。  
这个结果不够好的原因，判断是邻域操作不够好，还是少了点。其实也觉得是因为了这个容量限制了许多邻域操作，导致移动的时候收到了过多的限制导致的。