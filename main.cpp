#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include <pqxx/pqxx>
//Include the header of the new main window.
#include "plotwindow.h"

using namespace std;
using namespace pqxx;


int tlgetch() {
    struct termios oldtc, newtc;
    int ch;
    tcgetattr(STDIN_FILENO, &oldtc);
    newtc = oldtc;
    newtc.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
    ch=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
    return ch;
}

const std::string EnterPassword(int lon) {
    char pw[20];
    char ch;
    int i = 0;

    cout << "Enter password: ";
//  my_getpass(pwd, 10, cin);
    ch = '\00';
    while(ch != '\n') //Loop until 'Enter' is pressed
    {
        ch = tlgetch();
        if (ch == 127)  {
            if (i > 0) {
                cout << '\b';  //Cursor moves 1 position backwards
                i -=1;
            }
            continue;
        }
        if (i<lon) {
            pw[i] = ch;
            if (ch != '\n') {
                cout << "*";
            }
            i += 1;
        }
    }
    pw[i - 1] = '\00';
    cout << endl;
    return pw;
}

int main (int argc, char *argv[])
{

    string db, host, port, usr, pwd, con;
    int len;

    if (argc ==  2) {
        if (string(argv[1]) == "-h") {
            //123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|
            cout << "Trading Lab" << endl;
            cout << "Data are registered in PostgreSQL data base." << endl;
            cout << "Before tlab execution call:" << endl;
            cout << "  $ postgres -D /usr/local/pgsql/data." << endl;
            cout << "Usage:" << endl;
            cout << "  tlab -d<dbname> -h<hostaddr> -p<port> -u<user> -p[<password>]" << endl;
            cout << "General options:" << endl;
            cout << "  -d <dbname>   PostgreSQL data base name" << endl;
            cout << "  -h <hostaddr> Host address, for local host is 127.0.0.1" << endl;
            cout << "  -p aport>     Port, usually is 5432" << endl;
            cout << "  -u <user>     User,  database name registered" << endl;
            cout << "  -p<password>  Password, optional, when void manual entry" << endl;
            cout << "Example:" << endl;
            cout << "  $ ./tlrec -dmydb -h127.0.0.1 -p5432 -ujordi -p" << endl;

        }
        if (string(argv[1]) == "-v") {
            cout<< "tlrec (1.0.0)" << endl;
        }
        return 0;
    }
    if (argc !=  6) {
        cout << "Usage is tlrec -d<dbname> -h<hostaddr> -p<port> -u<user> -p[<password>]" << endl;
        cout << "General options:" << endl;
        cout << "  -h for help" << endl;
        cout << "  -v for version" << endl;
        return 0;
    }

    if (string(argv[1]).substr(0,2) == "-d") {
        len = string(argv[1]).size();
        db = string(argv[1]).substr(2,len-2);
    }

    if (string(argv[2]).substr(0,2) == "-h") {
        len = string(argv[2]).size();
        host = string(argv[2]).substr(2,len-2);
    }

    if (string(argv[3]).substr(0,2) == "-p") {
        len = string(argv[3]).size();
        port = string(argv[3]).substr(2,len-2);
    }

    if (string(argv[4]).substr(0,2) == "-u") {
        len = string(argv[4]).size();
        usr = string(argv[4]).substr(2,len-2);
    }

    if (string(argv[5]).substr(0,2) == "-p") {
        len = string(argv[5]).size();
        pwd = string(argv[5]).substr(2,len-2);
    }

    if (pwd == "") pwd = EnterPassword(10);

    con = "dbname=";
    con.append(db);
    con.append(" user=");
    con.append(usr);
    con.append(" password=");
    con.append(pwd);
    con.append(" hostaddr=");
    con.append(host);
    con.append(" port=");
    con.append(port);


//     int today, newYear;
//     today   = Julian.calc_julianToday();
//     newYear = Julian.calc_juliandate(2015, 1, 1);
//     std::cout << today << newYear << std::endl;

    string sql;

    try {
//        connection C("dbname=mydb user=jordi password=XXXXXXXX hostaddr=127.0.0.1 port=5432");
        connection C(con.c_str());
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        KAboutData aboutData( "tlab", 0,
                              ki18n("tlab"), "1.0",
                              ki18n("Technology lab and advanced trading strategies"),
                              KAboutData::License_GPL,
                              ki18n("Copyright (c) 2015 Jordi Estrada") );
        argc = 1;
        KCmdLineArgs::init( argc, argv, &aboutData );

        KApplication app;
        PlotWindow* mainWindow = new PlotWindow(C);
//        while(true) {
        mainWindow->show();
//            return app.exec();
//            pqxx::internal::sleep_seconds(1);
//        }
        C.disconnect ();
        cout << "Closed database successfully: " << C.dbname() << endl;
        return app.exec();

        pqxx::internal::sleep_seconds(4);



        //     QTimer *timer = new QTimer(this);
//     connect(timer, SIGNAL(timeout()),
//             logic, SLOT( nextTimeStep() ) );
//     connect(timer, SIGNAL(timeout()),
//             ballGraph, SLOT(nextAnimationFrame()));
//     timer->start(10);

    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }
}
