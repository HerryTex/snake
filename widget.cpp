#include "widget.h"
#include "ui_widget.h"

#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);



    // 使用定时器
    // 创建定时器
    timer = new QTimer();
    // 关联超时信号（timeout）
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));


    // 改窗口大小
    resize(500,368);

    // 初始化蛇的长度为1
    QRectF rect(300, 180, nodeWidth, nodeHeight);
    m_snake.append(rect);
    addTop();
    addTop();

    // 初始化食物
    addNewFood();
}

Widget::~Widget()
{
    delete ui;
}

// 控制方向
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:        // 上键
        if(moveFlag != DIR_DOWN){
            moveFlag = DIR_UP;
        }

        break;
    case Qt::Key_Down:      // 下键
        if(moveFlag != DIR_UP){
            moveFlag = DIR_DOWN;
        }

        break;
    case Qt::Key_Right:     // 右键
        if(moveFlag != DIR_LEFT){
            moveFlag = DIR_RIGHT;
        }

        break;
    case Qt::Key_Left:      // 左键
        if(moveFlag != DIR_RIGHT){
            moveFlag = DIR_LEFT;
        }


        break;
    case Qt::Key_Space:     // 空格
        if(gameStart == false){
            gameStart = true;
            // 启动定时器
            timer->start(100);  // 每100毫秒发出一次超时信号

        }else{
            gameStart = false;
            timer->stop();
        }

        break;
    default:
        break;
    }
}
// 向上移动
void Widget::addTop()
{
    QPointF leftTop;    // 左上角
    QPointF rightBoton; // 右下角

    // 确定插入位置
    // 判断越界
    if(m_snake[0].y() - nodeHeight < 0){
        // 考虑到蛇会穿墙，因此，当蛇头到边框顶部的时候，方块加到同x坐标的最下面
        leftTop = QPoint(m_snake[0].x(), this->height() - nodeHeight);
        rightBoton = QPointF(m_snake[0].x() + nodeWidth, this->height());

    }else{
        // 正常情况
        leftTop = QPointF(m_snake[0].x(), m_snake[0].y()-nodeHeight);
        rightBoton = m_snake[0].topRight();
    }

    // 插入
    m_snake.insert(0, QRectF(leftTop, rightBoton));

}
// 向下移动
void Widget::addDown()
{
    QPointF leftTop;    // 左上角
    QPointF rightBoton; // 右下角

    // 确定插入位置
    // 判断越界
    if(m_snake[0].y() + nodeHeight * 2 > this->height()){
        leftTop = QPointF(m_snake[0].x(), 0);
        rightBoton = QPointF(m_snake[0].x()+nodeWidth, nodeHeight);
    }else{
        // 正常情况
        leftTop = m_snake[0].bottomLeft();
        rightBoton = m_snake[0].bottomRight() + QPointF(0,nodeHeight);
    }

    // 插入
    m_snake.insert(0, QRectF(leftTop, rightBoton));

}
// 向右移动
void Widget::addRight()
{
    QPointF leftTop;    // 左上角
    QPointF rightBoton; // 右下角

    if(m_snake[0].x() + nodeWidth*2 > this->width()){
        leftTop = QPointF(0, m_snake[0].y());

    }else{
        leftTop = m_snake[0].topRight();
    }

    rightBoton = leftTop + QPointF(nodeWidth, nodeHeight);

    // 插入
    m_snake.insert(0, QRectF(leftTop, rightBoton));

}
// 向左移动
void Widget::addLeft()
{
    QPointF leftTop;    // 左上角
    QPointF rightBoton; // 右下角
    if(m_snake[0].x() - nodeWidth < 0){
        leftTop = QPointF(this->width()-nodeWidth, m_snake[0].y());
    }else{
        leftTop = m_snake[0].topLeft() - QPointF(nodeWidth, 0);
    }

    rightBoton = leftTop + QPointF(nodeWidth, nodeHeight);

    // 插入
    m_snake.insert(0, QRectF(leftTop, rightBoton));
}

// 删除贪吃蛇尾部的方块
void Widget::deleteLast()
{
    m_snake.removeLast();
}

// 渲染，画图
void Widget::paintEvent(QPaintEvent *event)
{
    // 当前窗口的画家
    QPainter painter(this);

    // 画笔
    QPen pen;
    // 画刷
    QBrush brush;

    // 背景图片
    QPixmap pix;
    pix.load(":/img/Snipaste_2024-03-12_23-05.png");
    painter.drawPixmap(0, 0, 500, 368, pix);

    // 画蛇---用画笔,画刷
    pen.setColor(Qt::darkYellow);
    brush.setColor(Qt::darkGreen);
    brush.setStyle(Qt::SolidPattern);
    // 把画笔和画刷告诉画家
    painter.setPen(pen);
    painter.setBrush(brush);

    // 开始画蛇，蛇在链表里面
    for(int i = 0; i < m_snake.length(); i++){
        painter.drawRect(m_snake[i]);
    }

    // 画食物
    pen.setColor(Qt::red);
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    //painter.drawRect(foodNode);
    painter.drawEllipse(foodNode);


    // 检查蛇头是否碰到蛇身，碰到 gameover
    if(checkContact()){
        QFont font("方正舒体", 30, QFont::ExtraLight, false);
        painter.setFont(font);
        painter.drawText((this->width() - 300)/2,
                         (this->height() - 30)/2,
                         QString("GAME OVER!!") );
        // 游戏结束，关闭定时器
        timer->stop();
    }

    QWidget::paintEvent(event);

}

// 在地图随机位置放一个食物
void Widget::addNewFood()
{
    foodNode = QRectF(rand() % (this->width() / 20) * 20,
                      rand() % (this->height() / 20) * 20,
                      nodeWidth,
                      nodeHeight);

}

// 检查蛇头是否碰到蛇身的函数
bool Widget::checkContact()
{
    // 防止蛇身相互碰撞以为蛇头导致gameover
    for( int i = 0; i < m_snake.length(); i++ )
        for(int j = i + 1; j < m_snake.length(); j++){
            if(m_snake[i] == m_snake[j]){
                return true;
            }
        }
    return false;
}

// 超时处理函数
void Widget::timeout()
{
    // 吃苹果，判断有没有重合
    int count = 1;  // 记录标记，如果重合
    // 判断有无相交
    if(m_snake[0].intersects(foodNode)){
        // 吃掉，蛇的长度+1
        count++;
        // 吃掉食物后，刷新食物的位置
        addNewFood();
    }

    // 如果蛇吃到了食物，那就在头部加两次方块表示变长
    while(count--){

        // 让蛇动起来：加入向上走，上面加个小方格，下面删个小方格
        switch (moveFlag) {
            case DIR_UP:
               // 顶部加一个小方块，尾巴删一个小方块
                addTop();
                break;
            case DIR_DOWN:
                addDown();
                break;
            case DIR_RIGHT:
                addRight();
                break;
            case DIR_LEFT:
                addLeft();
                break;
            default:
                break;
        }
    }

    deleteLast();
    update();
}

























