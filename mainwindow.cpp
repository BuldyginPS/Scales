#include "mainwindow.h"
//#include "func.cpp"
#include "./ui_mainwindow.h"


//#include "mainwindow.h"

using namespace std;

HANDLE hSerial;

int weight_start=0;

int hex4(QVector<int> numb1,QVector<int> numb2)
{
    QVector<int> hex,jrhex,mjhex;
    int jr=0, mj=0;

    for(int i=0;i<8;i++)
    {
        jr += numb1[i] * pow(2, i);
        mj += numb2[i] * pow(2, i);
    }

    do {
        jrhex.insert(jrhex.end(), jr % 16);
        jr = jr / 16;
    } while (jr != 0);

    do {
        mjhex.insert(mjhex.end(), mj % 16);
        mj = mj / 16;
    } while (mj != 0);

    int buff =0;
    for(int i=0;i<jrhex.size();i++)
    {
        buff = jrhex[i];
        hex.insert(i, buff);
    }

    for(int i=jrhex.size(),j=0;i<(jrhex.size()+mjhex.size());j++,i++)
    {
        buff = mjhex[j];
        hex.insert(i, buff);
    }

    int numb = 0;
    for (int j = 0; j < hex.size();j++)
    {
        numb += hex[j] * pow(16, j);
    }

    return numb;
}


QVector<int> AsciitoBin(char ch)
{
    int i;
    bool opposite = FALSE;
    QVector<int> s(8);


    for (i = 0; i < 8; i++) {
        s[i] = ch % 2;
        ch = ch / 2;
        if (s[i] == -1) opposite = TRUE;
    }
    //для отрицательных
    if(opposite)
    {
        for (i = 0; i < 8; i++)
        {
            if (s[i] == -1) s[i] = 1;
        }
        for (i = 0; i < 8; i++)
        {
            if (s[i] == 1) s[i] = 0;
            else  s[i] = 1;
        }
        //+1
        bool flag = TRUE;
        for (i = 7; i < 0; i--)
        {
            if (s[i] == 0 && flag)
            {
                s[i] = 1;
                flag = FALSE;
            }
        }
    }

    return s;
}

int ReadCOM(HANDLE hSerial)
{
    DWORD iSize;
    char sReceivedChar;
    QVector<int> numb1, numb2;
    int  numb = 0, i = 0;

    while (i<10)
    {
        ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);
        if (iSize > 0)
        {            
            if (i == 2) numb1 = AsciitoBin(sReceivedChar);

            if (i == 3)
            {
                //numb2 = (int)sReceivedChar;
                numb2 = AsciitoBin(sReceivedChar);
                numb = hex4(numb1, numb2);
            }
        }
        i++;
    }
    return numb;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    LPCTSTR sPortName = L"COM7";
    hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);

    if (hSerial != INVALID_HANDLE_VALUE) {
        qDebug() << "open!\n";
    }
    else {
        qDebug() << "cant open! \n";
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        qDebug() << "getting state error\n";
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        qDebug() << "error setting serial port state\n";
    }

    //char data[] = { 0xF8, 0x55, 0xCE, 0x0001, 0x23, 0x82,0x9B };
    char data[] = "CMD_GET_MASSA";
    DWORD dwSize = sizeof(data);
    DWORD dwBytesWritten;

    BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);

     qDebug() << dwSize << " Bytes in string. " << dwBytesWritten << " Bytes sended. \n" ;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    QTime time = QTime::currentTime();

    int msec;
    //weight_start =ReadCOM(hSerial);
    int weight=weight_start,StopWeight;
    StopWeight = ui->textEdit_2->toPlainText().split(" ")[0].toInt();
    //qDebug() << StopWeight;
    while(weight_start-weight<StopWeight)
    {
        weight = ReadCOM(hSerial);
        qDebug() << weight;
    }
    msec = time.msecsTo( QTime::currentTime() );
    qDebug() << msec ;//"OVER";
    ui->label_3->setText(QString::number((double)msec/1000)+" sec");

}
/*
void MainWindow::taking_data()
{
    keybd_event(VK_LCONTROL ,0, 0, 0);
    keybd_event('J' ,0, 0, 0);
    keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
    keybd_event('J', 0, KEYEVENTF_KEYUP, 0);
    qDebug() << "Data taking";
}
*/

void MainWindow::on_startButton_2_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->setFocus();

    weight_start = ReadCOM(hSerial);
    ui->textEdit->textCursor().insertText(QString::number(weight_start));
}

