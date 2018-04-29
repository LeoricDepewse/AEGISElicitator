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
    DatabaseHandler(std::string addr, std::string _sessionID);
    //Listings
    std::vector<std::string> listDatabases();
    std::vector<std::string> listAssets();
    std::string getCurrentDatabase() const;
    //Database
    bool createdatabase(std::string name);
    bool openDatabase(std::string name);
    bool deleteDatabase(std::string name);
    //Asset
    int createAsset(QJsonObject asset);
    std::vector<DataObject> getAssets();

private:
    QJsonDocument request(const REQTYPE method, const std::string url, const QByteArray payload);
    QJsonDocument get(const std::string url, const std::vector<Item>* payload);
    QJsonDocument post(const std::string url, const QJsonObject *payload);
    QJsonDocument replyCap(QNetworkReply *reply);
    std::vector<DataObject> _assets; bool _assetSync = false;
    std::string _webaddr;
    std::string _sessionID;
    std::string _currentDatabase;
    QNetworkAccessManager _nam;
};

#endif // DATABASEHANDLER_H
