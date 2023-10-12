#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");
    m_server=new QTcpServer(this);
    connect(ui->openBtn,&QPushButton::clicked,this,&Widget::openBtnFunc);
    connect(ui->sendBtn,&QPushButton::clicked,this,&Widget::sendBtnFunc);
    connect(this->m_server,&QTcpServer::newConnection,[=]()
    {
        m_socket=m_server->nextPendingConnection();

        qDebug()<<"connect ok";
        connect(m_socket,&QTcpSocket::readyRead,[=]()
        {
            QByteArray msgArray;
            msgArray=m_socket->readAll();
            ui->msgReceiveTextEdit->append("客户端say："+msgArray);

            connect(m_socket,&QTcpSocket::disconnected,[=]()
            {
               m_socket->close();
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
    if(str.isEmpty())
    {
        QMessageBox::information(this,"warning","no port edit");
        return;
    }
    m_server->listen(QHostAddress::Any,str.toUShort());
    ui->openBtn->setDisabled(true);
}

void Widget::sendBtnFunc()
{
    QString str=ui->msgSendtextEdit->toPlainText();
    m_socket->write(str.toUtf8());
    ui->msgReceiveTextEdit->append("服务端say："+str);
}

