#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPen>
#include <QPainter>
#include <iostream>
#include <winsock2.h>
#include <QPushButton>
#include <fstream>

// тип ICMP пакета
#define ICMP_ECHO_REPLY			0
#define ICMP_UNREACHABLE			3
#define ICMP_QUENCH				4
#define ICMP_REDIRECT				5
#define ICMP_ECHO				8
#define ICMP_TIME				11
#define ICMP_PARAMETER				12
#define ICMP_TIMESTAMP				13
#define ICMP_TIMESTAMP_REPLY			14
#define ICMP_INFORMATION			15
#define ICMP_INFORMATION_REPLY		16

// ICMP коды для ICMP типа ICMP_UNREACHABLE
#define ICMP_UNREACHABLE_NET			0
#define ICMP_UNREACHABLE_HOST			1
#define ICMP_UNREACHABLE_PROTOCOL		2
#define ICMP_UNREACHABLE_PORT			3
#define ICMP_UNREACHABLE_FRAGMENTATION	4
#define ICMP_UNREACHABLE_SOURCE		5
#define ICMP_UNREACHABLE_SIZE			8

// ICMP коды для ICMP типа ICMP_TIME
#define ICMP_TIME_TRANSIT			0
#define ICMP_TIME_FRAGMENT			1

// ICMP коды для ICMP типа ICMP_REDIRECT
#define ICMP_REDIRECT_NETWORK			0
#define ICMP_REDIRECT_HOST			1
#define ICMP_REDIRECT_SERVICE_NETWORK		2
#define ICMP_REDIRECT_SERVICE_HOST		3

// Версия IP пакета
#define RS_IP_VERSION		0x40

// IP флаги фрагментации
#define IP_FLAG_FO_MASK	0xE000
#define IP_FLAG_MORE_FRAG	0x2000
#define IP_FLAG_DONT_FRAG	0x4000

// IP тип сервиса
#define IP_TOS_0x00		0x00
#define IP_TOS_0x02		0x02
#define IP_TOS_0x04		0x04
#define IP_TOS_0x08		0x08
#define IP_TOS_0x10		0x10

struct ip_header
{
unsigned char	ver_ihl = 0x45;	// Длина заголовка (4 бита)
                // (измеряется в словах по 32 бита) +
                // + Номер версии протокола (4 бита)
unsigned char	tos = IP_TOS_0x00;		// Тип сервиса
unsigned short	tlen = 0x1C;		// Общая длина пакета
unsigned short	id = 0x01;		// Идентификатор пакета
unsigned short	flags_fo = 16384;	// Управляющие флаги (3 бита)
                    // + Смещение фрагмента (13 бит)
unsigned char	ttl=0x02;		// Время жизни пакета
unsigned char	proto=0x01;		// Протокол верхнего уровня
unsigned short	crc=0x00;		// CRC заголовка
unsigned int	src_addr = inet_addr("127.0.0.1");	// IP-адрес отправителя
unsigned int	dst_addr = inet_addr("192.168.1.101");	// IP-адрес получателя
};

struct icmp_header
{
unsigned char   type = ICMP_ECHO;			// тип ICMP- пакета
unsigned char   code=0x00;			// код ICMP- пакета
unsigned short  crc=0x00;			// контрольная сумма
union {
    struct { unsigned char	uc1, uc2, uc3, uc4; } s_uc;
    struct { unsigned short	us1, us2; } s_us;
    unsigned long s_ul;
    } s_icmp;				// зависит от типа
};

using namespace std;
class Window : public QWidget {
  public:
    Window() {
        this->setFixedSize(500,300);
        // Создаем сервер
        WSADATA ws;
        if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws)))
        {
            // Ошибка...
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        if(LOBYTE(ws.wVersion) != 2 ||
                HIBYTE(ws.wVersion) != 2)
        {
                std::cout << WSAGetLastError() << std::endl;
                exit(1);
        }

        if (INVALID_SOCKET == (s = WSASocket (AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0,WSA_FLAG_OVERLAPPED)))
        {

            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        int tos = 0;
        int tos_len = sizeof (tos);
        int per=setsockopt(s, IPPROTO_IP, 3, (char *)&tos, tos_len);
        if (per == SOCKET_ERROR)
        {

            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
        unsigned int use_own_header = 1;
        // Установка опции RAW для сокета, что говорит о том
        // что мы вручную будем формировать заголовки пакетов
        if ( setsockopt (s, IPPROTO_IP, 2, (char*)&use_own_header, sizeof(use_own_header))== SOCKET_ERROR)
        {
            std::cout << WSAGetLastError() << std::endl;
            exit(1);
        }
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
        ip_header ip;
        icmp_header icmp;
        memset (&addr, 0, sizeof (addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.S_un.S_addr = ip.dst_addr;
        addr.sin_port = htons(1234);
        rs_send_icmp(s,ip,icmp,8);
    }
    // Функция принятия файла от сервера
    void recv_from_server(){
        ip_header ip;
        icmp_header icmp;
        //rs_send_icmp(s,ip,icmp,8);
        int result;
        char * buffer =(char *) calloc (28, sizeof (char));
        memset(buffer,' ',512);
        sockaddr_in target;
                //memset (&target, 0, sizeof (target));
                //target.sin_family = AF_INET;
                //target.sin_addr.s_addr = ip.dst_addr;
                //target.sin_port = htons(1234);
        int senderaddrsize = sizeof(target);
        fd_set s_set;
        FD_ZERO(&s_set);
        FD_SET(s, &s_set);
        timeval timeout = {1, 0};
        int select_res = select(s, 0, &s_set, 0, &timeout);
        std::cout << select_res << std::endl;
        std::cout << 444 << WSAGetLastError() << std::endl;
        if(select_res && select_res != SOCKET_ERROR){

            if (SOCKET_ERROR == ( result = recvfrom ( s, (char*)buffer, 28, 0,(struct sockaddr *)&target,&senderaddrsize)))
            {
                std::cout << 321 << WSAGetLastError() << std::endl;
            }
            std::cout << result << std::endl;

         }
    }
    unsigned short rs_crc (unsigned short * buffer, int length)
    {
        unsigned long crc = 0;
        // Вычисление CRC
        while (length > 1)
        {
            crc += *buffer++;
            length -= sizeof (unsigned short);
        }
        if (length) crc += *(unsigned char*) buffer;
        // Закончить вычисления
        crc = (crc >> 16) + (crc & 0xffff);
        crc += (crc >> 16);
        // Возвращаем инвертированное значение
        return (unsigned short)(~crc);
    }
    int rs_send_ip (SOCKET s, struct ip_header iph,
                 unsigned char * data,
                 int data_length,
                 unsigned short dst_port_raw)
    {
        char * buffer;
        int result;
        sockaddr_in target;
        unsigned char header_length;
        unsigned int packet_length;
        memset (&target, 0, sizeof (target));
        target.sin_family = AF_INET;
        target.sin_addr.s_addr = iph.dst_addr;
        target.sin_port = dst_port_raw;

        // Вычисление длины и заголовка пакета
        header_length = sizeof (struct ip_header);
        packet_length = header_length + data_length;

        // Установка CRC.
        iph.crc = 0;

        // Заполнение некоторых полей заголовка IP
        iph.ver_ihl = RS_IP_VERSION;

        // Если длина пакета не задана, то длина пакета
        // приравнивается к длине заголовка
        if (!(iph.ver_ihl & 0x0F))
            iph.ver_ihl |= 0x0F & (header_length / 4);
        buffer =(char *) calloc (packet_length, sizeof (char));

        // Копирование заголовка пакета в буфер ( CRC равно 0).
        memcpy (buffer, &iph, sizeof (struct ip_header));

        // Копирование данных в буфер
        if (data) memcpy (buffer + header_length, data,
                    data_length);

        // Вычисление CRC.
        iph.crc = rs_crc((unsigned short *) buffer,
                    packet_length);

        // Копирование заголовка пакета в буфер (CRC посчитана).
        memcpy (buffer, &iph, sizeof (struct ip_header));

        // Отправка IP пакета в сеть
        if (SOCKET_ERROR == ( result = sendto ( s, buffer, packet_length, 0,(struct sockaddr *)&target,sizeof (target))))
        {
            std::cout << 321 << WSAGetLastError() << std::endl;
        }
        free (buffer);
        return result;
    }

    int rs_send_icmp (SOCKET s, struct ip_header iph,
                   struct icmp_header icmph,
                   int data_length)
    {
        unsigned char * buffer;
        int result;
        unsigned char header_length;
        unsigned int packet_length;
        data_length = 0;

        // Вычисление длин пакета и заголовка.
        header_length = sizeof (struct icmp_header);
        packet_length = header_length + data_length;
        icmph.crc = 0;
        buffer = (unsigned char *)calloc (packet_length, sizeof (char));

        // Копирование заголовка пакета в буфер ( CRC равно 0).
        memcpy (buffer, &icmph, sizeof (struct icmp_header));

        // Вычисление CRC.
        icmph.crc = rs_crc ((unsigned short *) buffer,
                      packet_length);

        // Копирование заголовка пакета в буфер (CRC посчитана).
        memcpy (buffer, &icmph, sizeof (struct icmp_header));

        // Отправка IP пакета с вложенным ICMP пакетом.
        result = rs_send_ip (s, iph, buffer, packet_length, htons(1234));
        free (buffer);
        std::cout << 123 << result << std::endl;
        return result;
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
