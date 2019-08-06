### 2维Delaunay三角剖分算法

---

##### 定义：

* Delaunay三角剖分：给出一种“好的”三角网格定义。
  * 满足**空圆特性**和**最大化最小角特性**

* 三角剖分：
  * 假设**V**是二维实数域上的**有限点集**，边e是由点集中的点作为端点构成的封闭线段, **E**为e的集合。那么该点集V的一个三角剖分T=(V,E)是一个平面图G，该平面图满足条件：
    1. 除了端点，平面图中的边不包含点集中的任何点。
    2. 没有相交边。
    3. 平面图中所有的面都是三角面，且所有三角面的合集是散点集V的凸包。
* Delaunay边：
  * 假设E中的一条边e（两个端点为a,b），e若满足下列条件，则称之为Delaunay边：存在一个圆经过a,b两点，圆内(注意是圆内，圆上最多三点共圆)不含点集V中任何其他的点，这一特性又称**空圆特性**。
* Delaunay三角剖分：
  * 定义1：如果点集V的一个三角剖分T**只包含**Delaunay边，那么该三角剖分称为Delaunay三角剖分。
  * 定义2：假设T为V的任一三角剖分，则T是V的一个Delaunay三角剖分，当前仅当**T中的每个三角形的外接圆的内部不包含V中任何的点**。 



##### 集成工具

* 图片三角剖分工具
  * https://zhiyishou.github.io/Polyer/
* JS Delaunay三角形生成工具：
  * https://travellermap.com/tmp/delaunay.htm



##### 参考

* Chinamming网格剖分：https://blog.csdn.net/Chinamming/article/details/16874371
* 纸异兽博客：https://www.cnblogs.com/zhiyishou/p/4430017.html
* eearth Bowyer&Watson逐点插入法：https://www.jianshu.com/p/172749e6116a
* ironwallaby 前端 Delaunay实现：https://github.com/ironwallaby/delaunay
* jeanbroid openGL显示：https://github.com/jeanbroid/delaunay-cpp
* Paul Bourke 逐点插入法：http://paulbourke.net/papers/triangulate/
* Samuel Peterson分治法算法：http://www.geom.uiuc.edu/~samuelp/del_project.html