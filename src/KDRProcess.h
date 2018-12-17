#ifndef MYPROCESS_H
#define MYPROCESS_H
#include <QObject>
#include <QSocketNotifier>
#include <QString>

class KDRProcess : public QObject{
    Q_OBJECT
public:
    KDRProcess(QStringList cmd);
    virtual ~KDRProcess();
signals:
    void logString(const QString& str);
private:
    FILE* popen ();
    void logOutput(int fd);
    QSocketNotifier *notifier;
    int pid_child;
    FILE *fout;
    char **args;

};

#endif /* MYPROCESS_H */

