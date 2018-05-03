#include "dataform.h"

DataForm::DataForm(QWidget *parent) : QFrame(parent)
{
    this->setLayout(new QVBoxLayout);
}

DataObject *DataForm::dataModel() const
{
    return _dataModel;
}

void DataForm::setDataModel(DataObject *dataModel)
{
    if(_dataModel != dataModel)
    {
        //if a creation form, delete object made from template.
        if(_dataModel != nullptr && _dataModel->ID() == -1)
            delete _dataModel;
        _dataModel = dataModel;
        //erase existing form
        _formFields.clear();
        QLayoutItem *child;
        while ((child = layout()->takeAt(0)) != 0)
        {
            delete child->widget();
            delete child;
        }
        if(_dataModel == nullptr)
            return;
        //create new form
#ifdef QT_DEBUG
        qDebug() << "Creating form...";
#endif
        for(int i = 0; i < _dataModel->size(); ++i)
        {
#ifdef QT_DEBUG
            qDebug() << "Creating field No. " + QString::fromStdString(std::to_string(i));
#endif
            if((*_dataModel)[i].isDummy)
            {
                _formFields.push_back(nullptr);
                continue;
            }
            DataProp item = (*_dataModel)[i];
            QLabel* label = new QLabel(QString::fromStdString(item.displayName));
            layout()->addWidget(label);
            QWidget* widget = nullptr;
            switch(item.type)
            {
            case text:
                widget = new QLineEdit(this);
                if((*_dataModel)[i].data != nullptr)
                    ((QLineEdit*)widget)->setText(QString::fromStdString(*(std::string*)(*_dataModel)[i].data));
                break;
            case largeText:
                widget = new QPlainTextEdit(this);
                if((*_dataModel)[i].data != nullptr)
                    ((QPlainTextEdit*)widget)->setPlainText(QString::fromStdString(*(std::string*)(*_dataModel)[i].data));
                break;
            case number:
                widget = new QSpinBox(this);
                break;
            case array:
                widget = new QLineEdit(this);
                break;
            case combo:
                widget = new QComboBox(this);
                {
                    const std::vector<std::string>* choices;
//                    if(typeid(item.data) != typeid(Cofor(int i = 0; i < _formFields.size(); ++i)mboChoice))
//                        throw "Combo declaration error";
                    const ComboChoice* combo = (ComboChoice*)item.data;
                    choices = combo->choices;
                    foreach(std::string str, *choices)
                        ((QComboBox*)widget)->addItem(QString::fromStdString(str));
//#ifdef QT_DEBUG
//                    qDebug() << "SPECIAL: field type [combo] using archetype SIGNIFICANCE: ";
//                    qDebug() << "SPECIAL: field type [combo] using archetype ASSETTYPE: ";
//#endif
                    ((QComboBox*)widget)->setCurrentIndex(combo->index);
                }
                break;
            case DataType::object://no known pattern, retained for compatability
            case nType:
                break;
            }
            if(widget == nullptr)
            {
                _formFields.push_back(nullptr);
                continue;
            }
            _formFields.push_back(widget);
            layout()->addWidget(widget);
        }
        _submitButton = new QPushButton("Submit", this);
        layout()->addWidget(_submitButton);
        connect(_submitButton, SIGNAL(released()), this, SLOT(submit()));
    }
}

void DataForm::submit()
{
    for(int i = 0; i < _dataModel->size(); ++i)
    {
        if((*_dataModel)[i].isDummy)
            continue;
        switch((*_dataModel)[i].type)
        {
        case text:
            (*_dataModel)[i].data = new std::string(((QLineEdit*)_formFields[i])->text().toStdString());
            break;
        case largeText:
            (*_dataModel)[i].data = new std::string(((QPlainTextEdit*)_formFields[i])->toPlainText().toStdString());
            break;
        case number:
            (*_dataModel)[i].data = new Integer(((QSpinBox*)_formFields[i])->value());
            break;
        case array:
            //TODO
            {
                std::string tagString = ((QLineEdit*)_formFields[i])->text().toStdString();
                std::vector<std::string> tags = split(tagString, ',');
                (*_dataModel)[i].data = new DataProp[tags.size()];
                std::vector<DataProp>* arr = new std::vector<DataProp>();
                for(int j = 0; j < tags.size(); ++j)
                {
                    trim(tags[j]);
                    arr->push_back({"", "", text, false, new std::string(tags[j])});
                }
                (*_dataModel)[i].data = arr;
            }
            break;
        case combo:
            ((ComboChoice*)(*_dataModel)[i].data)->index = ((QComboBox*)_formFields[i])->currentIndex();
        case object:
        case nType:
            //NONE
            break;
        }
    }
    on_submit(_dataModel->buildJson());
}
