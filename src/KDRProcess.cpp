#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <pty.h>
#include <unistd.h>

#include "KDRProcess.h"

KDRProcess::KDRProcess(QStringList cmd) {
    args = (char**) malloc(sizeof (*args) * cmd.size());
    for (int i = 0; i < cmd.size(); ++i) {
        args[i] = cmd[i].toUtf8().data();
        printf("args %d is %s\n", i, args[i] );
    }
    fout = popen();
}

KDRProcess::~KDRProcess() {
    free(args);
    delete notifier;
    fclose(fout);
    kill(pid_child, SIGKILL);
    printf("Kill process (%s) pid=%d", args[0], pid_child);
}

void KDRProcess::logOutput(int fd) {
     
    char buf[1024] = {0};
    fgets(buf, 1024, fout);
    //printf("Catch child process (%s) pid=%d", buf, pid_child);
    QString line = QString::fromUtf8(buf);
    if (line.length())
        emit logString(line);
}

FILE*
KDRProcess::popen() {
    int pty;
    pid_child = forkpty(&pty, NULL, NULL, NULL);
    switch (pid_child) {
        case -1:
            printf("%s", "pty fork");
        case 0:
            printf("execv %s\n", args[0]);
            if (execv("/root/src/sys/pipe/mine", args) == -1)
                printf("Failed execve (%s): %d, %s", args[0], errno, strerror(errno));
            _exit(1);
        default:
            
            FILE *fp = fdopen(pty, "r");
            if (fp) {
                int fd = fileno(fp);
                printf("pty %d fd %d\n", pty, fd);
                fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
                notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
                connect(notifier, &QSocketNotifier::activated, this, &KDRProcess::logOutput);
                printf("Forking child process (%s) pid=%d", args[0], pid_child);
            }
            return fp;
    }
}