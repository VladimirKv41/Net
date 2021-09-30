#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <iostream>
#include <winsock.h>
#include <QMouseEvent>
#include <QPushButton>


using namespace std;
class Window : public QWidget {
  public:
    Window() {
        this->setFixedSize(500,300);
        WSADATA ws;
        if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
        {
            // Ошибка...
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        // Тип адреса (TCP/IP)
        addr.sin_family = AF_INET;
        // Адрес сервера. Т.к. TCP/IP представляет адреса в числовом
        // виде, то для перевода адреса используем функцию inet_addr.
        addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(1234);
        if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
        {
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        if (SOCKET_ERROR == bind(s, (SOCKADDR*)&addr, sizeof(addr)))
        {
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        if (SOCKET_ERROR == listen(s,SOMAXCONN))
        {
            // Ошибка...
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        size = sizeof(addr);
        button = new QPushButton(this);
        button->setText("Search");
        button->setGeometry(20,60,80,30);
        connect(button, &QPushButton::clicked, this, &Window::buttonpress);
        button_recv->setText("Recv");
        button_recv->setGeometry(20,200,80,30);
        connect(button_recv, &QPushButton::clicked, this, &Window::button_recv_press);
        label->setText("");
        label->setFixedHeight(25);
        label->setFixedWidth(300);
        label->setGeometry(65,22,25,300);
        label->setFont(QFont("Calibri", 20));
        label_result->setText("");
        label_result->setFixedHeight(200);
        label_result->setFixedWidth(400);
        label_result->setGeometry(20,42,400,200);
        label_result->setFont(QFont("Calibri", 20));
    }
    void recv_from_client(){
        int32_t number,number2;
        int a = 0;
        if (SOCKET_ERROR == (a = recv (s2, (char* ) & number, sizeof(number), 0 )) )
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
        if (SOCKET_ERROR == (a = recv (s2, (char* ) & number2, sizeof(number), 0 )) )
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
        label_result->setText("Приняты числа " + QString::number(number) + " и " + QString::number(number2) + ".\n Результат: \n" + QString::number(number + number2));
        int32_t result = number + number2;
        if (SOCKET_ERROR == ( send (s2, (char*)&(result), sizeof(result), 0 ) ))
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
    }
public slots:
    void buttonpress(){
        if(flag != 1) accept_connection();
    };
    void button_recv_press(){
        recv_from_client();
    };
  protected:
    void paintEvent(QPaintEvent *event){
        Q_UNUSED(event);
        QPainter painter(this);
        QPen pen(Qt::black, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(20, 20, 30, 30);
        if(flag == 1){
            QPen pen(QColor(77,167,30), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(15, 35, 35, 45);
            painter.drawLine(35, 45, 60, 20);
            update();
        }
        else if(flag == -1){
            QPen pen(QColor(236,71,25), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(20, 20, 50, 50);
            painter.drawLine(20, 50, 50, 20);
            update();
        }
    };
    void accept_connection(){
        fd_set s_set;
        FD_ZERO(&s_set);
        FD_SET(s, &s_set);
        timeval timeout = {5, 0};
        int select_res = select(s, &s_set, 0, 0, &timeout);
        if(select_res){
            if (SOCKET_ERROR == (s2 = accept(s, (SOCKADDR*)&addr, &size)))
            {
                 flag = -1;
                label->setText("Ошибка "+QString::number(WSAGetLastError()));
                std::cout << WSAGetLastError() << std::endl;
            }
            else {

                flag = 1;
                label->setText("Клиент подключен");
            }
        }
        else {
            flag = -1;
            label->setText("Клиент не найден");
        }
    }
    int8_t flag = 0;
    int size;
    QLabel* label = new QLabel(this);
    QLabel* label_result = new QLabel(this);
    QPainter painter;
    QPushButton *button;
    QPushButton *button_recv = new QPushButton(this);
    SOCKADDR_IN addr;
    SOCKET s,s2;
};


int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        Window view;
        view.show();
        return app.exec();
}
