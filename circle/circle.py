import matplotlib.pyplot as plt
from matplotlib import animation
from matplotlib.ticker import MultipleLocator

radius = int(input("input radius:"))
x_axes = [0]
y_axes = [radius]
x, y = [], []

fig, ax = plt.subplots()
line, = plt.plot([], [], '.-', color='orange')


def draw_grid():
    # 绘制网格
    ax.set_xlim(-radius - 1, radius + 1)
    ax.set_ylim(-radius - 1, radius + 1)
    ax.xaxis.grid(True)
    ax.yaxis.grid(True)
    ax.xaxis.set_major_locator(MultipleLocator(1))
    ax.yaxis.set_major_locator(MultipleLocator(1))
    plt.axis('square')
    return line


def fill_circle():
    # 将八分之一圆补全
    for time in range(3):
        size = len(x_axes)
        for i in range(size):
            x_axes.append(y_axes[size - i - 1])
            y_axes.append(x_axes[size - i - 1])


def Bresenham_circle(r):
    x = 0
    y = r
    d = 1.25 - r
    while x <= y:
        if d < 0:
            d += 2 * x + 3
        else:
            d += 2 * (x - y) + 5
            y -= 1
        x += 1
        x_axes.append(x)
        y_axes.append(y)

    fill_circle()


def animate(step):
    # 绘制动图
    if step > len(x_axes) - 1:
        return line
    x.append(x_axes[step])
    y.append(y_axes[step])
    line.set_data(x, y)
    return line


Bresenham_circle(radius)

ani = animation.FuncAnimation(fig, animate, frames=radius * 7,
                              init_func=draw_grid, interval=150)

plt.show()

ani.save("/Users/liaoqianqian/Desktop/circle.gif", writer='pillow')
