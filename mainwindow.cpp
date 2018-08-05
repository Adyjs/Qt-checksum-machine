#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QMessageBox>

static bool md5Locker , sha256Locker , sha512Locker;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    initial();
}

MainWindow::~MainWindow(){
    delete ui;
}

/*
QString number = QString("%1").arg(yourNumber, 5, 10, QChar('0'));*/

void MainWindow::render(workThread *thr){
    QMutex mutex;
    mutex.lock();
    size_t digestSize = thr->digest_size;
    QString method = thr->hashMethod;
    QString hashValue = UCToQStr(thr->hashValue , digestSize );
    QLabel *resultShow = ui->result_label;
    QStatusBar *status = ui->statusBar;

    if(method == "md5"){
        ui->md5_hash_value->setText(hashValue);
        status->clearMessage();
        status->showMessage("MD5 hash done.");
        md5Locker = true;
    }
    if(method == "sha256"){
        ui->sha256_hash_value->setText(hashValue);
        status->clearMessage();
        status->showMessage("SHA256 hash done.");
        sha256Locker = true;
    }
    if(method == "sha512"){
        ui->sha512_hash_value->setText(hashValue);
        status->clearMessage();
        status->showMessage("SHA512 hash done.");
        sha512Locker = true;
    }

    bool input = ui->verify_textedit->toPlainText().isEmpty();
    resultShow->setStyleSheet("background-color: #eee ; color:#fff");
    resultShow->setText("");
    if( (!input) && md5Locker && sha256Locker && sha512Locker ){
        status->clearMessage();
        status->showMessage("all hash & verify works done.");
        verifyResultRender();
    }
    else{
        status->clearMessage();
        status->showMessage("all hash works done.");
    }
    mutex.unlock();
}

bool MainWindow::hashVerify(QTextEdit *base , QTextEdit *target){
    /*translate input hash to lowercase , and compare with file hash value*/
    return QString::compare(base->toPlainText().toLower() , target->toPlainText());
}

void MainWindow::verifyResultRender(){
    QLabel *resultShow = ui->result_label;
    resultShow->setText("");

    QTextEdit *input , *md5 , *sha256 , *sha512;
    input = ui->verify_textedit;
    md5 = ui->md5_hash_value;
    sha256 = ui->sha256_hash_value;
    sha512 = ui->sha512_hash_value;
    //qDebug()<<__LINE__<<" input :"<<input->toPlainText()<< ",\nmd5 : "<<md5->toPlainText()<<",\nsha256 : "<<sha256->toPlainText()<<",\nsha512 : "<<sha512->toPlainText();
    if( hashVerify(input, md5) == 0 ){
        resultShow->setText("MD5\n符合");
        resultShow->setStyleSheet("background-color: #eee ; color:#43b738");
    }
    else if( hashVerify(input, sha256) == 0 ){
        resultShow->setText("SHA256\n符合");
        resultShow->setStyleSheet("background-color: #eee ; color:#43b738");
    }
    else if( hashVerify(input, sha512) == 0 ){
        resultShow->setText("SHA512\n符合");
        resultShow->setStyleSheet("background-color: #eee ; color:#43b738");
    }
    else{
        resultShow->setText("皆不符合");
        resultShow->setStyleSheet("background-color: #eee ; color:#ff0000");
    }
}

char *MainWindow::QStrToC(QString str){
    return str.toLatin1().data();
}

QString MainWindow::UCToQStr(unsigned char *str , size_t digest_size){
    QString s;
    QString result = "";
    for ( size_t i = 0; i < digest_size; i++){
        s = QString("%1").arg(str[i],2,16 , QChar('0'));
        if(s == "0"){
            s="00";
        }
        result.append(s);
    }
    //result = QString("%1").arg(yourNumber, 5, 10, QChar('0'));
    return result;
}

bool MainWindow::isFilePathEmpty(){
    bool empty = ui->file_path_lineedit->text().isEmpty();
    if(empty){
        ui->hash_btn->setStyleSheet("background-color: #ddd");

        return empty;
    }
    ui->hash_btn->setStyleSheet("background-color: #43b738");
    return empty;
}

void MainWindow::getFile(){
    QString file =  QFileDialog::getOpenFileName(this , tr("Choose File") , "/home", "");
    ui->file_path_lineedit->setText(file);
    ui->md5_hash_value->setText("");
    ui->sha256_hash_value->setText("");
    ui->sha512_hash_value->setText("");
    ui->hash_btn->setDisabled(isFilePathEmpty());
}

void MainWindow::textEditInit(QTextEdit *edit){
    edit->setPlainText("");
    edit->setReadOnly(true);
}


void MainWindow::pushHashBtn(){
    QStatusBar *status = ui->statusBar;
    status->clearMessage();
    status->showMessage("starting to hash , MD5 , SHA256 , SHA512");

    QString path = ui->file_path_lineedit->text();
    ui->md5_hash_value->setText("");
    ui->sha256_hash_value->setText("");
    ui->sha512_hash_value->setText("");

    t_md5->path = path;
    t_sha256->path = path;
    t_sha512->path = path;

    /*
     * run multi-thread from here ,
     * send signal by signal-func passHashValue(workThread *) in workThread.cpp
     * signal receive slot-func is in here , render(workTHread) ,
     * then , translate string from unsigned char * to QString ,
     * finally , render the contents into front-end UI
    */

    t_md5->start();
    t_sha256->start();
    t_sha512->start();
}

void MainWindow::pushClearBtn(){
    /*clear-up all contents in edits*/
    ui->file_path_lineedit->setText("");
    ui->md5_hash_value->setText("");
    ui->sha256_hash_value->setText("");
    ui->sha512_hash_value->setText("");
    ui->verify_textedit->setText("");
    ui->result_label->setText("");

    /*disabled the hash button*/
    ui->hash_btn->setDisabled(true);
    ui->hash_btn->setStyleSheet("background-color: #ddd");

    /*clear and reset the label of verify result*/
    ui->result_label->setText("");
    ui->result_label->setStyleSheet("background-color: #eee");

    /*show status on statusBar*/
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("clear up all old contents , done.");
}

void MainWindow::initial(){
    this->setMaximumSize(650 , 500);
    ui->verify_textedit->setMaximumHeight(62);
    ui->md5_hash_value->setMaximumHeight(62);
    ui->sha256_hash_value->setMaximumHeight(62);
    ui->sha512_hash_value->setMaximumHeight(62);
    ui->hash_btn->setStyleSheet("background-color: #ddd; color:#fff");
    ui->clear_btn->setStyleSheet("background-color: #dd0000; color:#fff");

    t_md5 = new workThread(this);
    //t_md5->path = ui->file_path_lineedit->text();
    t_md5->hashMethod = "md5";
    t_md5->digest_size = MD5_DIGEST_LENGTH;
    connect(t_md5 , SIGNAL(passHashValue(workThread *)) , this , SLOT(render(workThread *)));
    md5Locker = false;

    t_sha256 = new workThread(this);
    //t_sha256->path = ui->file_choose_btn->text();
    t_sha256->hashMethod = "sha256";
    t_sha256->digest_size = SHA256_DIGEST_LENGTH;
    connect(t_sha256 , SIGNAL(passHashValue(workThread *)) , this , SLOT(render(workThread *)));
    sha256Locker = false;

    t_sha512 = new workThread(this);
    //t_sha512->path = ui->file_choose_btn->text();
    t_sha512->hashMethod = "sha512";
    t_sha512->digest_size = SHA512_DIGEST_LENGTH;
    connect(t_sha512 , SIGNAL(passHashValue(workThread *)) , this , SLOT(render(workThread *)));
    sha512Locker = false;

    QPushButton *chooseFileBtn , *hashBtn , *exitBtn , *clearBtn;
    chooseFileBtn = ui->file_choose_btn;
    hashBtn = ui->hash_btn;
    hashBtn->setDisabled(true);
    exitBtn = ui->exit_btn;
    clearBtn = ui->clear_btn;

    connect(chooseFileBtn , SIGNAL(clicked()) , this , SLOT(getFile()));
    connect(hashBtn , SIGNAL(clicked()) , this , SLOT(pushHashBtn()));
    connect(exitBtn , SIGNAL(clicked()) , this , SLOT(close()));
    connect(clearBtn  , SIGNAL(clicked()) , this , SLOT(pushClearBtn()));

    QLineEdit *filePath = ui->file_path_lineedit;
    filePath->setText("");
    filePath->setReadOnly(true);
    filePath->setPlaceholderText("\"檔名請更改為『英文』『數字』『符號』的組合\"");

    QTextEdit *md5Hash , *sha256Hash , *sha512Hash;
    md5Hash = ui->md5_hash_value;
    sha256Hash = ui->sha256_hash_value;
    sha512Hash = ui->sha512_hash_value;
    textEditInit(md5Hash);
    textEditInit(sha256Hash);
    textEditInit(sha512Hash);

    QTextEdit *verify = ui->verify_textedit;
    verify->setPlaceholderText("\"輸入已知的檔案雜湊值, 可與計算出的檔案雜湊值比對驗證\"");

    QStatusBar *status = ui->statusBar;
    status->clearMessage();
    status->showMessage("ready.");
}

void MainWindow::on_action_author_btn_triggered(){
    QMessageBox *msgBox = new QMessageBox();
    QString title = "作者 ＆ 使用說明";
    QString msg = "使用者您好,\n\n使用方法：\n首先選擇你要驗證的檔案, 檔名請保持『英文』『數字』『符號』的組合,如果是『中文或其他語系』的檔名可能會造成程式無預期的關閉。\n\n接著可以輸入已知的檔案雜湊值, 當按下 Hash 按鈕時, 等到檔案雜湊值計算出來之後, 就會自動比對驗證雜湊值, 完成之後可以直接選取別的檔案, 或者按清除按鈕之後在重新操作。\n\n\n這個 checksum machine 應用程式是我的學習 Linux programming 的一個小作品,\n是從學習「C 語言」,「多執行序」,「Linux 作業系統」的過程中所延伸出來的東西, 在寫完 C 語言版本的 checksum machine 之後, 想著如何把它做成日常看到的小型圖形的桌面程式，接著就學習 Qt 及 C++ 基礎語法 , 並把 C 語言版本的 porting 到 Qt 上, 也就是現在這個小程式。\n\nAuthor:Adyjs Chang\nemail:adyjsdev@gmail.com";
    msgBox->information(this , title , msg);
    //QMessageBox::information(this , "作者 ＆ 使用說明" , "test");
}
