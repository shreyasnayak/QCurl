#include "txnjsfilemanager.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>
#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>

TxNjsFileManager::TxNjsFileManager(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager,&QNetworkAccessManager::finished,this,&TxNjsFileManager::onReplyFinished);
}

void TxNjsFileManager::uploadFile(QString filePath,QString tenant,QString bearerToken,QString uploadPath)
{
    QUrl url(uploadPath);
    QFile *file = new QFile(QUrl(filePath).toLocalFile());
    if(file->open(QIODevice::ReadOnly))
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart filePart;
        QNetworkRequest uploadUrl = QNetworkRequest(url);
        uploadUrl.setRawHeader("tenant",tenant.toUtf8());
        uploadUrl.setRawHeader("Authorization",QString("Bearer %1").arg(bearerToken).toUtf8());
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"file\"; filename=\"%1\"").arg(file->fileName()));
        filePart.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(filePart);
        m_networkManager->post(uploadUrl, multiPart);
    }
    else
    {
        qErrnoWarning("TxNjsFileManager : Failed to read file");
    }
}

void TxNjsFileManager::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        qInfo() << QString("No Error");
        qInfo() << QString("Body : %1 ").arg(QString(reply->readAll()));
        foreach (QByteArray key, reply->rawHeaderList()) {
           qInfo() << QString("Header :: %1 : %2 ").arg(key,QString(reply->rawHeader(key)));
        }
    }
    else
    {
        qErrnoWarning(QString("Upload failed, Error : %1").arg(reply->errorString()).toUtf8());
        qInfo() << QString("Body : %1 ").arg(QString(reply->readAll()));
        foreach (QByteArray key, reply->rawHeaderList()) {
           qInfo() << QString("Header :: %1 : %2 ").arg(key,QString(reply->rawHeader(key)));
        }
    }
    reply->deleteLater();
}
