#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/qss/style.qss");
    if (!file.open(QFile::ReadOnly))  {
        QMessageBox::critical(nullptr,"出现错误",QString("无法打开样式表文件：\n"));
    }
    qApp->setStyleSheet(file.readAll());    //QApplication 是单列模式 qApp 是获取他的宏
    file.close();
    MainWindow w;
    w.show();
    return a.exec();
}
