#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class server; }
QT_END_NAMESPACE

class server : public QWidget{
    Q_OBJECT

public:
    server(QWidget *parent = nullptr);
    ~server();

private slots:
    void on_btn_listen_clicked();//کدهای مربوط به دکمه پذیرش
    void on_btn_send_clicked();//کدهای مربوط به دکمه ارسال
    void server_New_Connect();// کدهای مربوط به ارتباط جدید در سرور
    void socket_Recv_Data();//کدهای مربوط به سوکت دریافت اطلاعات
    void socket_Disconnect();//کدهای مربوط به سوکت قطع ارتباط

private:
    Ui::server *ui;
    QTcpServer *m_server;
    QTcpSocket *m_socket;
};
#endif // SERVER_H
