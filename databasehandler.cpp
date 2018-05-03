#include "databasehandler.h"

DatabaseHandler::DatabaseHandler(std::string addr, std::string sessionID)
{
    _webaddr = addr;
#ifdef QT_DEBUG
    this->_sessionID = "test";
#else
    this->_sessionID = sessionID;
#endif
}

//LISTINGS INTERFACE
std::vector<std::string> DatabaseHandler::listDatabases()
{
    std::string url = _webaddr + "/api/settings/databases";
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
    std::vector<DataObject> assets = *(this->getAssets());
    std::vector<std::string> names;
    foreach(DataObject asset, assets)
        names.push_back(*(std::string*)(asset[0].data));
    return names;
//    std::string url = _webaddr + "/api/assets";
//#ifdef QT_DEBUG
//    qDebug() << "QUERY(GET): Asking for list of assets...";
//    qDebug() << "GET: " << QString::fromStdString(url);
//#endif
//    QJsonDocument Json = get(url, nullptr);
//    if(!Json.isObject())
//    {
//        qCritical() << "Bad responce: not of type [dict].";
//        throw QString("Failed to fetch assets.");
//    }
//    auto dict = Json.object().keys();
//    std::vector<std::string> names;
//    foreach(QString name, dict)
//        names.push_back(name.toStdString());
//#ifdef QT_DEBUG
//    else qDebug() << "OK.";
//#endif
//    return names;
}

std::string DatabaseHandler::getCurrentDatabase() const
{
    return _currentDatabase;
}

//DATABASE INTERFACE
bool DatabaseHandler::createdatabase(std::string name)
{
    std::string url = _webaddr + "/api/settings/database/" + name + "/create";
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting creation of database " + QString::fromStdString(name);
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, nullptr);
    if(reply.object().value("message").toString() == "New database successfully created")
    {
        _currentDatabase = name;
        return true;
    }
    return false;
}

bool DatabaseHandler::openDatabase(std::string name)
{
    std::string url = _webaddr + "/api/settings/database/" + name + "/open";
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting opening of database " + QString::fromStdString(name);
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, nullptr);
    if(reply.object().value("message").toString() == "Database successfully opened")
    {
        _currentDatabase = name;
        return true;
    }
    return false;
}

bool DatabaseHandler::deleteDatabase(std::string name)
{
    std::string url = _webaddr + "/api/settings/database/" + name + "/delete";
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
int DatabaseHandler::createAsset(QJsonObject asset)
{
    std::string url = _webaddr + "/api/assets";
#ifdef QT_DEBUG
    qDebug() << "QUERY(POST): Requesting creation of asset ";
    qDebug() << "POST: " << QString::fromStdString(url);
#endif
    QJsonDocument reply = post(url, &asset);
    _assetSync = false;
    if(reply.object().contains("asset_id"))
        return reply.object().value("asset_id").toInt();
    else return -1;
}

std::vector<DataObject> *DatabaseHandler::getAssets()
{
    if(!_assetSync)
    {
        //populate asset list
        std::string url = _webaddr + "/api/assets";
#ifdef QT_DEBUG
        qDebug() << "QUERY(GET): Requesting list of assets";
        qDebug() << "GET: " << QString::fromStdString(url);
#endif
        QJsonDocument reply = get(url, nullptr);
        if(!reply.isObject())
        {
            qCritical() << "Bad responce: not of type [dict].";
            throw QString("Failed to fetch assets.");
        }
        _assets.clear();
        auto assetJson = reply.object();
        foreach(QJsonValue item, assetJson)
        {
            auto asset = DataObject(_assets.size(), ASSET, ASSET_C);
            for(int j = 0; j < asset.size(); ++j)
            {
                if(!item.isObject()) throw QString("Invalid asset in list!");
                QJsonObject jsonObject = item.toObject();
                switch(asset[j].type)
                {
                case text:
                case largeText:
                    if(!jsonObject.value(QString::fromStdString(asset[j].name)).isString()) throw QString("DataType mismatch!");
                    asset[j].data = new std::string(jsonObject.value(QString::fromStdString(asset[j].name)).toString().toStdString());
                    break;
                case number:
                    if(!jsonObject.value(QString::fromStdString(asset[j].name)).isDouble()) throw QString("DataType mismatch!");
                    asset[j].data = new Integer(jsonObject.value(QString::fromStdString(asset[j].name)).toInt());
                    break;
                case array:
                    if(!jsonObject.value(QString::fromStdString(asset[j].name)).isArray()) throw QString("DataType mismatch!");
                    {
                        auto jsonArray = jsonObject.value(QString::fromStdString(asset[j].name)).toArray();
                        std::vector<DataProp> *arr = new std::vector<DataProp>();
                        foreach(QJsonValue val, jsonArray)
                        {
                            if(val.isString()) arr->push_back({"", "", text, false, new std::string(val.toString().toStdString())});
                            else if(val.isDouble()) arr->push_back({"", "", number, false, new Integer(val.toInt())});
                            else if(val.isNull()) arr->push_back({"", "", nType});
                            else throw QString("structure exceeds nesting limit!");
                        }
                        asset[j].data = arr;
                    }
                    break;
                case combo:
                    if(!jsonObject.value(QString::fromStdString(asset[j].name)).isString()) throw QString("DataType mismatch!");
                    {
                        std::string value = jsonObject.value(QString::fromStdString(asset[j].name)).toString().toStdString();
                        ComboChoice* choice = (ComboChoice*)asset[j].data;
//                        switch(*(Integer*)asset[j].data)
//                        {
//                        case T_SIGNIFICANCE:
//                            choice->choices = &SIGNIFICANCE;
//                            break;
//                        case T_ASSETTYPE:
//                            choice->choices = &gAssetType;
//                            break;
//                        default:
//                            throw "Invalid choices configuration";
//                        }
                        choice->index = -1;
                        for(uint i = 0; i < choice->choices->size(); ++i)
                            if(value.compare(choice->choices->at(i)) == 0) {choice->index = i; break;}
                        if(choice->index == -1) throw "invalid combo choice!";
//                        asset[j].data = choice;
                    }
                    break;
                case object:
                case nType:
                    //NONE
                    break;
                }
            }
            _assets.push_back(asset);
        }
        _assetSync = true;
    }
    return &_assets;
}

//Network functions
QJsonDocument DatabaseHandler::get(const std::string url, const std::vector<Item> *payload)
{
    QNetworkReply* reply;
    std::string req = "http://" + url + "?session_id=" + _sessionID;
    if(payload != nullptr)
        for(auto it = payload->begin(); it != payload->end(); ++it)
            req += "&" + it->key + "=" + it->value;
    reply = _nam.get(QNetworkRequest(QUrl(QString::fromStdString(req))));
    return replyCap(reply);
}

QJsonDocument DatabaseHandler::post(const std::string url, const QJsonObject* payload)
{
    QNetworkReply* reply;
    QNetworkRequest request;
    std::string req = "http://" + url;
    QJsonObject container;
    if(payload == nullptr)
    {
        req += "?session_id=" + _sessionID;
        request = QNetworkRequest(QUrl(QString::fromStdString(req)));
    }
    else
    {
        container.insert("session_id", QJsonValue(_sessionID.c_str()));
        container.insert("object", QJsonValue(*payload));
        request = QNetworkRequest(QUrl(QString::fromStdString(req)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    }
    QJsonDocument doc(container);
    reply = _nam.post(request, doc.toJson());
    return replyCap(reply);
}

QJsonDocument DatabaseHandler::replyCap(QNetworkReply* reply)
{
    while(!reply->isFinished())
        qApp->processEvents();
    QByteArray data = reply->readAll();
#ifdef QT_DEBUG
    qDebug() << "RESPONCE:" << endl << QString::fromStdString(data.toStdString());
#endif
    reply->deleteLater();
    return QJsonDocument::fromJson(data);
}
