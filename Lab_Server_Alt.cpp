#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPen>
#include <QPainter>
#include <iostream>
#include <winsock.h>
#include <QPushButton>
#include <fstream>


using namespace std;
class Window : public QWidget {
  public:
    Window() {
        this->setFixedSize(500,300);
        // Создаем клиента
        WSADATA ws;
        if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
        {
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        addr.sin_family = AF_INET;
        addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(1234);
        if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
        {
            // Ошибка...
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        size = sizeof(addr);
        // Кнопка подключения к серверу
        button_search->setText("Connect");
        button_search->setGeometry(20,60,80,30);
        connect(button_search, &QPushButton::clicked, this, &Window::button_search_press);
        // Кнопка отправки файла серверу
        button_send->setText("Send");
        button_send->setGeometry(20,200,80,30);
        connect(button_send, &QPushButton::clicked, this, &Window::button_send_press);
        // Кнопка принятия файла от сервера
        button_recv->setText("Recv");
        button_recv->setGeometry(20,240,80,30);
        connect(button_recv, &QPushButton::clicked, this, &Window::button_recv_press);
        // Результат подключения
        label->setText("");
        label->setFixedHeight(25);
        label->setFixedWidth(300);
        label->setGeometry(65,22,25,300);
        label->setFont(QFont("Calibri", 20));
    }

public slots:
    void button_search_press(){
        if(is_connected != 1) connect_to_server();
    };
    void button_send_press(){
        send_to_server();
    };
    void button_recv_press(){
        recv_from_server();
    };
  protected:
    void paintEvent(QPaintEvent *event){
        Q_UNUSED(event);
        QPainter painter(this);
        QPen pen(Qt::black, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(20, 20, 30, 30);
        if(is_connected == 1){
            QPen pen(QColor(77,167,30), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(15, 35, 35, 45);
            painter.drawLine(35, 45, 60, 20);
            update();
        }
        else if(is_connected == -1){
            QPen pen(QColor(236,71,25), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(20, 20, 50, 50);
            painter.drawLine(20, 50, 50, 20);
            update();
        }
    };
    // Функция отправки файла серверу
    void send_to_server(){
        FILE *File;
        char *Buffer;
        unsigned long Size;
        File = fopen("C:\\Users\\vogel\\Videos\\Red Dead Redemption 2\\Red Dead Redemption 2 2021.08.22 - 23.35.10.01.mp4", "rb");
        fseek(File, 0, SEEK_END);
        Size = ftell(File);
        fseek(File, 0, SEEK_SET);
        Buffer = new char[Size];
        fread(Buffer, Size, 1, File);
        fclose(File);
        if (SOCKET_ERROR == send(s, (const char*)&Size, sizeof(Size), 0))
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
        int amount;
        while (Size > 0) {
           if (SOCKET_ERROR == ( amount = send (s, Buffer, Size, 0 ) ))
           {
               std::cout << WSAGetLastError() << std::endl;
               return;
           }
           else {
               Size -= amount;
           }
       }
       delete[] Buffer;
    }
    // Функция принятия файла от сервера
    void recv_from_server(){
        unsigned long Size;
        if (SOCKET_ERROR == recv(s,(char*)&Size, sizeof(Size), 0))
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
        FILE *File;
        File = fopen("Red Dead Redemption 2 2021.08.22 - 23.35.10.01.mp4", "wb");
        char *Buffer = new char[Size];
        int amount;
        while (Size > 0) {
            if (SOCKET_ERROR == ( amount = recv (s, Buffer, Size, 0 ) ))
            {
                std::cout << WSAGetLastError() << std::endl;
                return;
            }
            else {
                fwrite((const char*)Buffer, 1, Size, File);
                Size -= amount;
            }
        }
        fclose(File);
        delete[] Buffer;
    }
    // Функция отправки запроса на поключение
    void connect_to_server(){
        fd_set s_set;
        FD_ZERO(&s_set);
        FD_SET(s, &s_set);
        timeval timeout = {0, 0};
        int select_res = select(s, 0, &s_set, 0, &timeout);
        if(!select_res){
            is_connected = -1;
            label->setText("Сервер не найден");
        }
            if (SOCKET_ERROR == ::connect(s, (sockaddr*)&addr,sizeof(addr)))
            {
                is_connected = -1;
                label->setText("Ошибка "+QString::number(WSAGetLastError()));
                std::cout << WSAGetLastError() << std::endl;
            }
            else {
                is_connected = 1;
                label->setText("Запрос серверу отправлен");
            }
    }

    int8_t is_connected = 0;
    int size;
    QLabel* label = new QLabel(this);
    QPainter painter;
    QPushButton *button_search = new QPushButton(this);
    QPushButton *button_send = new QPushButton(this);
    QPushButton *button_recv = new QPushButton(this);
    SOCKADDR_IN addr;
    SOCKET s;
};


int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        Window view;
        view.show();
        return app.exec();
}
