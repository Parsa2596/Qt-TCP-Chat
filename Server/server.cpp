#include "server.h"
#include "ui_server.h"
#include <QtWidgets/QMessageBox>
#include <QTime>

server::server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::server){
    ui->setupUi(this);
    ui->btn_send->setEnabled(false);

    m_server = new QTcpServer();
    connect(m_server,&QTcpServer::newConnection,this,&::server::server_New_Connect);
}

server::~server(){
    m_server->close();
    m_server->deleteLater();
    delete ui;
}

void server::on_btn_listen_clicked(){
    if(ui->btn_listen->text() ==QString("Monitor"))
    {
        qint16 port = ui->lineEdit_Port->text().toInt();
        if(!m_server->listen(QHostAddress::Any,port))
        {
            QMessageBox::critical(this,"Error !", m_server->errorString(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        ui->btn_listen->setText("cancel listening");
    }
    else
    {
        if(m_socket->state() == QAbstractSocket::ConnectedState)
        {
            m_socket->disconnectFromHost();
        }
        m_server->close();
        ui->btn_listen->setText("Monitor");
        ui->btn_send->setEnabled(false);
    }
}


void server::on_btn_send_clicked(){
    if(ui->textEdit->toPlainText() == QString())
    {
        QMessageBox msgb;
        msgb.setText("Cant send empty message!");
        msgb.resize(60,40);
        msgb.exec();
        return;
    }
    m_socket->write(ui->textEdit->toPlainText().toUtf8());

    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append("From Server: "+QTime::currentTime().toString());

    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
    ui->textBrowser->append(ui->textEdit->toPlainText().toUtf8());

    m_socket->flush();
    ui->textEdit->clear();
}

void server::server_New_Connect(){
    m_socket = m_server->nextPendingConnection();

    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&server::socket_Recv_Data);
    QObject::connect(m_socket,&QTcpSocket::disconnected,this,&server::socket_Disconnect);

    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append(tr("Client Connect ! %1\n").arg(QTime::currentTime().toString()));
    ui->btn_send->setEnabled(true);
}

void server::socket_Recv_Data(){
    QByteArray data_tmp;
    data_tmp = m_socket->readAll();
    if(!data_tmp.isEmpty())
    {
        QString str = QString(data_tmp);

        ui->textBrowser->setTextColor(Qt::gray);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->textBrowser->append("From Client: "+QTime::currentTime().toString());

        ui->textBrowser->setTextColor(Qt::black);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",16));
        ui->textBrowser->append(str);

        QTextCursor cursor = ui->textBrowser->textCursor(); //Automatically pull down to the bottom
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(cursor);
    }
}

void server::socket_Disconnect(){
    ui->btn_send->setEnabled(false);
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append(tr("Client Left ! %1\n").arg(QTime::currentTime().toString()));
}
