#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <qcurlrequest.h>

#include <txnjsfilemanager.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationVersion("1.0.0");
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("Description: ");
    QCommandLineOption header({"H","header"}, QCoreApplication::translate("main", "HTTP Header"),"HEADER-KEY: VALUE");
    QCommandLineOption method({"X","method"}, QCoreApplication::translate("main", "HTTP Method"),"METHOD");
    QCommandLineOption url({"U","url"}, QCoreApplication::translate("main", "HTTP/HTTPS URL"),"URL");
    QCommandLineOption body({"B","body"}, QCoreApplication::translate("main", "Request Body"),"Body");
    QCommandLineOption form ({"F","form"}, QCoreApplication::translate("main", "Form body"),"Form body");
    parser.addOption(header);
    parser.addOption(method);
    parser.addOption(url);
    parser.addOption(body);
    parser.addOption(form);
    parser.process(a);

    QCurlRequest qcurl;

    // Lambda function to handle the requestFinished signal
    QObject::connect(&qcurl, &QCurlRequest::requestFinished, [&a](const QByteArray &response) {
        qDebug() << "Response received : " << response;
        a.exit(0);
    });

    // Lambda function to handle the requestError signal
    QObject::connect(&qcurl, &QCurlRequest::requestError, [&a](const QString &error) {
        qDebug() << "Error : " << error;
        a.exit(0);
    });

    const QCurlRequest::EHTTP_METHOD methodValue = QCurlRequest::methodStringToEnum(parser.value(method));
    const QString urlValue = parser.value(url);
    const QByteArray bodyValue = parser.value(body).toUtf8();

    QList<QCurlRequest::Header> headerList;
    if(parser.isSet(header))
    {
       const QStringList headers = parser.values(header);
       foreach (QString headerKeyValue, headers)
       {
           QStringList keyValue = headerKeyValue.split(":");
           if(keyValue.count()>0)
           {
               QCurlRequest::Header header;
               header.key = keyValue[0];
               header.value = keyValue[1];
               headerList.append(header);
               qInfo() << "Header Set" << header.key << ":" << header.value;
           }
       }
    }

    switch (methodValue)
    {
        case QCurlRequest::EHTTP_METHOD::UNKNOW:
            qInfo() << "Error : Unknown method";
            return 0;
        break;
        case QCurlRequest::EHTTP_METHOD::HEAD:
            qInfo() << "Sending Head request";
            qcurl.head(urlValue,headerList);
        break;
        case QCurlRequest::EHTTP_METHOD::GET:
            qInfo() << "Sending Get request";
            qcurl.get(urlValue,headerList);
        break;
        case QCurlRequest::EHTTP_METHOD::POST:
            if(parser.isSet(form))
            {
                if(parser.isSet(form))
                {
                   const QString formValue = parser.value(form);
                   const QStringList keyValue = formValue.split("@");
                   qcurl.postfile(urlValue,keyValue[1],headerList);
                }
                else
                {
                    qInfo() << "Failed to send form post request";
                }
            }
            else
            {
                qInfo() << "Sending Post request";
                qcurl.post(urlValue,bodyValue,headerList);
            }
        break;
    }

    return a.exec();
}
