#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <bitset>
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <QMouseEvent>
#include <QEvent>
#include <QPushButton>
#include "iphlpapi.h"
#include "icmpapi.h"
#include "QLineEdit"
#include "QTextEdit"
#include "winsock2.h"
#include <QRegularExpression>
#include <qlineedit.h>
#include <qwidget.h>
#include <QWidgetItem>
#include <QScrollArea>
#include <QFileDialog>


using namespace std;

class Window_HTTP : public QWidget {
  public:
    Window_HTTP() {
        this->setFixedSize(900,600);
        this->setWindowTitle("HTTP");

        button_connect->setText("Connect");
        button_connect->setGeometry(200,20,80,30);
        connect(button_connect, &QPushButton::clicked, this, &Window_HTTP::button_connect_press);

        page_address->setGeometry(20,25,160,20);
        page_address->setReadOnly(false);
        page_address->setEnabled(true);
        page_address->setPlaceholderText("Page");
        page_address->setFont(QFont("Calibri", 13));

        label_status->setFont(QFont("Calibri", 13));

        label_connect->setGeometry(20,110,200,20);
        label_connect->setFont(QFont("Calibri", 20));

        label_html_text->setText("HTML Text: ");
        label_html_text->setGeometry(300,20,100,20);
        label_html_text->setFont(QFont("Calibri", 13));
        label_html_text->setAlignment(Qt::AlignLeft);

        html_text->setFont(QFont("Calibri", 13));

        area_html_text->setWidget(html_text);
        area_html_text->setGeometry(300,50,580,530);

        area_status->setWidget(label_status);
        area_status->setGeometry(20,140,260,440);
    }

public slots:
    void button_connect_press(){
        connect_to_server();
    };
  protected:
    int rs_init (int v_major, int v_minor)
    {
        WSADATA wsadata;
        // Инициализация WinSock заданной версии
        if (WSAStartup(MAKEWORD(v_major, v_minor), &wsadata))
        {
            return WSAGetLastError();
        }
        // Проверка версии WinSock
        if(LOBYTE(wsadata.wVersion) != v_minor ||
            HIBYTE(wsadata.wVersion) != v_major)
        {
            rs_exit();
            return WSAGetLastError();
        }
        return 0;
    }
    int rs_exit(void)
    {
        // Закрытие библиотеки Winsock
        WSACleanup ();
        return 0;
    }
    void paintEvent(QPaintEvent *event){
        Q_UNUSED(event);
        QPainter painter(this);
        QPen pen(Qt::black, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(20, 70, 30, 30);
        if(flag_connection == 1){
            QPen pen(QColor(77,167,30), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(15, 85, 35, 95);
            painter.drawLine(35, 95, 60, 70);
            update();
        }
        else if(flag_connection == 0){
            QPen pen(QColor(236,71,25), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(20, 70, 50, 100);
            painter.drawLine(20, 100, 50, 70);
            update();
        }
    };
    // Запрос HTML-страницы
    void connect_to_server(){
        QString server_page = page_address->text().mid(0,page_address->text().indexOf("/"));
        std::string request = "GET ";
        request += page_address->text().mid(page_address->text().indexOf("/")).toStdString();
        request += " HTTP/1.0\r\nHost: ";
        request += server_page.toStdString();
        request += "\r\n\r\n";
        html_text->setText("");
        label_status->setText(label_status->text() + "\n---------\n" + "Инициализация WinSock..");
        if (rs_init(2,2))
        {
            error();
            label_status->adjustSize();
            return;
        }
        // 2. Создание сокета
        label_status->setText(label_status->text() + "\n" + "Cоздание сокета..");
        if(INVALID_SOCKET == (s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
        {
            error();
            label_status->adjustSize();
            return;
        }
        // 3. Определение имени и IP-адреса компьютера
        // Получаем адрес
        label_status->setText(label_status->text() + "\n" + "Получение IP-адреса..");
        if (NULL == (host_inf = gethostbyname(server_page.toLocal8Bit().data())))
        {
            error();
            label_status->adjustSize();
            return;
        }
        // Заполняем  поля структуры adr для использование ее в
        // connect
        addr.sin_family	= AF_INET;
        addr.sin_addr.S_un.S_addr = *(DWORD* ) host_inf->h_addr_list[0];
        addr.sin_port = htons (80);
        // 4. Привязка сокета к IP-адресу компьютера
        label_status->setText(label_status->text() + "\n" + "Соединение с сервером..");
        if (SOCKET_ERROR == ::connect(s, (sockaddr*) &addr,  sizeof(addr) ) )
        {
            error();
            label_status->adjustSize();
            return;
        }
        // Посылаем запрос серверу
        label_status->setText(label_status->text() + "\n" + "Запрос серверу..");
        if (SOCKET_ERROR == send(s, request.c_str(), strlen(request.c_str()), 0))
        {
            error();
            label_status->adjustSize();
            return;
        }
        label_status->setText(label_status->text() + "\n" + "Установка неблокирующего сокета..");
        BOOL l = TRUE;
        if (SOCKET_ERROR == ioctlsocket (s, FIONBIO, (unsigned long* ) &l) )
        {
            error();
            label_status->adjustSize();
            return;

        }
        // Ждём ответа
        int len, res;
        fd_set read_s;
        timeval time_out;
        time_out.tv_sec = 3; // 0.5 sec.
        do
        {
            FD_ZERO (&read_s);	 	// Обнуляем множество
            FD_SET (s, &read_s); 	// Заносим в него наш сокет
            label_status->setText(label_status->text() + "\n" + "Проверка готовности сокета..");
            if (SOCKET_ERROR == (res = select (0, &read_s, NULL, NULL, &time_out) ) )
            {
                error();
                label_status->adjustSize();
                return;
            }
            label_status->setText(label_status->text() + "\n" + "Ожидание данных..");
            if ((res!=0) && (FD_ISSET (s, &read_s)) )
            {
                // Данные готовы к чтению...
                if (SOCKET_ERROR == (len = recv (s, (char *) &buff, 1024, 0) ) )
                {
                    error();
                    label_status->adjustSize();
                    return;
                }
                // Выводим их на экран.
                label_status->setText(label_status->text() + "\n" + "Данные получены.. ");
                html_text->setText(html_text->text() + buff);
            }
        }
        while (len!=0);
        // Закрываем соединенеие
        label_status->setText(label_status->text() + "\n" + "Закрытие сокета..");
        if (SOCKET_ERROR == closesocket (s) )
        {
            error();
        }
        label_status->adjustSize();
        html_text->adjustSize();
        flag_connection = 1;
        label_connect->setText("Успешно");
        rs_exit();
    }
    void error(){
        label_connect->setText("Ошибка " + QString::number(WSAGetLastError()));
        flag_connection = 0;
        update();
    }
    HOSTENT* host_inf;	// Информация о хосте.
    SOCKADDR_IN addr;	// Адрес компьютера
    int8_t flag_connection = -1;
    QLineEdit* page_address = new QLineEdit(this);
    QLabel* label_status = new QLabel(this);
    QLabel* label_connect = new QLabel(this);
    QLabel* label_html_text = new QLabel(this);
    QLabel* html_text = new QLabel(this);
    char buff [1024];
    QPainter painter;
    QPushButton *button_connect = new QPushButton(this);
    QScrollArea *area_html_text = new QScrollArea(this);
    QScrollArea *area_status = new QScrollArea(this);
    SOCKET s;
};

class Window_FTP : public QWidget {
  public:
    Window_FTP() {
        this->setFixedSize(620,600);
        this->setWindowTitle("FTP");

        button_connect->setText("Connect");
        button_connect->setGeometry(520,20,80,30);
        connect(button_connect, &QPushButton::clicked, this, &Window_FTP::button_connect_press);

        page_address->setGeometry(20,25,480,20);
        page_address->setReadOnly(false);
        page_address->setEnabled(true);
        page_address->setPlaceholderText("Page");
        page_address->setFont(QFont("Calibri", 13));

        label_status->setFont(QFont("Calibri", 13));

        label_connect->setGeometry(20,110,200,20);
        label_connect->setFont(QFont("Calibri", 20));

        area_html_text->setWidget(label_status);
        area_html_text->setGeometry(20,150,580,430);
    }

public slots:
    void button_connect_press(){
        connect_to_server();
    };
  protected:
    int rs_init (int v_major, int v_minor)
    {
        WSADATA wsadata;
        // Инициализация WinSock заданной версии
        if (WSAStartup(MAKEWORD(v_major, v_minor), &wsadata))
        {
            return WSAGetLastError();
        }
        // Проверка версии WinSock
        if(LOBYTE(wsadata.wVersion) != v_minor ||
            HIBYTE(wsadata.wVersion) != v_major)
        {
            rs_exit();
            return WSAGetLastError();
        }
        return 0;
    }
    int rs_exit(void)
    {
        // Закрытие библиотеки Winsock
        WSACleanup ();
        return 0;
    }
    void paintEvent(QPaintEvent *event){
        Q_UNUSED(event);
        QPainter painter(this);
        QPen pen(Qt::black, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(20, 70, 30, 30);
        if(flag_connection == 1){
            QPen pen(QColor(77,167,30), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(15, 85, 35, 95);
            painter.drawLine(35, 95, 60, 70);
            update();
        }
        else if(flag_connection == 0){
            QPen pen(QColor(236,71,25), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(20, 70, 50, 100);
            painter.drawLine(20, 100, 50, 70);
            update();
        }
    };
    // Скачать файл с FTP-сервера
    void connect_to_server(){
        QString server_page = page_address->text().mid(0,page_address->text().indexOf("/"));
        std::string request1 = "USER anonymous\r\n";
        std::string request2 = "PASS\r\n";
        std::string request3 = "PASV\r\n";
        std::string request4 = "RETR "+page_address->text().mid(page_address->text().indexOf("/")).toStdString()+"\r\n";
        label_status->setText("");
        label_status->setText(label_status->text() + "\n---------\n" + "Инициализация WinSock..");
        if (rs_init(2,2))
        {
            error();
            label_status->adjustSize();
            return;
        }
        // 2. Создание сокета
        label_status->setText(label_status->text() + "\n" + "Cоздание сокета..");
        if(INVALID_SOCKET == (s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
        {
            error();
            label_status->adjustSize();
            return;
        }
        // 3. Определение имени и IP-адреса компьютера
        // Получаем адрес
        label_status->setText(label_status->text() + "\n" + "Получение IP-адреса..");
        if (NULL == (host_inf = gethostbyname(server_page.toLocal8Bit().data())))
        {
            error();
            label_status->adjustSize();
            return;
        }
        // Заполняем  поля структуры adr для использование ее в
        // connect
        addr.sin_family	= AF_INET;
        addr.sin_addr.S_un.S_addr = *(DWORD* ) host_inf->h_addr_list[0];
        addr.sin_port = htons (21);
        // 4. Привязка сокета к IP-адресу компьютера
        label_status->setText(label_status->text() + "\n" + "Соединение с сервером..");
        if (SOCKET_ERROR == ::connect(s, (sockaddr*) &addr,  sizeof(addr) ) )
        {
            error();
            label_status->adjustSize();
            return;
        }
        recv (s, (char *) &buff, 1024, 0);
        label_status->setText(label_status->text() + "Сервер : " +  + "\n" + buff + "");
        memset(&buff[0], 0, sizeof(buff));
        // Посылаем запрос серверу
        if (SOCKET_ERROR == send(s, request1.c_str(), strlen(request1.c_str()), 0))
        {
            error();
            label_status->adjustSize();
            return;
        }
        recv (s, (char *) &buff, 1024, 0);
        label_status->setText(label_status->text() + "Сервер : " + "\n" +  buff);
        memset(&buff[0], 0, sizeof(buff));
        if (SOCKET_ERROR == send(s, request2.c_str(), strlen(request2.c_str()), 0))
        {
            error();
            label_status->adjustSize();
            return;
        }
        recv (s, (char *) &buff, 1024, 0);
        label_status->setText(label_status->text() + "Сервер : " + "\n" +  buff);
        memset(&buff[0], 0, sizeof(buff));
        if (SOCKET_ERROR == send(s, request3.c_str(), strlen(request3.c_str()), 0))
        {
            error();
            label_status->adjustSize();
            return;
        }
        recv (s, (char *) &buff, 1024, 0);
        label_status->setText(label_status->text() + "Сервер : " + "\n" +  buff);
        string = buff;
        memset(&buff[0], 0, sizeof(buff));
        if (rs_init(2,2))
        {
            error();
            label_status->adjustSize();
            return;
        }
        if(INVALID_SOCKET == (s2 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
        {
            error();
            label_status->adjustSize();
            return;
        }
        if (NULL == (host_inf = gethostbyname(server_page.toLocal8Bit().data())))
        {
            error();
            label_status->adjustSize();
            return;
        }
        addr.sin_family	= AF_INET;
        addr.sin_addr.S_un.S_addr = *(DWORD* ) host_inf->h_addr_list[0];
        addr.sin_port = htons ((string.mid(string.indexOf(QRegularExpression("[0-9]+,[0-9]+\\)")),
            string.indexOf(QRegularExpression(",[0-9]+\\)")) -
            string.indexOf(QRegularExpression("[0-9]+,[0-9]+\\)"))).toInt()*256) +
            string.mid(string.indexOf(QRegularExpression("[0-9]+\\)")),
            string.indexOf(QRegularExpression("\\)")) -
            string.indexOf(QRegularExpression("[0-9]+\\)"))).toInt()
        );
        if (SOCKET_ERROR == ::connect(s2, (sockaddr*) &addr,  sizeof(addr) ) )
        {
            error();
            label_status->adjustSize();
            return;
        }
        if (SOCKET_ERROR == send(s, request4.c_str(), strlen(request4.c_str()), 0))
        {
            error();
            label_status->adjustSize();
            return;
        }
        recv (s, (char *) &buff, 1024, 0);
        label_status->setText(label_status->text() + "Сервер : " + "\n" +  buff);
        memset(&buff[0], 0, sizeof(buff));
        recv (s, (char *) &buff, 1024, 0);
        label_status->setText(label_status->text() + "Сервер : " + "\n" +  buff);
        memset(&buff[0], 0, sizeof(buff));
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
            "",
            QFileDialog::ShowDirsOnly |
            QFileDialog::DontResolveSymlinks
        );
        ofstream file;
        label_status->setText(label_status->text() + "\n" + "Файл сохранен в " + dir + "\n" + QDateTime::currentDateTime().toString());
        file.open(dir.toStdString() + page_address->text().mid(page_address->text().indexOf(QRegularExpression("\\/[^\\/]+\\..+"))).toStdString());
        QString string22 = dir + page_address->text().mid(page_address->text().indexOf(QRegularExpression("\\/[^\\/]+\\..+")));
        char buffer[1];
        int bytes(0);
        while (recv(s2, buffer, sizeof(buffer), 0)) {
            file << buffer[0];
        }
        file.close();
        // Закрываем соединенеие
        label_status->setText(label_status->text() + "\n" + "Закрытие сокета..");
        if (SOCKET_ERROR == closesocket (s) )
        {
            error();
            label_status->adjustSize();
            return;
        }
        if (SOCKET_ERROR == closesocket (s2) )
        {
            error();
            label_status->adjustSize();
            return;
        }
        label_status->adjustSize();
        label_status->adjustSize();
        flag_connection = 1;
        label_connect->setText("Успешно");
        rs_exit();
    }

    void error(){
        label_connect->setText("Ошибка " + QString::number(WSAGetLastError()));
        flag_connection = 0;
        update();
    }
    HOSTENT* host_inf;	// Информация о хосте.
    SOCKADDR_IN addr;	// Адрес компьютера
    int8_t flag_connection = -1;
    QLineEdit* page_address = new QLineEdit(this);
    QLabel* label_status = new QLabel(this);
    QLabel* label_connect = new QLabel(this);
    char buff [1024];
    QPainter painter;
    QPushButton *button_connect = new QPushButton(this);
    QScrollArea *area_html_text = new QScrollArea(this);
    SOCKET s,s2;
    QString string;
};

int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        Window_FTP win_ftp;
        Window_HTTP win_http;
        win_ftp.show();
        win_http.show();
        return app.exec();
}
