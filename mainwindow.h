#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "workThread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    workThread *t_md5 , *t_sha256 , *t_sha512;

    char *QStrToC(QString str);
    void initial();
    bool isFilePathEmpty();
    void textEditInit(QTextEdit *);
    bool hashVerify(QTextEdit * , QTextEdit *);
    void verifyResultRender();
    QString UCToQStr(unsigned char *str , size_t digest_size);
    void statusBarSetting(const QString *msg);


public slots:
    void getFile();
    void pushHashBtn();
    void render(workThread *);
    void pushClearBtn();

private slots:
    void on_action_author_btn_triggered();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
