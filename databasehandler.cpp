#include "databasehandler.h"

DatabaseHandler::DatabaseHandler(std::string addr, std::string sessionID)
{
    webaddr = addr;
#ifdef QT_DEBUG
    this->sessionID = "test";
#else
    this->sessionID = sessionID;
#endif
}

//LISTINGS INTERFACE
std::vector<std::string> DatabaseHandler::listDatabases()
{
    std::string url = webaddr + "/api/settings/databases";
#ifdef QT_DEBUG
    qDebug() << "QUERY(GET): Asking for list of databases...";
    qDebug() << "GET: " << QString::fromStdString(url);
#endif
    QJsonDocument Json = get(url, nullptr);
    if(!Json.isArray())
    {
        qCritical() << "Bad responce: not of type [array].";
        throw QString("Failed to fetch databases.");
    }
#ifdef QT_DEBUG
    else
        qDebug() << "OK.";
#endif
    QJsonArray JsonArray = Json.array();
    std::vector<std::string> databases;
    for(auto it = JsonArray.begin(); it != JsonArray.end(); ++it)
        databases.push_back(it->toString().toStdString());
    return databases;
}

std::vector<std::string> DatabaseHandler::listAssets()
{
    std::string url = webaddr + "/api/assets";
#ifdef QT_DEBUG
    qDebug() << "QUERY(GET): Asking for list of assets...";
    qDebug() << "GET: " << QString::fromStdString(url);
#endif
    QJsonDocument Json = get(url, nullptr);
    if(!Json.isObject())
    {
        qCritical() << "Bad responce: not of type [dict].";
        throw QString("Failed to fetch assets.");
    }
#ifdef QT_DEBUG
    else
        qDebug() << "OK.";
#endif
    //TBD
}

//DATABASE INTERFACE
bool DatabaseHandler::createdatabase(std::string name)
{
    std::string url = webaddr + "/api/settings/database/" + name + "/create";
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting creation of database " + QString::fromStdString(name);
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, nullptr);
    if(reply.object().value("message").toString() == "New database successfully created")
        return true;
    return false;
}

bool DatabaseHandler::openDatabase(std::string name)
{
    std::string url = webaddr + "/api/settings/database/" + name + "/open";
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting opening of database " + QString::fromStdString(name);
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, nullptr);
    if(reply.object().value("message").toString() == "Database successfully opened")
        return true;
    return false;
}

bool DatabaseHandler::deleteDatabase(std::string name)
{
    std::string url = webaddr + "/api/settings/database/" + name + "/delete";
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting deletion of database " + QString::fromStdString(name);
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, nullptr);
    if(reply.object().value("message").toString() == "Database successfully deleted")
        return true;
    return false;
}

//ASSET INTERFACE
bool DatabaseHandler::createAsset(Asset asset)
{
    std::string url = webaddr + "/api/assets";
    QJsonObject object;
    object.insert("assetId", QJsonValue(asset.assetId));
    object.insert("assetName", QJsonValue(asset.assetName.c_str()));
    object.insert("shortCode", QJsonValue(asset.shortCode.c_str()));
    object.insert("assetDescription", QJsonValue(asset.assetDescription.c_str()));
    object.insert("assetSig", QJsonValue(asset.assetSig->c_str()));
    object.insert("assetType", QJsonValue(asset.assetType->c_str()));
    object.insert("cFlag", QJsonValue(asset.cFlag));
    object.insert("tags", QJsonValue(asset.tags.c_str()));
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting creation of asset " + QString::fromStdString(asset.assetName);
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, &object);
    //TBD confirmation
}

//Network functions
QJsonDocument DatabaseHandler::get(const std::string url, const std::vector<Item> *payload)
{
    QNetworkReply* reply;
    std::string req = url + "&session_id=" + sessionID;
    if(payload != nullptr)
        for(auto it = payload->begin(); it != payload->end(); ++it)
            req += "&" + it->key + "=" + it->value;
    reply = nam.get(QNetworkRequest(QUrl(QString::fromStdString(req))));
    return replyCap(reply);
}

QJsonDocument DatabaseHandler::post(const std::string url, const QJsonObject* payload)
{
    QNetworkReply* reply;
    std::string req;
    QJsonObject container;
    if(payload == nullptr)
        req = url + "&session_id=" + sessionID;
    else
    {
        container.insert("session_id", QJsonValue(sessionID.c_str()));
        container.insert("object", QJsonValue(*payload));
    }
    QJsonDocument doc(container);
    reply = nam.post(QNetworkRequest(QUrl(QString::fromStdString(url))), doc.toJson());
    return replyCap(reply);
}

QJsonDocument DatabaseHandler::replyCap(QNetworkReply* reply)
{
    while(!reply->isFinished())
        qApp->processEvents();
    reply->deleteLater();
#ifdef QT_DEBUG
    qDebug() << "RESPONCE:" << endl << QString::fromStdString(reply->readAll().toStdString());
#endif
    return QJsonDocument::fromJson(reply->readAll());
}
