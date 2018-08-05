#ifndef THREAD_H
#define THREAD_H
#include <QString>
#include <QThread>
#include <QDebug>
#include <QFileDialog>
#include <openssl/md5.h>
#include <openssl/sha.h>


#include "workThread.h"

class workThread : public QThread{
    Q_OBJECT
public:
    workThread(QObject *parent = nullptr);
    QString path;
    QString hashMethod;
    size_t digest_size;
    unsigned char *hashValue;

    void run();
    QString md5();
    QString sha512();
    QString sha256();
    char *QStrToC(QString str);


signals:
    void passHashValue(workThread *);
};

#endif // THREAD_H




