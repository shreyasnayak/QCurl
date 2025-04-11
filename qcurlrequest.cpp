#include "qcurlrequest.h"
#include <QHttpMultiPart>
#include <QFile>

QCurlRequest::QCurlRequest(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &QCurlRequest::onReplyFinished);
    connect(networkManager, &QNetworkAccessManager::sslErrors, this, &QCurlRequest::onSslErrors);
}

QCurlRequest::~QCurlRequest()
{
    delete networkManager;
}

QCurlRequest::EHTTP_METHOD QCurlRequest::methodStringToEnum(QString method)
{
    if(method=="GET")
    {
        return QCurlRequest::EHTTP_METHOD::GET;
    }
    else if(method=="POST")
    {
        return QCurlRequest::EHTTP_METHOD::POST;
    }
    else if(method=="HEAD")
    {
        return QCurlRequest::EHTTP_METHOD::HEAD;
    }
    else
    {
        return QCurlRequest::EHTTP_METHOD::UNKNOW;
    }
}

void QCurlRequest::head(const QUrl &url, const QList<QCurlRequest::Header> &headers)
{
    QNetworkRequest request(url);
    foreach(QCurlRequest::Header header, headers) {
        request.setRawHeader(header.key.toUtf8(),header.value.toUtf8());
    }
    networkManager->head(request);
}

void QCurlRequest::get(const QUrl &url, const QList<QCurlRequest::Header> &headers)
{
    QNetworkRequest request(url);
    foreach(QCurlRequest::Header header, headers) {
        request.setRawHeader(header.key.toUtf8(),header.value.toUtf8());
    }
    networkManager->get(request);
}

void QCurlRequest::post(const QUrl &url, const QByteArray &data, const QList<QCurlRequest::Header> &headers)
{
    QNetworkRequest request(url);
    foreach(QCurlRequest::Header header, headers) {
        request.setRawHeader(header.key.toUtf8(),header.value.toUtf8());
    }
    networkManager->post(request, data);
}

void QCurlRequest::postfile(const QUrl &url, const QString &filePath, const QList<QCurlRequest::Header> &headers)
{
    qInfo() << filePath;
    QFile *file = new QFile(filePath);
    if(file->open(QIODevice::ReadOnly))
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QNetworkRequest uploadUrl = QNetworkRequest(url);
        foreach(QCurlRequest::Header header, headers) {
           uploadUrl.setRawHeader(header.key.toUtf8(),header.value.toUtf8());
        }
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"file\"; filename=\"%1\"").arg(file->fileName()));
        filePart.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(filePart);
        networkManager->post(uploadUrl, multiPart);
    }
    else
    {
        emit requestError("Failed to read file");
    }
}

void QCurlRequest::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        emit requestFinished(reply->readAll());
    } else {
        emit requestError(reply->errorString());
    }
    reply->deleteLater();
}

void QCurlRequest::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    foreach (QSslError err, errors)
    {
       qInfo() << "SSL Error : " << err.errorString();
    }
    reply->ignoreSslErrors(errors);
}

