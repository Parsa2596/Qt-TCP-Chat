#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void on_btn_Connect_clicked();//کدهای مربوط به دکمه پیوستن
    void on_btn_Send_clicked();//کدهای مربوط به دکمه ارسال
    void socket_Recv_Data();//کدهای مربوط به دریافت اطلاعات
    void Socket_Disconnect();//کدهای مربوط به قطع کردن ارتباط

private:
    Ui::Client *ui;
    QTcpSocket *m_socket;
};
#endif // CLIENT_H
