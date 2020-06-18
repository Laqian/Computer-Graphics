import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

X_Axis = 64
Y_Axis = 52

image = np.zeros([X_Axis, Y_Axis])
# 设置多边形顶点

polygon = [
    [48, 8],
    [60, 20],
    [48, 44],
    [20, 28],
    [8, 40],
    [12, 16],
    [28, 8]
]

point_num = len(polygon)
non_extreme = []
fig, ax = plt.subplots()
line = plt.plot([], [], '.-', color='orange')


class Node:
    # 定义节点
    def __init__(self, data):
        self._data = data
        self._next = None

    def get_data(self):
        return self._data

    def get_next(self):
        return self._next

    def set_data(self, new_data):
        self._data = new_data

    def set_next(self, new_next):
        self._next = new_next


class SingleLinkList:
    # 定义链表
    def __init__(self):
        # 初始化链表为空
        self._head = None
        self._size = 0

    def get_head(self):
        # 获取链表头
        return self._head

    def is_empty(self):
        # 判断链表是否为空
        return self._head is None

    def insert(self, data, index):
        # 在链表的index处添加节点
        insert = Node(data)
        node = self._head
        pre = node
        if index == 0:
            self._head = insert
        else:
            for i in range(index):
                pre = node
                node = node.get_next()
                i += 1
            pre.set_next(insert)
        insert.set_next(node)

        self._size += 1

    def append(self, data):
        # 在链表尾部追加一个节点
        temp = Node(data)
        if self._head is None:
            self._head = temp
        else:
            node = self._head
            while node.get_next() is not None:
                node = node.get_next()
            node.set_next(temp)
        self._size += 1

    def remove(self, data):
        # 在链表尾部删除一个节点
        node = self._head
        prev = None
        while node is not None:
            if node.get_data() == data:
                if not prev:
                    # 父节点为None
                    self._head = node.get_next()
                else:
                    prev.set_next(node.get_next())
                break
            else:
                prev = node
                node = node.get_next()
        self._size -= 1


def poly_fill(image):
    # 生成多边形填充矩阵
    y_min = Y_Axis
    y_max = 0
    # 确定y的取值范围
    for index, y in enumerate(polygon):
        if y[1] < y_min:
            y_min = y[1]
        if y[1] > y_max:
            y_max = y[1]
    # 根据多边形顶点，初始化并建立ET表
    ET = []
    for i in range(Y_Axis):
        ET.append(None)

    for i in range(y_min, y_max + 1):
        for j in range(point_num):
            # 下端点的纵坐标y值等于i的边，归入第i类
            if polygon[j][1] == i:

                [x0, y0] = polygon[j]
                [x1, y1] = polygon[(j + 1 + point_num) % point_num]

                # 水平边不加入分类边表中
                if y1 == y0:
                    break
                else:
                    dx = (x1 - x0) / (y1 - y0)

                index_y = i
                # 找到ymax和x
                if y1 > y0:
                    ymax = y1
                    x = x0
                    # 非极值点需要做截断处理
                    if [x0, y0] in non_extreme:
                        x = x + dx
                        index_y = index_y + 1
                else:
                    ymax = y0
                    x = x1
                    index_y = y1
                    if [x1, y1] in non_extreme:
                        x = x + dx
                        index_y = index_y + 1


                # 第i类没有边，则直接插入ymax,x,dx
                if ET[index_y] is None:
                    ET[index_y] = SingleLinkList()
                    ET[index_y].append([ymax, x, dx])
                # 同一类中，各边按x值(x值相等时，按dx的值) 递增的顺序排成行
                else:
                    node = ET[index_y].get_head()
                    node_index = 0
                    while node.get_data()[1] < x and node is not None:
                        node = node.get_next()
                        node_index += 1
                    if node.get_data()[1] == x:
                        # 比较dx大小
                        if dx > node.get_data()[2]:
                            node_index += 1
                    ET[index_y].insert([ymax, x, dx], node_index)

    # 建立活性边表
    AEL = SingleLinkList()

    # y初始化
    for y in range(y_min, y_max + 1):

        # 将属于该类的所有边从ET中取出并插入边的活化链表AEL中
        if ET[y] is not None:
            node = ET[y].get_head()
            while node is not None:
                AEL.append(node.get_data())
                node = node.get_next()

        # 填充
        if not AEL.is_empty():
            node = AEL.get_head()
            x_list = []
            # 获取所有交点的x坐标
            while node is not None:
                [_, cur_x, _] = node.get_data()
                x_list.append(cur_x)
                node = node.get_next()

            # 排序
            x_list.sort()

            # 两两配对填充, 1表示被填充
            for i in range(0, len(x_list), 2):
                x1 = x_list[i]
                x2 = x_list[i + 1]
                for pixel in range(int(x1), int(x2) + 1):
                    image[pixel][y] = 1


        # 删除非活性边
        node = AEL.get_head()
        while node is not None:
            [ymax, cur_x, delta_x] = node.get_data()
            if ymax == y:
                AEL.remove([ymax, cur_x, delta_x])
            node = node.get_next()

        # 更新 cur_x
        node = AEL.get_head()
        while node is not None:
            [ymax, cur_x, delta_x] = node.get_data()
            node.set_data([ymax, cur_x + delta_x, delta_x])
            node = node.get_next()


def if_extreme():
    # 处理端点，找到非极值点
    for i in range(point_num):
        y0 = polygon[i][1]
        y1 = polygon[(i + 1 + point_num) % point_num][1]
        y2 = polygon[(i - 1 + point_num) % point_num][1]
        if y0 - y1 > 0 and y0 - y2 < 0 or y0 - y1 < 0 and y0 - y2 > 0:
            non_extreme.append(polygon[i])


def show_polygon():
    for i in range(point_num):
        plt.plot([polygon[i - 1][0], polygon[i][0]], [polygon[i - 1][1], polygon[i][1]], color='skyblue')


def init():
    # 绘制网格
    ax.set_xlim(0, X_Axis)
    ax.set_ylim(0, Y_Axis)
    ax.xaxis.set_major_locator(plt.MultipleLocator(4))  # 设置x主坐标间隔 4
    ax.yaxis.set_major_locator(plt.MultipleLocator(4))  # 设置y主坐标间隔 4
    ax.grid(which='major', axis='x', linewidth=0.75, linestyle='-', color='0.75')  # 由每个x主坐标出发对x主坐标画垂直于x轴的线段
    ax.grid(which='major', axis='y', linewidth=0.75, linestyle='-', color='0.75')
    plt.grid(linestyle='-.')
    # 显示多边形图形
    show_polygon()
    return line


def animate(step):
    # 生成多边形图象动图
    for i in range(X_Axis):
        if image[i][step] == 1:
            ax.scatter(i, step, color='orange')


if_extreme()
poly_fill(image)

ani = animation.FuncAnimation(fig, animate, frames=Y_Axis,
                              init_func=init, interval=150)

plt.show()

ani.save("/Users/liaoqianqian/Desktop/polygen.gif", writer='pillow')
