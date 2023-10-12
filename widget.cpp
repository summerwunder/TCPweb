#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_server=new QTcpServer(this);
    connect(ui->openBtn,&QPushButton::clicked,this,&Widget::openBtnFunc);

    connect(this->m_server,&QTcpServer::newConnection,[=]()
    {
        m_socket=m_server->nextPendingConnection();

        qDebug()<<"connect ok";
        connect(m_socket,&QTcpSocket::readyRead,[=]()
        {
            QByteArray msgArray;
            msgArray=m_socket->readAll();
            ui->msgReceiveTextEdit->append("客户端say"+msgArray);

            connect(m_socket,&QTcpSocket::disconnected,[=]()
            {
               m_socket->disconnect();
               m_socket->deleteLater();
               qDebug()<<"disconnet ok";
            });
        });
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openBtnFunc()
{
    QString str=ui->portLineEdit->text();
    m_server->listen(QHostAddress::Any,str.toUShort());
    ui->openBtn->setDisabled(true);
}

