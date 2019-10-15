#include "QFtdiManager.h"
#include <QtAndroid>

QFtdiManager *QFtdiManager::ftdiManager;

QFtdiManager::QFtdiManager()
{
    this->androidActivity = QtAndroid::androidActivity();

    if (NULL == this->androidActivity)
    {
        errorStringMsg = "Java Exception: bool QFtdiManager::androidActivity is NULL!";
        return;
    }

    jFtdiManager =  QAndroidJniObject(QByteArray("org/qtproject/qt5/JFtdiManager").constData());
    if (Android::exception("Java Exception: bool QFtdiManager::jFtdiManager constructor"))
    {
        errorStringMsg = "Java Exception: bool QFtdiManager::jFtdiManager constructor";
    }

    if (NULL == jFtdiManager)
    {
        errorStringMsg = "Java Exception: bool QFtdiManager::jFtdiManager is NULL!";
        return;
    }

    jint result = jFtdiManager.callMethod<jint>(QByteArray("getInstance").constData(), QByteArray("(Landroid/content/Context;)I").constData(), androidActivity.object());

    if (0 > result) {}

    if (Android::exception("Java Exception: bool QFtdiManager::jFtdiManager.getInstance"))
    {
        errorStringMsg = "Java Exception: bool QFtdiManager::jFtdiManager.getInstance";
    }
}

QFtdiManager::~QFtdiManager() {}

QFtdiManager* QFtdiManager::getInstance()
{
    if (NULL == ftdiManager)
    {
        ftdiManager = new QFtdiManager();
    }
    return ftdiManager;
}

QStringList QFtdiManager::getDeviceCount()
{
    QStringList ports;

    if (!jFtdiManager.isValid())
    {
        errorStringMsg = "QFtdiManager::jFtdiManager is invalid!";
        return ports;
    }

    jint devCount = jFtdiManager.callMethod<jint>(QByteArray("getDeviceCount").constData());
	
    if (Android::exception("Java Exception: QFtdiManager::getDeviceCount()"))
    {
        errorStringMsg = "QFtdiManager::jFtdiManager is invalid!";
    }

    for(int i = 0; i < devCount; i++)
    {
        ports.append("ttyUSB" + QString::number(i));
    }

    return ports;
}

QString QFtdiManager::errorString()
{
    QString s = errorStringMsg;
    errorStringMsg = "";

    return s;
}

void QFtdiManager::clearError()
{
    errorStringMsg = "";
}
