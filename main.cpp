#include "widget.h"

#include <QApplication>
/*
 *      1、定时器，100ms，检查输出超时函数timeout，就开始检查，小方块往前面移动一个位置
 *      2、按键机制，空格暂停
 *      3、渲染机制画图
 *      4、移动的原理，忘哪里移动，就在前面加个小方块，蛇的末尾删掉一个方块，达到移动的效果
 *      5、蛇和苹果有没有相交--有接口
 *
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("贪吃蛇");
    w.setWindowIcon(QIcon(":/img/logo.ico"));
    w.show();
    return a.exec();
}
