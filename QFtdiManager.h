#ifndef QFTDIMANAGER_H
#define QFTDIMANAGER_H

#include <QObject>
#include <QtCore>
#include <QDateTime>
#include <QTimer>
#include <QtDebug>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include "Android.h"


enum FTDI_BAUDRATE_
{
	FTDI_BAUDRATE_300    = 300,
	FTDI_BAUDRATE_600    = 600,
	FTDI_BAUDRATE_1200   = 1200,
	FTDI_BAUDRATE_2400   = 2400,
	FTDI_BAUDRATE_4800   = 4800,
	FTDI_BAUDRATE_9600   = 9600,
	FTDI_BAUDRATE_19200  = 19200,
	FTDI_BAUDRATE_38400  = 38400,
	FTDI_BAUDRATE_57600  = 57600,
	FTDI_BAUDRATE_115200 = 115200,
	FTDI_BAUDRATE_230400 = 230400,
	FTDI_BAUDRATE_460800 = 460800,
	FTDI_BAUDRATE_921600 = 921600
};
Q_DECLARE_FLAGS(FTDI_BAUDRATE, FTDI_BAUDRATE_)

enum FTDI_PARITY_
{//ok
    FTDI_PARITY_NONE  = 0,
    FTDI_PARITY_ODD   = 1,
    FTDI_PARITY_EVEN  = 2,
    FTDI_PARITY_MARK  = 3,
    FTDI_PARITY_SPACE = 4
};
Q_DECLARE_FLAGS(FTDI_PARITY, FTDI_PARITY_)

enum FTDI_STOP_BITS_
{//ok
    FTDI_STOP_BITS_1  = 0,
    FTDI_STOP_BITS_2  = 2
};
Q_DECLARE_FLAGS(FTDI_STOP_BITS, FTDI_STOP_BITS_)

enum FTDI_DATA_BITS_
{//ok
    FTDI_DATA_BITS_7 = 7,
    FTDI_DATA_BITS_8 = 8
};
Q_DECLARE_FLAGS(FTDI_DATA_BITS, FTDI_DATA_BITS_)

enum FTDI_FLOW_CTRL_
{//ok
    FTDI_FLOW_CTRL_NONE     = 0,
    FTDI_FLOW_CTRL_RTS_CTS  = 256,
    FTDI_FLOW_CTRL_DTR_DSR  = 512,
    FTDI_FLOW_CTRL_XON_XOFF = 1024
};
Q_DECLARE_FLAGS(FTDI_FLOW_CTRL, FTDI_FLOW_CTRL_)

enum FTDI_BREAK_
{//ok
    FTDI_BREAK_OFF = 0,
    FTDI_BREAK_ON  = 16384
};
Q_DECLARE_FLAGS(FTDI_BREAK, FTDI_BREAK_)

class QFtdiManager : public QObject
{
    Q_OBJECT

public:
    static QFtdiManager *ftdiManager;
    explicit QFtdiManager();
    ~QFtdiManager();

    static QFtdiManager *getInstance();
    QString errorString();
    void clearError();

public slots:
    QStringList getDeviceCount();

private:
    QAndroidJniObject jFtdiManager;
    QAndroidJniObject androidActivity;

    QString errorStringMsg;

};

#endif // QFTDIMANAGER_H
