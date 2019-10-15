#include "QFtdiDevice.h"


QVector<QSharedPointer<QFtdiDevice>> QFtdiDevice::objectList;
qint64 QFtdiDevice::staticObjectIndex = 0;

QFtdiDevice::QFtdiDevice(QObject *parent) : QObject(parent)
{
    this->androidActivity = QtAndroid::androidActivity();
    if (NULL == this->androidActivity)
    {
        errorStringMsg = "Java Exception: bool QFtdiDevice::androidActivity is NULL!";
    }

    jFtdiDevice =  QAndroidJniObject(QByteArray("org/qtproject/qt5/JFtdiDevice").constData());
    exception("Java Exception: bool QFtdiDevice::QFtdiDevice(QObject *parent)");

    if (!jFtdiDevice.isValid()) {
        errorStringMsg = "Java Exception: bool QFtdiDevice::jFtdiDevice is NULL!";
    }

    int i = 0;
    while(i < objectList.count()) {
        if (NULL == objectList.at(i)) {
            objectList.remove(i);
        }
        else {
            i++;
        }
    }
    objectIndex = staticObjectIndex;
    objectList.append(QSharedPointer<QFtdiDevice>(this));
    staticObjectIndex++;

    jlong jObjectIndex = objectIndex;
    jFtdiDevice.callMethod<jint>(QByteArray("getInstance").constData(),
                                 QByteArray("(JLandroid/content/Context;)I").constData(),
                                 jObjectIndex,
                                 androidActivity.object());
    exception("Java Exception: bool QFtdiDevice::jFtdiDevice.getInstance");


}

QFtdiDevice::~QFtdiDevice()
{


    if (jFtdiDevice.isValid()) {

        if (isOpen()) close();
        deleteGlobalRef(jFtdiDevice.object());
    }

//qDebug()<<"DBG:";
//qDebug()<<"DBG:"<<__func__;
//qDebug()<<"DBG:"<<"objectList.count() ="<<objectList.count();
//qDebug()<<"DBG:";
}

bool QFtdiDevice::isOpen()
{
    jint result = 0;

    if (jFtdiDevice.isValid()) {
        result = jFtdiDevice.callMethod<jint>(QByteArray("isOpen").constData());
        exception("Java Exception: bool QFtdiDevice::isOpen()");
    }

    switch (result) {
    case -1:
        errorStringMsg = "Invalid device index!";
        return false;
    case -2:
        errorStringMsg = "Device is NULL!";
        return false;
        break;
    case -3:
        errorStringMsg = "Device not open!";
        return false;
    case 1:
        return true;
        break;
    default:
        errorStringMsg = "Unknown error!";
        return false;
        break;
    }

    return false;
}

bool QFtdiDevice::setPortName(const QString &name)
{
    bool ok = false;
    int deviceIndex = name.toInt(&ok);

    if (!ok)
    {
        errorStringMsg = "QFtdiDevice::setPortName(QString name): invalid port name, should correspond to port index!";
        return false;
    }

    this->deviceIndex = deviceIndex;
    jint jindex = deviceIndex;
    jint result = jFtdiDevice.callMethod<jint>(QByteArray("setIndex").constData(),
                                                     QByteArray("(I)I").constData(),
                                                     jindex);

    qDebug()<<__func__<<"JFtdiDevice:: setIndex() =>"<<result;

exception("Java Exception: bool QFtdiDevice::setPortIndex()");

    if (result < 0) {
        errorStringMsg = "Java Error: bool QFtdiDevice::setPortIndex() returned " + QString::number(result);
        return false;
    }

    //Android::dumpLocalRefTable();

    return true;
}

bool QFtdiDevice::openPort(
        FTDI_BAUDRATE baudRate,
        FTDI_STOP_BITS stopBits,
        FTDI_DATA_BITS dataBits,
        FTDI_PARITY parity,
        FTDI_FLOW_CTRL flowControl)
{

    jint jBaudRate =    (jint) baudRate;
    jint jStopBits =    (jint) stopBits;
    jint jDataBits =    (jint) dataBits;
    jint jParity =      (jint) parity;
    jint jFlowControl = (jint) flowControl;

    jint result = jFtdiDevice.callMethod<jint>(
                QByteArray("open").constData(),
                QByteArray("(IIIII)I").constData(),
                jBaudRate,
                jStopBits,
                jDataBits,
                jParity,
                jFlowControl
                );
    exception("Java Exception: bool QFtdiDevice::openPort()");

    switch (result) {
    case -1:
        errorStringMsg = "Invalid device index!";
        return false;
    case -2:
        errorStringMsg = "Device is NULL!";
        return false;
        break;
    case -3:
        errorStringMsg = "Device not open!";
        return false;
    case 1:
        return true;
        break;
    default:
        errorStringMsg = "Unknown error!";
        return false;
        break;
    }

    return false;
}

bool QFtdiDevice::getDeviceCount(int &deviceCount)
{
    jint result = jFtdiDevice.callMethod<jint>(QByteArray("getDeviceCount").constData());
    exception("Java Exception: bool QFtdiDevice::getDeviceCount()");

    deviceCount = result;

    return true;
}

bool QFtdiDevice::close()
{
    jint result = 0;

    if (jFtdiDevice.isValid()) {
        result = jFtdiDevice.callMethod<jint>(QByteArray("close").constData());
        exception("Java Exception: bool QFtdiDevice::closePort()");

        qDebug()<<__func__<<"JFtdiDevice:: close() = >"<<result;
    }
    switch (result) {
    case -1:
        errorStringMsg = "Invalid device index!";
        return false;
    case -2:
        errorStringMsg = "Device is NULL!";
        return false;
        break;
    case -3:
        errorStringMsg = "Device not closed!";
        return false;
    case 1:
        return true;
        break;
    default:
        errorStringMsg = "Unknown error!";
        return false;
        break;
    }

    return false;
}

bool QFtdiDevice::sendString(QString txData, QString &status)
{
    // TODO
    Q_UNUSED(txData)
    Q_UNUSED(status)

    return false;
}

bool QFtdiDevice::write(QByteArray txData)
{
    jbyteArray jTxData = QByteArray2jbyteArray(txData);
    exception("Java Exception: jbyteArray QByteArray2jbyteArray()");

    jint result = jFtdiDevice.callMethod<jint>(QByteArray("sendBytes").constData(),
                                               QByteArray("([B)I").constData(),
                                               jTxData
                                               );
    exception("Java Exception: bool QFtdiDevice::sendBytes()");

    deleteLocalRef(jTxData);

    switch (result) {
    case -1:
        errorStringMsg = "Invalid device index!";
        return false;
    case -2:
        errorStringMsg = "Device is NULL!";
        return false;
        break;
    case -3:
        errorStringMsg = "Device not open!";
        return false;
    case -4:
        errorStringMsg = "Invalid rx data length!";
        return false;
    case -5:
        errorStringMsg = "Unable to write data!";
        return false;
    case 1:
        return true;
        break;
    default:
        errorStringMsg = "Unknown error!";
        return false;
        break;
    }

    return false;
}

QByteArray QFtdiDevice::readAll()
{
    QByteArray rxData;

    QAndroidJniObject jRxDataObj = jFtdiDevice.callObjectMethod<jbyteArray>(QByteArray("readBytes").constData());
    exception("Java Exception: QByteArray QFtdiDevice::readAll()");

    if (jRxDataObj.isValid())
    {
        jbyteArray jRxData = jRxDataObj.object<jbyteArray>();
        rxData = jbyteArray2QByteArray(jRxData);
        exception("Java Exception: QByteArray jbyteArray2QByteArray()");
    }

    //!
    //deleteLocalRef(jRxDataObj.object());


    return rxData;
}

bool QFtdiDevice::purgeRxTx(bool rx, bool tx)
{
    // TODO
    Q_UNUSED(rx)
    Q_UNUSED(tx)

    return false;
}

bool QFtdiDevice::setLatencyTimer(quint8 latency)
{
    // TODO
    Q_UNUSED(latency)

    return false;
}

quint8 QFtdiDevice::getLatencyTimer()
{
    // TODO
    return 0;
}

void QFtdiDevice::setBufferSize(int size)
{
    // TODO
    if (size < 64) size = 64;
	if (size > 262144) size = 262144;

	parameters.bufferSize = size;
}

int QFtdiDevice::getBufferSize()
{
    // TODO
    return parameters.bufferSize;
}

void QFtdiDevice::setMaxTransferSize(int size)
{
    // TODO
    if (size < 64) size = 64;
	if (size > 262144) size = 262144;

	parameters.maxTransferSize = size;
}

int QFtdiDevice::getMaxTransferSize()
{
    // TODO
    return parameters.maxTransferSize;
}

void QFtdiDevice::setBufferNumber(int number) 
{
    // TODO
    if (number < 2) number = 2;
	if (number > 16) number = 16;

    parameters.nrBuffers = number;
}

int QFtdiDevice::getBufferNumber() 
{
    // TODO
    return parameters.nrBuffers;
}

void QFtdiDevice::setReadTimeout(int timeout) 
{
    // TODO
    parameters.rxTimeout = timeout;
}

int QFtdiDevice::getReadTimeout() 
{
    // TODO
    return parameters.rxTimeout;
}

bool QFtdiDevice::reset()
{
    // TODO
    return false;
}

void QFtdiDevice::jReadyRead(JNIEnv *env, jobject thiz , jlong jObjectIndex, jint jDeviceIndex)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    for(int i = 0; i < objectList.count(); i++)
    {
        if (!objectList.at(i).isNull()) {
            if (objectList.at(i)->objectIndex == jObjectIndex && objectList.at(i)->deviceIndex == jDeviceIndex)
            {
                emit objectList.at(i)->readyRead();
            }
        }
    }

//qDebug()<<"DBG:";
//qDebug()<<"DBG:"<<__func__<<"objectList.count() = "<< objectList.count();
//qDebug()<<"DBG:"<<__func__<<"deviceIndex ="<< jDeviceIndex;
//qDebug()<<"DBG:"<<__func__<<"objectIndex: ="<< jObjectIndex;
//qDebug()<<"DBG:";
}

QString QFtdiDevice::errorString()
{
    QString s = errorStringMsg;
    errorStringMsg = "";

    return s;
}

void QFtdiDevice::clearError()
{
    errorStringMsg.clear();
}

jbyteArray QFtdiDevice::QByteArray2jbyteArray(QByteArray buf)
{
    QAndroidJniEnvironment env;
    jbyteArray array = env->NewByteArray(buf.length());
    env->SetByteArrayRegion (array, 0, buf.length(), reinterpret_cast<jbyte*>(buf.data()));

    return array;
}

QByteArray QFtdiDevice::jbyteArray2QByteArray(jbyteArray buf)
{
    QAndroidJniEnvironment env;
    int len = env->GetArrayLength(buf);
    QByteArray array;
    array.resize(len);
    env->GetByteArrayRegion (buf, 0, len, reinterpret_cast<jbyte*>(array.data()));
    return array;
}

bool QFtdiDevice::deleteLocalRef(jobject jObject)
{
    QAndroidJniEnvironment jniEnv;
    jniEnv->DeleteLocalRef(jObject);
    return !exception("Java exception: bool Android::deleteLocalRef()");
}

bool QFtdiDevice::deleteGlobalRef(jobject jObject)
{
    QAndroidJniEnvironment jniEnv;
    jniEnv->DeleteGlobalRef(jObject);
    return !exception("Java exception: bool Android::deleteGlobalRef()");
}

bool QFtdiDevice::exception(QString debugMessage)
{
    QAndroidJniEnvironment jniEnv;
    if (jniEnv->ExceptionCheck()) {
        //jniEnv->ExceptionDescribe();
        jniEnv->ExceptionClear();
        if (!debugMessage.isEmpty())  qDebug(qPrintable(debugMessage));

        return true;
    }
    return false;
}


//create a vector with all JNINativeMethod(s)
static JNINativeMethod methods[] =
{
    {
        "onReadyRead", "(JI)V", (void *)QFtdiDevice::jReadyRead
    }
};

//automatically called on start
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    // get the JNIEnv pointer.
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }

    // search for Java class which declares the native methods
    jclass javaClass = env->FindClass("org/qtproject/qt5/JFtdiDevice");
    if (!javaClass)
    {
        return JNI_ERR;
    }

    // register native methods
    if (env->RegisterNatives(javaClass, methods, sizeof(methods) / sizeof(methods[0])) < 0)
    {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
