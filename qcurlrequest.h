#ifndef QCURLREQUEST_H
#define QCURLREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class QCurlRequest : public QObject
{
    Q_OBJECT
public:
    enum EHTTP_METHOD
    {
        UNKNOW,
        GET,
        HEAD,
        POST,
    };

    struct Header
    {
        QString key;
        QString value;
    };

    explicit QCurlRequest(QObject *parent = nullptr);
    ~QCurlRequest();
    static EHTTP_METHOD methodStringToEnum(QString method);
    void head(const QUrl &url, const QList <Header> &headers);
    void get(const QUrl &url, const QList <Header> &headers);
    void post(const QUrl &url, const QByteArray &data, const QList <Header> &headers);
    void postfile(const QUrl &url,const QString &filePath, const QList <Header> &headers);

signals:
    void requestFinished(QByteArray response);
    void requestError(QString error);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);


private:
    QNetworkAccessManager *networkManager;
};

#endif // QCURLREQUEST_H
