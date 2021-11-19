#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <bitset>
#include <iostream>
#include <winsock2.h>
#include <QMouseEvent>
#include <QEvent>
#include <QPushButton>
#include "iphlpapi.h"
#include "icmpapi.h"
#include "QLineEdit"
#include "QTextEdit"
#include "winsock2.h"
#include <QRegularExpressionValidator>
#include <qlineedit.h>
#include <qwidget.h>
#include <QBitArray>
#include <QChar>
#include <QWidgetItem>
#include <map>
std::map<int, std::string> map_protocol {
    {0, "HOPOPT(IPv6 Hop-by-Hop Option)"},{1, "ICMP(Internet Control Message)"},{2, "IGMP(Internet Group Management)"},{3, "GGP(Gateway-to-Gateway)"},{4, "IPv4(IPv4 encapsulation)"},
    {5, "ST(Stream)"},{6, "TCP(Transmission Control)"},{7, "CBT(Core Based Trees)"},{8, "EGP(Exterior Gateway Protocol)"},{9, "IGP(any private interior gateway (used by Cisco for their IGRP))"},
    {10, "BBN-RCC-MON(BBN RCC Monitoring)"},{11, "NVP-II(Network Voice Protocol)"},{12, "PUP(PARC Universal Packet)"},{13, "ARGUS"},{14, "EMCON(Emissions Control)"},
    {15, "XNET(Cross Net Debugger)"},{16, "CHAOS"},{17, "UDP(User Datagram)"},{18, "MUX(Multiplexing)"},{19, "DCN-MEAS(DCN Measurement Subsystems)"},
    {20, "HMP(Host Monitoring)"},{21, "PRM(Packet Radio Measurement)"},{22, "XNS-IDP(XEROX NS IDP)"},{23, "TRUNK-1"},{24, "TRUNK-2"},
    {25, "LEAF-1"},{26, "LEAF-2"},{27, "RDP(Reliable Data Protocol)"},{28, "IRTP(Internet Reliable Transaction)"},{29, "ISO-TP4(ISO Transport Protocol Class 4)"},
    {30, "NETBLT(Bulk Data Transfer Protocol)"},{31, "MFE-NSP(MFE Network Services Protocol)"},{32, "MERIT-INP(MERIT Internodal Protocol)"},{33, "DCCP(Datagram Congestion Control Protocol)"},{34, "3PC(Third Party Connect Protocol)"},
    {35, "IDPR(Inter-Domain Policy Routing Protocol)"},{36, "XTP(eXtreme Transaction Processing)"},{37, "DDP(Datagram Delivery Protocol)"},{38, "IDPR-CMTP(IDPR Control Message Transport Proto)"},{39, "TP++(TP++ Transport Protocol)"},
    {40, "IL(IL Transport Protocol)"},{41, "IPv6(IPv6 encapsulation)"},{42, "SDRP(Source Demand Routing Protocol)"},{43, "IPv6-Route(Routing Header for IPv6)"},{44, "IPv6-Frag(Fragment Header for IPv6)"},
    {45, "IDRP(Inter-Domain Routing Protocol)"},{46, "RSVP(Reservation Protocol)"},{47, "GRE(Generic Routing Encapsulation)"},{48, "DSR(Dynamic Source Routing Protocol)"},{49, "BNA(Burroughs Network Architecture)"},
    {50, "ESP(Encap Security Payload)"},{51, "AH(Authentication Header)"},{52, "I-NLSP(Integrated Net Layer Security TUBA)"},{53, "SWIPE(IP with Encryption)"},{54, "NARP(NBMA Address Resolution Protocol)"},
    {55, "MOBILE(IP Mobility)"},{56, "TLSP(Transport Layer Security Protocol using Kryptonet key management)"},{57, "SKIP(Simple Key-Management for Internet Protocol)"},{58, "IPv6-ICMP(ICMP for IPv6)"},{59, "IPv6-NoNxt(No Next Header for IPv6)"},
    {60, "IPv6-Opts(Destination Options for IPv6)"},{61, "Any host internal protocol"},{62, "CFTP(Comfortable FTP)"},{63, "any local network"},{64, "SAT-EXPAK(SATNET and Backroom EXPAK)"},
    {65, "KRYPTOLAN"},{66, "RVD(MIT Remote Virtual Disk Protocol)"},{67, "IPPC(Internet Pluribus Packet Core)"},{68, "any distributed file system"},{69, "SAT-MON(SATNET Monitoring)"},
    {70, "VISA(VISA Protocol)"},{71, "IPCV(Internet Packet Core Utility)"},{72, "CPNX(Computer Protocol Network Executive)"},{73, "CPHB(Computer Protocol Heart Beat)"},{74, "WSN(Wang Span Network)"},
    {75, "PVP(Packet Video Protocol)"},{76, "BR-SAT-MON(Backroom SATNET Monitoring)"},{77, "SUN-ND(SUN ND PROTOCOL)"},{78, "WB-MON(WIDEBAND Monitoring)"},{79, "WB-EXPAK(WIDEBAND EXPAK)"},
    {80, "ISO-IP(ISO Internet Protocol)"},{81, "VMTP(Versatile Message Transaction Protocol)"},{82, "SECURE-VMTP(Secure Versatile Message Transaction Protocol)"},{83, "VINES"},{84, "TTP(Time-Triggered Protocol)/IPTM(Internet Protocol Traffic Manager)"},
    {85, "NSFNET-IGP"},{86, "DGP(Dissimilar Gateway Protocol)"},{87, "TCF(Target Communication Framework)"},{88, "EIGRP"},{89, "OSPFIGP"},
    {90, "Sprite-RPC(Sprite RPC Protocol)"},{91, "LARP(Locus Address Resolution Protocol)"},{92, "MTP(Multicast Transport Protocol)"},{93, "AX.25(AX.25 Frames)"},{94, "IPIP(IP-within-IP Encapsulation Protocol)"},
    {95, "MICP(Mobile Internetworking Control Pro.)"},{96, "SCC-SP(Semaphore Communications Sec. Pro.)"},{97, "ETHERIP(Ethernet-within-IP Encapsulation)"},{98, "ENCAP(Encapsulation Header)"},{99, "any private encryption scheme"},
    {100, "GMTP"},{101, "IFMP(Ipsilon Flow Management Protocol)"},{102, "PNNI(PNNI over IP)"},{103, "PIM(Protocol Independent Multicast)"},{104, "ARIS"},
    {105, "SCPS"},{106, "QNX"},{107, "A/N(Active Networks)"},{108, "IPComp(IP Payload Compression Protocol)"},{109, "SNP(Sitara Networks Protocol)"},
    {110, "Compaq-Peer(Compaq Peer Protocol)"},{111, "IPX-in-IP(IPX in IP)"},{112, "VRRP(Virtual Router Redundancy Protocol)"},{113, "PGM(PGM Reliable Transport Protocol)"},{114, "any 0-hop protocol"},
    {115, "L2TP(Layer Two Tunneling Protocol)"},{116, "DDX(D-II Data Exchange)"},{117, "IATP(Interactive Agent Transfer Protocol)"},{118, "STP(Schedule Transfer Protocol)"},{119, "SRP(SpectraLink Radio Protocol)"},
    {120, "UTI(Universal Transport Interface Protocol)"},{121, "SMP(Simple Message Protocol)"},{122, "SM(Simple Multicast Protocol)"},{123, "PTP(Performance Transparency Protocol)"},{124, "IS-IS over IPv4(Intermediate System to Intermediate System Protocol over IPv4)"},
    {125, "FIRE(Flexible Intra-AS Routing Environment)"},{126, "CRTP(Combat Radio Transport Protocol)"},{127, "CRUDP(Combat Radio User Datagram)"},{128, "SSCOPMCE(Service-Specific Connection-Oriented Protocol in a Multilink and Connectionless Environment)"},{129, "IPLT"},
    {130, "SPS(Secure Packet Shield)"},{131, "PIPE(Private IP Encapsulation within IP)"},{132, "SCTP(	Stream Control Transmission Protocol)"},{133, "FC(Fibre Channel)"},{134, "RSVP-E2E-IGNORE(Reservation Protocol (RSVP) End-to-End Ignore)"},
    {135, "Mobility Header(Mobility Extension Header for IPv6)"},{136, "UDPLite(Lightweight User Datagram Protocol)"},{137, "MPLS-in-IP(Multiprotocol Label Switching Encapsulated in IP)"},{138, "manet(MANET Protocols)"},{139, "HIP(Host Identity Protocol)"},
    {140, "Shim6(Site Multihoming by IPv6 Intermediation)"},{141, "WESP(	Wrapped Encapsulating Security Payload)"},{142, "ROHC(Robust Header Compression)"},{143, "Ethernet(IPv6 Segment Routing)"},
};

typedef struct IPPacket //Структура ip пакета
{
    unsigned char  ip_ver_hlen;
    unsigned char  ip_tos;
    unsigned short ip_length;
    unsigned short ip_id;
    unsigned short ip_flag_offset;
    unsigned char  ip_ttl;
    unsigned char  ip_protocol;
    unsigned short ip_xsum;
    unsigned int   ip_srcaddr;
    unsigned int   ip_dstaddr;
    unsigned char  ip_data[];
};

using namespace std;
class Window_Sniffer : public QWidget {
  public:
    Window_Sniffer() {
        this->setFixedSize(900,600);

        if (rs_init(2,2))
        {
            error();
        }
        // 2. Создание сокета
        s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
        if(INVALID_SOCKET == s)
        {
            error();
            rs_exit();
        }

        // 3. Определение имени и IP-адреса компьютера
        if(gethostname(name, sizeof(name)))
        {
            error();
            rs_exit();
        }
        host_inf = gethostbyname(name );
        ZeroMemory( &addr, sizeof(addr) );
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = ((struct in_addr *)host_inf->h_addr_list[0])->s_addr;
        // 4. Привязка сокета к IP-адресу компьютера
        if(bind(s, (SOCKADDR *)&addr, sizeof(SOCKADDR)))
        {
            error();
            rs_exit();
        }
        buffer = (char *) calloc(max_buf_len, sizeof (char));
        // 5. Включение promiscuous mode
        if(ioctlsocket(s, 0x98000001, &flag))
        {
            error();
            rs_exit();
        }




        button_sniff->setText("Sniff");
        button_sniff->setGeometry(20,500,80,30);
        connect(button_sniff, &QPushButton::clicked, this, &Window_Sniffer::button_sniff_press);

        button_stop->setText("Stop");
        button_stop->setGeometry(100,500,80,30);
        connect(button_stop, &QPushButton::clicked, this, &Window_Sniffer::button_stop_press);

        button_change->setText("Change");
        button_change->setGeometry(520,20,80,30);
        connect(button_change, &QPushButton::clicked, this, &Window_Sniffer::button_change_press);

        QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
        QRegularExpression ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
        QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
        spec_source->setValidator(ipValidator);
        spec_source->setGeometry(20,450,120,20);
        spec_source->setReadOnly(false);
        spec_source->setEnabled(true);
        spec_source->setPlaceholderText("Specify source");
        spec_source->setFont(QFont("Calibri", 13));

        sniff_ip->setValidator(ipValidator);
        sniff_ip->setGeometry(400,25,120,20);
        sniff_ip->setReadOnly(false);
        sniff_ip->setEnabled(true);
        sniff_ip->setPlaceholderText("IP to Sniff");
        sniff_ip->setFont(QFont("Calibri", 13));

        // Результат подключения
        label->setText("Успешно подключено");
        label->setFixedHeight(25);
        label->setFixedWidth(300);
        label->setGeometry(65,22,25,300);
        label->setFont(QFont("Calibri", 20));

        label_ippacket->setText("IP Packet info: ");
        label_ippacket->setGeometry(20,60,400,20);
        label_ippacket->setFont(QFont("Calibri", 13));

        label_version->setText("Version: ");
        label_version->setGeometry(20,90,400,20);
        label_version->setFont(QFont("Calibri", 13));

        label_header_length->setText("Header length: ");
        label_header_length->setGeometry(20,120,400,20);
        label_header_length->setFont(QFont("Calibri", 13));

        label_tos->setText("Type of service(TOS): ");
        label_tos->setGeometry(20,150,400,20);
        label_tos->setFont(QFont("Calibri", 13));

        label_packet_length->setText("Packet length: ");
        label_packet_length->setGeometry(20,180,400,20);
        label_packet_length->setFont(QFont("Calibri", 13));

        label_id->setText("ID: ");
        label_id->setGeometry(20,210,400,20);
        label_id->setFont(QFont("Calibri", 13));

        label_flags->setText("Flags: ");
        label_flags->setGeometry(20,240,400,20);
        label_flags->setFont(QFont("Calibri", 13));

        label_offset->setText("Offset: ");
        label_offset->setGeometry(20,270,400,20);
        label_offset->setFont(QFont("Calibri", 13));

        label_ttl->setText("Time to live(TTL): ");
        label_ttl->setGeometry(20,300,400,20);
        label_ttl->setFont(QFont("Calibri", 13));

        label_protocol->setText("Protocol: ");
        label_protocol->setGeometry(20,330,400,20);
        label_protocol->setFont(QFont("Calibri", 13));

        label_checksum->setText("Checksum: ");
        label_checksum->setGeometry(20,360,400,20);
        label_checksum->setFont(QFont("Calibri", 13));

        label_source->setText("Source: ");
        label_source->setGeometry(20,390,400,20);
        label_source->setFont(QFont("Calibri", 13));

        label_destination->setText("Destination: ");
        label_destination->setGeometry(20,420,400,20);
        label_destination->setFont(QFont("Calibri", 13));

        label_ipdata->setText("Data: ");
        label_ipdata->setGeometry(400,90,480,490);
        label_ipdata->setFont(QFont("Calibri", 13));
        label_ipdata->setAlignment(Qt::AlignLeft);
    }

public slots:
    void button_sniff_press(){
        to_sniff();
    };
    void button_change_press(){
        if(sniff_ip->text() != ""){
            reset();

        }
        else {
            closesocket (s);
            if (rs_init(2,2))
            {
                error();
            }
            // 2. Создание сокета
            s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
            if(INVALID_SOCKET == s)
            {
                error();
                rs_exit();
            }
            // 3. Определение имени и IP-адреса компьютера
            if(gethostname(name, sizeof(name)))
            {
                error();
                rs_exit();
            }
            host_inf = gethostbyname(name );
            ZeroMemory( &addr, sizeof(addr) );
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = ((struct in_addr *)host_inf->h_addr_list[0])->s_addr;;

            // 4. Привязка сокета к IP-адресу компьютера
            if(bind(s, (SOCKADDR *)&addr, sizeof(SOCKADDR)))
            {
                error();
                rs_exit();
            }
                buffer = (char *) calloc
                    (max_buf_len, sizeof (char));
            // 5. Включение promiscuous mode
            if(ioctlsocket(s, 0x98000001, &flag))
            {
                error();
                rs_exit();
            }
            label->setText("Успешно подключено");
            flag_connection = 1;
            update();
            button_sniff->setEnabled(true);
        }
    };
    void button_stop_press(){
        flag_loop = 0;
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
    void reset()
    {
        closesocket (s);
        if (rs_init(2,2))
        {
            error();
        }
        // 2. Создание сокета
        s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
        if(INVALID_SOCKET == s)
        {
            error();
            rs_exit();
        }
        ZeroMemory( &addr, sizeof(addr) );
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(sniff_ip->text().toStdString().c_str());
        sniff_ip->setText("");
        // 4. Привязка сокета к IP-адресу компьютера
        if(bind(s, (SOCKADDR *)&addr, sizeof(SOCKADDR)))
        {
            error();
            rs_exit();
        }
            buffer = (char *) calloc
                (max_buf_len, sizeof (char));
        // 5. Включение promiscuous mode
        if(ioctlsocket(s, 0x98000001, &flag))
        {
            error();
            rs_exit();
        }

    }
    void paintEvent(QPaintEvent *event){

        Q_UNUSED(event);
        QPainter painter(this);
        QPen pen(Qt::black, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawRect(20, 20, 30, 30);
        if(flag_connection == 1){
            QPen pen(QColor(77,167,30), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(15, 35, 35, 45);
            painter.drawLine(35, 45, 60, 20);
            update();
        }
        else if(flag_connection == 0){
            QPen pen(QColor(236,71,25), 5, Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(20, 20, 50, 50);
            painter.drawLine(20, 50, 50, 20);
            update();
        }

    };
    // Перехватить пакет
    void to_sniff(){
        flag_loop = 1;
        // Приём IP-пакетов
        button_sniff->setEnabled(false);
        while(flag_loop)
        {
            QCoreApplication::processEvents();
            count = recv (s, buffer, max_buf_len, 0);
            if(count >= sizeof(IPPacket))
            {
                // Обработка IP-пакета
                 ip = (IPPacket*)buffer;
                 IN_ADDR temp_addr;
                 temp_addr.s_addr = ip->ip_srcaddr;
                 if(spec_source->text() != "" && spec_source->text() != QString::fromStdString(inet_ntoa(temp_addr))){
                     continue;
                 }

                     label_version->setText("Version: " + QString::number(static_cast<int>(ip->ip_ver_hlen >> 4)));
                     label_header_length->setText("Header length: " + QString::number(static_cast<int>(ip->ip_ver_hlen & 15)));
                     label_tos->setText("Type of service(TOS): " + QString::fromStdString(bitset<8>(ip->ip_tos).to_string()));
                     label_packet_length->setText("Packet length: " + QString::number(htons(ip->ip_length)));
                     label_id->setText("ID: " + QString::number(htons(ip->ip_id)));
                     label_flags->setText("Flags: " + QString::fromStdString(bitset<3>(ip->ip_flag_offset >> 13).to_string()));
                     label_offset->setText("Offset: " + QString::fromStdString(bitset<13>(ip->ip_flag_offset & 8191).to_string()));
                     label_ttl->setText("Time to live(TTL): " + QString::number(static_cast<int>(ip->ip_ttl)));
                     std::map<int,std::string>::iterator it;
                     it = map_protocol.find(ip->ip_protocol);
                     if(it != map_protocol.end()) label_protocol->setText("Protocol: " + QString::fromStdString(map_protocol.at(ip->ip_protocol)));
                     else label_protocol->setText("Protocol: Not Found(Non standart)");
                     label_checksum->setText("Checksum: " + QString("%1").arg(ntohs(ip->ip_xsum),0,16));
                     label_source->setText("Source: " + QString::fromStdString(inet_ntoa(temp_addr)));
                     temp_addr.s_addr = ip->ip_dstaddr;
                     label_destination->setText("Destination: " + QString::fromStdString(inet_ntoa(temp_addr)));
                     label_ipdata->setText("Data: \n");
                     for(int i = 0; i < ntohs(ip->ip_length)  - (ip->ip_ver_hlen & 15) * 4 ; i++){
                         if(ip->ip_data[i] == 0){
                             label_ipdata->setText(label_ipdata->text() + ".");
                         }else{
                             label_ipdata->setText(label_ipdata->text() + QString((char)ip->ip_data[i]));
                         }
                         if(i%64==0 & i!=0){
                             label_ipdata->setText(label_ipdata->text() + "\n");
                         }
                     }
                     break;


            }

       }
        // Освобождаем память буфера
        free(buffer);
        buffer = (char *) calloc(max_buf_len, sizeof (char));
        button_sniff->setEnabled(true);


    }

    void error(){
        label->setText(QString::number(WSAGetLastError()));
        flag_connection = 0;
        update();
        button_sniff->setEnabled(false);
    }
    char name[128];	// Имя хоста (компьютера).
    HOSTENT* host_inf;	// Информация о хосте.
    SOCKADDR_IN addr;	// Адрес компьютера
    IPPacket* ip;
    int count = 0;
    char* buffer;
    // Максимальный размер буфера для приёма данных - 64k
    const unsigned int max_buf_len = 0x10000;
    bool flag_connection = 1;
    bool flag_loop = 1;
    unsigned long flag = 1;
    QLineEdit* spec_source = new QLineEdit(this);
    QLineEdit* sniff_ip = new QLineEdit(this);

    QLabel* label = new QLabel(this);
    QLabel* label_ippacket = new QLabel(this);
    QLabel* label_version = new QLabel(this);
    QLabel* label_header_length = new QLabel(this);
    QLabel* label_tos = new QLabel(this);
    QLabel* label_packet_length = new QLabel(this);
    QLabel* label_id = new QLabel(this);
    QLabel* label_flags = new QLabel(this);
    QLabel* label_offset = new QLabel(this);
    QLabel* label_ttl = new QLabel(this);
    QLabel* label_protocol = new QLabel(this);
    QLabel* label_checksum = new QLabel(this);
    QLabel* label_source = new QLabel(this);
    QLabel* label_destination = new QLabel(this);
    QLabel* label_ipdata = new QLabel(this);
    QPainter painter;
    QPushButton *button_sniff = new QPushButton(this);
    QPushButton *button_change = new QPushButton(this);
    QPushButton *button_stop = new QPushButton(this);

    SOCKET s;

};

class Window_ICMP : public QWidget {
  public:
    Window_ICMP() {
        this->setFixedSize(300,300);
        QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
        QRegularExpression ipRegex ("^" + ipRange+ "\\." + ipRange+ "\\." + ipRange+ "\\." + ipRange + "$");
        QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
        LineEd->setValidator(ipValidator);
        button_send->setText("Ping");
        button_send->setGeometry(20,240,80,30);
        connect(button_send, &QPushButton::clicked, this, &Window_ICMP::button_send_press);
        LineEd->setGeometry(20,200,120,30);
        LineEd->setFocus();
        LineEd->setReadOnly(false);
        LineEd->setEnabled(true);
        // Результат подключения
        label->setText("");
        label->setGeometry(20,50,280,200);
        label->setFont(QFont("Calibri", 15));
        label->setAlignment(Qt::AlignLeft);
    }

public slots:
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
    void send_to_client(){
        HANDLE handle;  // Обработчик
        unsigned long ping_addr = INADDR_NONE;
        DWORD reply_number = 0;
        char send_buf[32] = "";
        LPVOID reply_buf = NULL;
        DWORD reply_buff_size = 0;
        ping_addr = inet_addr(LineEd->text().toStdString().c_str());
        handle = IcmpCreateFile();
        reply_buff_size  = sizeof(ICMP_ECHO_REPLY) + sizeof(send_buf);
        reply_buf = (VOID*) malloc(reply_buff_size );
        // Эхо-запрос
        reply_number = IcmpSendEcho(handle, ping_addr, send_buf, sizeof(send_buf),NULL, reply_buf, reply_buff_size , 1000);
        label->setText("");
        if (reply_number != 0) {
            // Эхо-ответ
            PICMP_ECHO_REPLY reply = (PICMP_ECHO_REPLY)reply_buf;
            struct in_addr reply_addr;
            reply_addr.S_un.S_addr = reply->Address;
            label->setText("Получен(-о) " + QString::number(reply_number) + " эхо-ответ(-ов).\n");
            label->setText(label->text() + "Время отклика = " + QString::number(reply->RoundTripTime) + "ms.");
            flag = 1;
        } else {
            label->setText("Ошибка " + QString::number(GetLastError()));
            flag = -1;
        }

        free(reply_buf); // Освобождаем память
         LineEd->setFocus();
    }
    int8_t flag = 0;
    QLineEdit* LineEd = new QLineEdit(this);
    QLabel* label = new QLabel(this);
    QPainter painter;
    QPushButton *button_send = new QPushButton(this);
    SOCKADDR_IN addr;
    SOCKET s;
};

int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        Window_ICMP win_icmp;
        Window_Sniffer win_sniffer;
        win_icmp.show();
        win_sniffer.show();
        return app.exec();
}
