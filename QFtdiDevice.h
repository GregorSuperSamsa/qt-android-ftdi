#ifndef QFTDIDEVICE_H
#define QFTDIDEVICE_H

#include "QFtdiManager.h"
#include <QVector>
#include <QtAndroid>
#include "Android.h"


class QFtdiDevice : public QObject
{
    Q_OBJECT
public:
	explicit QFtdiDevice(QObject *parent = 0);
    ~QFtdiDevice();
	
	struct Settings
	{
		int portIndex;
		int baudRate;
		quint8 stopBit;
		quint8 dataBit;
		quint8 parity;
		quint8 flowControl;
    };
	Settings settings;
	
	struct Information
	{
		int flags;
		short bcdDevice;
		int type;
		quint8 iSerialNumber;
		int id;
		int location;
		QString serialNumber;
		QString description;
		int handle;
		int breakOnParam;
		short modemStatus;
		short lineStatus;
    };
	Information information;

    struct Parameters 
	{
		int bufferSize = 16384;
		int maxTransferSize = 16384;
		int nrBuffers = 16;
		int rxTimeout = 5000;
    };
	Parameters parameters;
	
private:
    qint64 objectIndex;
    int deviceIndex;
    QByteArray rxBytes;
	QString rxString;
	quint8 latency;
    // jni
    QAndroidJniObject jFtdiDevice;
    QAndroidJniObject androidActivity;

    QString errorStringMsg;
	
public:
    bool reset();
    bool isOpen();
    bool setPortName(const QString &name);
    bool openPort(FTDI_BAUDRATE baudRate,
                  FTDI_STOP_BITS stopBits,
                  FTDI_DATA_BITS dataBits,
                  FTDI_PARITY parity,
                  FTDI_FLOW_CTRL flowControl);
    bool close();
    bool sendString(QString txData, QString &status);
    bool write(QByteArray txData);
    QByteArray readAll();
	bool purgeRxTx(bool rx = true, bool tx = true);
	bool setLatencyTimer(quint8 latency);
	quint8 getLatencyTimer();
	void setBufferSize(int size);
	int getBufferSize();
	void setMaxTransferSize(int size);
	int getMaxTransferSize();
	void setBufferNumber(int number);
	int getBufferNumber();
	void setReadTimeout(int timeout);
    int getReadTimeout();
    bool getDeviceCount(int &deviceCount);
    QString errorString();
    void clearError();
    bool deleteLocalRef(jobject jObject);
    bool deleteGlobalRef(jobject jObject);
    bool exception(QString debugMessage);

    static void jReadyRead(JNIEnv *env, jobject thiz, jlong jObjectIndex, jint jDeviceIndex);

    jbyteArray QByteArray2jbyteArray(QByteArray buf);
    QByteArray jbyteArray2QByteArray(jbyteArray buf);

    static qint64 staticObjectIndex;
    static QVector<QSharedPointer<QFtdiDevice>> objectList;

signals:
    void readyRead();
    void signalDeviceIndex(int i);
	
};

#endif // QFTDIDEVICE_H
