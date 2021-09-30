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
        // Создаем сервер
        WSADATA ws;
        if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
        {
            // Ошибка...
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        addr.sin_family = AF_INET;
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
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        size = sizeof(addr);
        // Кнопка поиска входящего запроса на подключение от клиента
        button->setText("Search");
        button->setGeometry(20,60,80,30);
        connect(button, &QPushButton::clicked, this, &Window::buttonpress);
        // Кнопка отправки файла клиенту
        button_recv->setText("Recv");
        button_recv->setGeometry(20,200,80,30);
        connect(button_recv, &QPushButton::clicked, this, &Window::button_recv_press);
        // Кнопка принятия файла от клиента
        button_send->setText("Send");
        button_send->setGeometry(20,240,80,30);
        connect(button_send, &QPushButton::clicked, this, &Window::button_send_press);
        // Результат подключения
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

public slots:
    void buttonpress(){
        if(flag != 1) accept_connection();
    };
    void button_recv_press(){
        recv_from_client();
    };
    void button_send_press(){
        send_to_client();
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
    // Функция принятия файла от клиента
    void recv_from_client(){
        unsigned long Size;
        if (SOCKET_ERROR == recv(s2,(char*)&Size, sizeof(Size), 0))
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
        FILE *File;
        File = fopen("124.mp4", "wb");
        char *Buffer = new char[Size];
        int amount;
        while (Size > 0) {
            if (SOCKET_ERROR == ( amount = recv (s2, Buffer, Size, 0 ) ))
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
    // Функция отправки файла клиенту
    void send_to_client(){
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
        if (SOCKET_ERROR == send(s2, (const char*)&Size, sizeof(Size), 0))
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
        int amount;
        while (Size > 0) {
           if (SOCKET_ERROR == ( amount = send (s2, Buffer, Size, 0 ) ))
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
    // Функция принятия подключения от клиента
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
    QPushButton *button = new QPushButton(this);;
    QPushButton *button_recv = new QPushButton(this);
    QPushButton *button_send = new QPushButton(this);
    SOCKADDR_IN addr;
    SOCKET s,s2;
};


int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        Window view;
        view.show();
        return app.exec();
}
