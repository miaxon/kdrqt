#include <QCoreApplication>
#include <QDebug>
#include "KDRProcess.h"

void print( const QString& str) {
    qDebug() << "out: " << str;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList cmd = QStringList() << "/root/src/sys/pipe/mine";
    
    KDRProcess proc(cmd);
    QObject::connect(&proc, &KDRProcess::logString, print);
    
    qDebug() << "proc finished";
    return app.exec();

}