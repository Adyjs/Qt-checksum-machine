#include "workThread.h"
#include <QtCore>
#include <stdio.h>
#include <stdlib.h>


workThread::workThread(QObject *parent) : QThread(parent){}

void workThread::run(){
    QMutex mutex;
    mutex.lock();
    QString hashMethod = this->hashMethod;
    //qDebug()<<hashMethod<<" , "<<__LINE__<<" , "<<QThread::currentThreadId();
    if(hashMethod == "md5"){
        this->hashValue = (unsigned char *) malloc( (this->digest_size) * sizeof(char) );
        char *tempPath = QStrToC(this->path);
        FILE *fp;
        char buf[512];
        size_t numRead;
        MD5_CTX md5;
        if( (fp = fopen(tempPath , "r")) == NULL){
            qDebug()<< "fopen error" << " , " <<tempPath<<" , "<< this->path;
            exit(1);
        }
        MD5_Init(&md5);
        while( (numRead = fread(buf , 1 , 512 , fp) ) != 0){
            MD5_Update(&md5 , buf , (unsigned long int) numRead);
        }
        MD5_Final(this->hashValue , &md5);
        fclose(fp);
        emit passHashValue(this);
    }
    if(hashMethod == "sha256"){
        this->hashValue = (unsigned char *) malloc( (this->digest_size) * sizeof(char) );
        char *tempPath = QStrToC(this->path);
        FILE *fp;
        char buf[512];
        size_t numRead;
        SHA256_CTX sha256;
        if( (fp = fopen(tempPath , "r")) == NULL){
            qDebug()<< "fopen error" << " , " <<tempPath<<" , "<< this->path;
            exit(1);
        }
        SHA256_Init(&sha256);
        while( (numRead = fread(buf , 1 , 512 , fp) ) != 0){
            SHA256_Update(&sha256 , buf , (unsigned long int) numRead);
        }
        SHA256_Final(this->hashValue , &sha256);
        fclose(fp);
        emit passHashValue(this);
    }
    if(hashMethod == "sha512"){
        this->hashValue = (unsigned char *) malloc( (this->digest_size) * sizeof(char) );
        char *tempPath = QStrToC(this->path);
        FILE *fp;
        char buf[512];
        size_t numRead;
        SHA512_CTX sha512;
        if( (fp = fopen(tempPath , "r")) == NULL){
            qDebug()<< "fopen error" << " , " <<tempPath<<" , "<< this->path;
            exit(1);
        }
        SHA512_Init(&sha512);
        while( (numRead = fread(buf , 1 , 512 , fp) ) != 0){
            SHA512_Update(&sha512 , buf , (unsigned long int) numRead);
        }
        SHA512_Final(this->hashValue , &sha512);
        fclose(fp);
        emit passHashValue(this);
    }
    mutex.unlock();
}

char *workThread::QStrToC(QString str){
    return str.toLatin1().data();
}

//QString workThread::sha512(){
//    this->hashValue = (unsigned char *) malloc( (this->digest_size) * sizeof(char) );
//    char *tempPath = QStrToC(this->path);
//    FILE *fp;
//    char buf[512];
//    size_t numRead;
//    SHA512_CTX sha512;
//    if( (fp = fopen(tempPath , "r")) == NULL){
//        qDebug()<< "fopen error" << " , " <<tempPath<<" , "<< this->path;
//        exit(1);
//    }
//    SHA512_Init(&sha512);
//    while( (numRead = fread(buf , 1 , 512 , fp) ) != 0){
//        SHA512_Update(&sha512 , buf , (unsigned long int) numRead);
//    }
//    SHA512_Final(this->hashValue , &sha512);
//    fclose(fp);
//    emit passHashValue(this);
//    this->quit();
//}
//QString workThread::sha256(){
//    this->hashValue = (unsigned char *) malloc( (this->digest_size) * sizeof(char) );
//    char *tempPath = QStrToC(this->path);
//    FILE *fp;
//    char buf[512];
//    size_t numRead;
//    SHA256_CTX sha256;
//    if( (fp = fopen(tempPath , "r")) == NULL){
//        qDebug()<< "fopen error" << " , " <<tempPath<<" , "<< this->path;
//        exit(1);
//    }
//    SHA256_Init(&sha256);
//    while( (numRead = fread(buf , 1 , 512 , fp) ) != 0){
//        SHA256_Update(&sha256 , buf , (unsigned long int) numRead);
//    }
//    SHA256_Final(this->hashValue , &sha256);
//    fclose(fp);
//    emit passHashValue(this);
//    this->quit();
//}
//QString workThread::md5(){
//    this->hashValue = (unsigned char *) malloc( (this->digest_size) * sizeof(char) );
//    char *tempPath = QStrToC(this->path);
//    FILE *fp;
//    char buf[512];
//    size_t numRead;
//    MD5_CTX md5;
//    if( (fp = fopen(tempPath , "r")) == NULL){
//        qDebug()<< "fopen error" << " , " <<tempPath<<" , "<< this->path;
//        exit(1);
//    }
//    MD5_Init(&md5);
//    while( (numRead = fread(buf , 1 , 512 , fp) ) != 0){
//        MD5_Update(&md5 , buf , (unsigned long int) numRead);
//    }
//    MD5_Final(this->hashValue , &md5);
//    fclose(fp);
//    emit passHashValue(this);
//    this->quit();
//}
