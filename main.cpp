#include <QApplication>
#include "mainwindow.h"

// 程序入口
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);  // 创建应用程序对象
    MainWindow w;  // 创建主窗口
    w.show();  // 显示主窗口
    return a.exec();  // 启动事件循环
}

