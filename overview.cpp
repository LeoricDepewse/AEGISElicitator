#include "overview.h"
#include "ui_overview.h"
#include "dynamiclinkdialog.h"

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
#ifdef QT_DEBUG
    handler = new DatabaseHandler("localhost:7071", "test");
    handler->openDatabase("default");
#else
    //establish connection
    bool connected = false;
    while(!connected)
    {
        QString addr = QInputDialog::getText(this, "Connect to CAIRIS", "CAIRIS Address:", QLineEdit::Normal, "");
        handler = new DatabaseHandler(addr.toStdString(), "test");
        connected = handler->openDatabase("default");
        if(!connected) delete handler;
    }
#endif
    ui->setupUi(this);
    createForm = new DataForm();
    ui->verticalFrame->layout()->addWidget(createForm);
    ui->verticalFrame->layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    connect(createForm, SIGNAL(on_submit(QJsonObject)), this, SLOT(createObject(QJsonObject)));
}

Overview::~Overview()
{
    delete ui;
    delete handler;
    delete createForm;
}

void Overview::on_actionDynamic_Linker_triggered()
{
    //Create DynLink Window
    auto window = new DynamicLinkDialog();
    window->show();
}

void Overview::on_actionNew_Database_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Database", "New Database name:", QLineEdit::Normal, "", &ok);
    if(!ok || name.isEmpty())
        return;
    QMessageBox msgBox;
    if(handler->createdatabase(name.toStdString()))
    {
        msgBox.setText("Database Created.");
        databases.append(name);
    }
    else
        msgBox.setText("Database failed to create!");
    msgBox.exec();
}

void Overview::on_actionOpen_Database_triggered()
{
    if(!dbsListed)
    {
        QStringList list;
        list.append("");//initial blank
        std::vector<std::string> dbs = handler->listDatabases();
        for(auto it = dbs.begin(); it != dbs.end(); ++it)
            list.append(QString::fromStdString(*it));
        list.append(QString::fromStdString(handler->getCurrentDatabase()));
        databases = list;
        dbsListed = true;
    }
    bool ok;
    QString name = QInputDialog::getItem(this, "Select database", "Database:", databases, 0, false, &ok);
    if(!ok || name.isEmpty() || name == QString::fromStdString(handler->getCurrentDatabase()))
        return;
    QMessageBox msgBox;
    if(handler->openDatabase(name.toStdString()))
        msgBox.setText("Database opened.");
    else
        msgBox.setText("Database failed to open!");
    msgBox.exec();
}

void Overview::on_typeCombo_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Asset")
        createForm->setDataModel(new DataObject(-1, ASSET, ASSET_C));
    else
        createForm->setDataModel(nullptr);
}

void Overview::createObject(QJsonObject object)
{
    if(ui->typeCombo->currentText() == "Asset")
        handler->createAsset(object);
}

//void Overview::on_typeCombo_currentIndexChanged(const QString &arg1)
//{
//    //change creation context
//    QLayoutItem *child;
//    while ((child = ui->itemFrame->layout()->takeAt(0)) != 0)
//    {
//        delete child->widget();
//        delete child;
//    }
//    formcache.clear();
//    ui->itemFrame->layout()->addWidget(new QLabel("Name:"));
//    QLineEdit* lineedit = new QLineEdit(); lineedit->setObjectName("name");
//    ui->itemFrame->layout()->addWidget(lineedit); formcache.push_back(lineedit);
//    if(arg1 == "Asset")
//    {
//        ui->itemFrame->layout()->addWidget(new QLabel("Short Code:"));
//        lineedit = new QLineEdit(); lineedit->setObjectName("shortCode");
//        ui->itemFrame->layout()->addWidget(lineedit); formcache.push_back(lineedit);
//        ui->itemFrame->layout()->addWidget(new QLabel("Description:"));
//        QPlainTextEdit* block = new QPlainTextEdit(); block->setObjectName("assetDescription");
//        ui->itemFrame->layout()->addWidget(block); formcache.push_back(block);
//        ui->itemFrame->layout()->addWidget(new QLabel("significance:"));
//        QComboBox* combo = new QComboBox(); combo->setObjectName("assetSig");
//        foreach(std::string sig, SIGNIFICANCE)
//            combo->addItem(QString::fromStdString(sig));
//        ui->itemFrame->layout()->addWidget(combo); formcache.push_back(combo);
//        ui->itemFrame->layout()->addWidget(new QLabel("asset Type:"));
//        combo = new QComboBox(); combo->setObjectName("assetType");
//        foreach(std::string type, gAssetType)
//            combo->addItem(QString::fromStdString(type));
//        ui->itemFrame->layout()->addWidget(combo); formcache.push_back(combo);
//        ui->itemFrame->layout()->addWidget(new QLabel("tags:"));
//        lineedit = new QLineEdit(); lineedit->setObjectName("tags");
//        ui->itemFrame->layout()->addWidget(lineedit); formcache.push_back(lineedit);
//    }
//    else//none
//        while ((child = ui->itemFrame->layout()->takeAt(0)) != 0)
//        {
//            delete child->widget();
//            delete child;
//        }
//}

//void Overview::on_commitNew_clicked()
//{
//    //creation package
//    if(ui->typeCombo->currentText() == "Asset")
//    {
//        Asset asset;
//        asset.assetId = nextAssetId;
//        asset.assetName = ((QLineEdit*)formcache[0])->text().toStdString();
//        asset.shortCode = ((QLineEdit*)formcache[1])->text().toStdString();
//        asset.assetDescription = ((QPlainTextEdit*)formcache[2])->toPlainText().toStdString();
//        //asset.assetSig = &(SIGNIFICANCE[((QComboBox*)formcache[3])->currentIndex()]);
//        asset.assetType = &(gAssetType[((QComboBox*)formcache[4])->currentIndex()]);
//        asset.tags = ((QLineEdit*)formcache[5])->text().toStdString();
//#ifdef QT_DEBUG
//        qDebug() << QString::fromStdString(asset.assetId + ", " + asset.assetName + ", " + asset.shortCode+ ", "
//                                           + asset.assetDescription + ", " + *asset.assetSig + ", "
//                                           + *asset.assetType + ", " + asset.tags);
//#endif
//        if(handler->createAsset(asset))
//            return;//confirm message
//    }
//}
