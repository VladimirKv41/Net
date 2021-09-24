#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <iostream>
#include <winsock.h>
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>


using namespace std;
class Window : public QWidget {
  public:
    Window() {
        this->setFixedSize(500,300);
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
        button_search->setText("Connect");
        button_search->setGeometry(20,60,80,30);
        connect(button_search, &QPushButton::clicked, this, &Window::button_search_press);
        button_send->setText("Send");
        button_send->setGeometry(20,200,80,30);
        connect(button_send, &QPushButton::clicked, this, &Window::button_send_press);
        label->setText("");
        label->setFixedHeight(25);
        label->setFixedWidth(300);
        label->setGeometry(65,22,25,300);
        label_sum_char->setText("+");
        label_sum_char->setFont(QFont("Calibri", 20));
        label_sum_char->setGeometry(63,143,20,20);
        label_result_char->setText("=");
        label_result_char->setFont(QFont("Calibri", 20));
        label_result_char->setGeometry(163,143,20,20);
        label->setFont(QFont("Calibri", 20));
        //line_number_1->setEnabled(false);
        line_number_1->setGeometry(20,120,100,20);
        //line_number_2->setEnabled(false);
        line_number_2->setGeometry(20,170,100,20);
        line_number_result->setEnabled(false);
        line_number_result->setReadOnly(true);
        line_number_result->setGeometry(223,145,100,20);
    }
    void send_to_server(){
        int32_t number = line_number_1->text().toInt();
        if (SOCKET_ERROR == ( send (s, (char*)&number, sizeof(number), 0 ) ))
        {
            std::cout << WSAGetLastError() << std::endl;
            return;
        }
       number = line_number_2->text().toInt();
       if (SOCKET_ERROR == ( send (s, (char*)&number, sizeof(number), 0 ) ))
       {
           std::cout << WSAGetLastError() << std::endl;
           return;
       }
       int a = 0;
       if (SOCKET_ERROR == (a = recv (s, (char* ) & number, sizeof(number), 0 )) )
       {
           std::cout << WSAGetLastError() << std::endl;
           return;
       }
       line_number_result->setText(QString::number(number));
    }
public slots:
    void button_search_press(){
        if(is_connected != 1) connect_to_server();
    };
    void button_send_press(){
        send_to_server();
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
    QLabel* label_sum_char = new QLabel(this);
    QLabel* label_result_char = new QLabel(this);
    QPainter painter;
    QPushButton *button_search = new QPushButton(this);
    QPushButton *button_send = new QPushButton(this);
    QLineEdit* line_number_1 = new QLineEdit(this);
    QLineEdit* line_number_2 = new QLineEdit(this);
    QLineEdit* line_number_result = new QLineEdit(this);
    SOCKADDR_IN addr;
    SOCKET s,s2;
};


int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        Window view;
        view.show();
        return app.exec();
}
