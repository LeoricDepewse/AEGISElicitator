#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H
#include <QtNetwork/QtNetwork>
#include <QJsonDocument>
#include <vector>
#include <exception>
#include "reqtype.h"
#include "item.h"
#include "objectpayloads.h"
#include "additionalfunctions.h"

class DatabaseHandler
{
public:
    DatabaseHandler(std::string addr, std::string sessionID);
    //Listings
    std::vector<std::string> listDatabases();
    std::vector<std::string> listAssets();
    //Database
    bool createdatabase(std::string name);
    bool openDatabase(std::string name);
    bool deleteDatabase(std::string name);
    //Asset
    bool createAsset(Asset asset);
    std::string getCurrentDatabase() const;

private:
    QJsonDocument request(const REQTYPE method, const std::string url, const QByteArray payload);
    QJsonDocument get(const std::string url, const std::vector<Item>* payload);
    QJsonDocument post(const std::string url, const QJsonObject *payload);
    QJsonDocument replyCap(QNetworkReply *reply);
    std::string webaddr;
    std::string sessionID;
    std::string currentDatabase;
    QNetworkAccessManager nam;
};

#endif // DATABASEHANDLER_H
