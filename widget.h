#ifndef WIDGET_H
#define WIDGET_H
#include <QKeyEvent>
#include <QTimer>
#include <QTime>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

//0、蛇的表示----链表
//1、按键处理，改变方向
//2、使用定时器-----因为一开始就要用，使用把他定义在构造函数上
//  2.1 关联信号槽
//  2.2启动定时器
//  2.3实现对应的超时处理函数
//3、

// 方向
enum Direct{
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    // 按键处理
    void keyPressEvent(QKeyEvent* event);

    // 贪吃蛇移动函数
    void addTop();
    void addDown();
    void addRight();
    void addLeft();

    void deleteLast();

    // 渲染，绘图
    void paintEvent(QPaintEvent* event);

    // 添加食物
    void addNewFood();

    // 检查蛇头是否碰到蛇身的函数
    bool checkContact();

 protected slots:
    // 超时处理函数
    void timeout();

private:
    Ui::Widget *ui;

    // 方向变量
    int moveFlag = DIR_UP;

    // 游戏有没有在开始
    bool gameStart = false;

    // 定时器
    QTimer *timer;

   // 蛇----用链表 保存的是位置区域 x坐标，y坐标这些
    QList<QRectF> m_snake;

   // 蛇小方块的宽高 20像素
    int nodeWidth = 15;
    int nodeHeight = 15;

    // 食物
    QRectF foodNode;

};

#endif // WIDGET_H
