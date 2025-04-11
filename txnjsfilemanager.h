#ifndef TXNJSFILEMANAGER_H
#define TXNJSFILEMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class TxNjsFileManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager* m_networkManager;
public:
    explicit TxNjsFileManager(QObject *parent = nullptr);
    void uploadFile(QString filePath, QString tenant, QString bearerToken, QString uploadPath);
private slots:
    void onReplyFinished(QNetworkReply *reply);
};

#endif // TXNJSFILEMANAGER_H
