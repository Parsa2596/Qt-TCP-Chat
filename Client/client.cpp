#include "client.h"
#include "ui_client.h"
#include <QtWidgets/QMessageBox>
#include <QTime>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client){
    ui->setupUi(this);
    m_socket = new QTcpSocket();

    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&Client::socket_Recv_Data);
    QObject::connect(m_socket,&QTcpSocket::disconnected,this,&Client::Socket_Disconnect);

    ui->btn_Send->setShortcut(QKeySequence(tr("ctrl+return")));
    ui->btn_Send->setEnabled(false);
}

Client::~Client(){
    delete m_socket;
    delete ui;
}


void Client::on_btn_Connect_clicked(){
    QString IP;
    qint16 port;

    if(ui->btn_Connect->text() == QString("Connect"))
    {
        IP = ui->lineEdit_IP->text();
        port = ui->lineEdit_Port->text().toInt();

        m_socket->abort();
        m_socket->connectToHost(IP,port);

            if(!m_socket->waitForConnected())
            {
                QMessageBox msgb;
                msgb.setText("Connection timed out!");
                msgb.resize(40,30);
                msgb.exec();
                return;
            }

         QMessageBox msgb;
         msgb.setText("Connection succeed");
         msgb.resize(40,30);
         msgb.exec();

         ui->btn_Send->setEnabled(true);
         ui->btn_Connect->setText("Disconnect");
      }
    else
    {
        m_socket->disconnectFromHost();

        ui->btn_Connect->setText("Connect");
        ui->btn_Send->setEnabled(false);
    }
}


void Client::on_btn_Send_clicked(){
    if(ui->textEdit->toPlainText() == QString())
    {
        QMessageBox msgb;
        msgb.setText("can't send empty message!");
        msgb.resize(60,40);
        msgb.exec();
        return;
    }
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append("From Client: "+QTime::currentTime().toString());

    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
    ui->textBrowser->append(ui->textEdit->toPlainText().toUtf8());

    m_socket->write(ui->textEdit->toPlainText().toUtf8());
    m_socket->flush();
    ui->textEdit->clear();
}

void Client::socket_Recv_Data(){
    QByteArray data_tmp;
    data_tmp = m_socket->readAll();
    if(!data_tmp.isEmpty())
    {
        QString str = QString(data_tmp);

        ui->textBrowser->setTextColor(Qt::gray);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->textBrowser->append("From Client :"+QTime::currentTime().toString());

        ui->textBrowser->setTextColor(Qt::black);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
        ui->textBrowser->append(str);
    }
}

void Client::Socket_Disconnect(){
    ui->btn_Send->setEnabled(false);
    ui->btn_Connect->setText("connect");
    QMessageBox msgb;
    msgb.setText("Disconnect!");
    msgb.resize(40,30);
    msgb.exec();
}
