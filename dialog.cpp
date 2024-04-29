#include "dialog.h"
#include "ui_dialog.h"
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/target1.jpg"));
    count++;
    count %= 256;
    vect_dialogs.push_back(this);
    ui->radioButton_json->setChecked(true);
}
vector<Dialog*> Dialog::vect_dialogs;
void Dialog::setLabelText(const QString str)
{
    ui->label->setText(str);
}

void Dialog::setIsSaving(const bool isSaving)
{
    this->isSaving = isSaving;
    if (!isSaving)
        setFixedHeight(170);
}

void Dialog::set_weapon_vect(vector<wepData> *weapon_vect)
{
    this->weapon_vect = weapon_vect;
}

void Dialog::set_wepMap(map<int, pair<HINSTANCE, CreateInstance> > *wepMap)
{
    this->wepMap = wepMap;
}

void Dialog::set_soldier(Soldier *soldier)
{
    this->soldier = soldier;
}

void Dialog::set_ser_libs(vector<HINSTANCE> &ser_libs)
{
    this->ser_libs = ser_libs;
}

void Dialog::set_dataMainWindow(MainWindowToSerialize *dataMainWindow)
{
    this->dataMainWindow = dataMainWindow;
}

void Dialog::set_isDoing(bool *isDoing)
{
    this->isDoing = isDoing;
}

void Dialog::set_isClosing(bool *isClosing)
{
    this->isClosing = isClosing;
}

void Dialog::set_isEncryption(bool isEncryption)
{
    this->isEncryption = isEncryption;
}
uchar Dialog::count = 0;
uchar Dialog::get_count()
{
    return count;
}

void Dialog::dialogs_close()
{
    for (auto d : vect_dialogs)
        d->close();
}

QByteArray Dialog::cryptography_perform(QByteArray &byteArray, const char* str)
{
    for (int i = 0; i < ser_libs.size(); i++) {
        typedef void* (*CreateEncryption)();
        typedef void (*DestroyEncryption)(void* instance);
        typedef void (*CallSetKeyFunc)(void* instance, const QByteArray key);
        typedef QByteArray (*CallCryptTextFunc)(void* instance, QByteArray text);
        CreateEncryption createEncryption = (CreateEncryption)GetProcAddress(ser_libs.at(i), "createEncryptionInstance");
        CallSetKeyFunc callSetKeyFunc = (CallSetKeyFunc)GetProcAddress(ser_libs.at(i), "callSetKey");
        CallCryptTextFunc callCryptTextFunc = (CallCryptTextFunc)GetProcAddress(ser_libs.at(i), str);
        DestroyEncryption destroyEncryption = (DestroyEncryption)GetProcAddress(ser_libs.at(i), "destroyEncryptionInstance");
        if (createEncryption != NULL) {
            void *instance = createEncryption();
            if (callSetKeyFunc != NULL) {
                callSetKeyFunc(instance, "dc20d916c10800638d88d9eade9f3c8b21821ae78783ce8e3ece16d2ef9f69e2");
                if (callCryptTextFunc != NULL) {
                    byteArray = callCryptTextFunc(instance, byteArray);
                    if (destroyEncryption != NULL)
                        destroyEncryption(instance);
                }
            }
        }
    }
    return byteArray;
}

Dialog::~Dialog()
{
    vect_dialogs.pop_back();
    count--;
    delete ui;
}
vector<HINSTANCE> Dialog::ser_libs;
void Dialog::on_pushButton_yes_clicked()
{
    (*isDoing) = true;
    (*isClosing) = true;
    Data all_data;
    if (isSaving) {
#define GET_FUNCTION_AND_RESULT(type, name, arg) \
        typedef type (*get_##name)(void*); \
            get_##name get__##name = (get_##name)GetProcAddress(hDll, "get_" #name); \
            if (get__##name != NULL) { \
                type result_##name = get__##name(arg); \
                data.vect_##type.push_back(make_pair(QString(#name), result_##name)); \
        } \
            else \
            qDebug() << "get_"#name" method NULL DEFINE GET Dialog";
        for (int i = 0; i < weapon_vect->size(); i++) {
            HINSTANCE hDll = weapon_vect->at(i).hDll;
            WeaponToSerialize data;

            GET_FUNCTION_AND_RESULT(int, count, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, id, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, createNum, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, pixmapX, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, pixmapY, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, shotsNum, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, bullets_in_the_target, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, pistolCapacity, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, currentPistolCapacity, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, ammoType, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, shootSpeed, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, reloadSpeed, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, cleanSpeed, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, cutsNum, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, cuts_in_the_target, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, residual_cuts, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, bladeThick, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, bladeWidth, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, durability, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, slashNum, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, slashSpeed, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, stabSpeed, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(int, sharpSpeed, weapon_vect->at(i).weap)

            GET_FUNCTION_AND_RESULT(bool, sharp, weapon_vect->at(i).weap)

            GET_FUNCTION_AND_RESULT(QString, modelName, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, shootSound, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, reloadSound, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, cleanSound, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, material, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, stabSound, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, slashSound, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, sharpSound, weapon_vect->at(i).weap)
            GET_FUNCTION_AND_RESULT(QString, img, weapon_vect->at(i).weap)

            GET_FUNCTION_AND_RESULT(QPointF, widgetPos, weapon_vect->at(i).weap)

            typedef vector<QString> (*get_info)(void*);
            get_info get__info = (get_info)GetProcAddress(hDll, "get_info");
            if (get__info != NULL)
                data.vect_info = get__info(weapon_vect->at(i).weap);
            else
                qDebug() << "method NULL 68 Dialog";

            data.number = weapon_vect->at(i).number;
            data.true_number = weapon_vect->at(i).true_number;
            all_data.dataWeapon.push_back(data);
        }
        SoldierToSerialize dataSoldier;
        dataSoldier.isAttack = soldier->isAttack;
        dataSoldier.listAttackTexts = soldier->listAttackTexts;
        dataSoldier.listButtonTexts = soldier->listButtonTexts;
        dataSoldier.weaponMap = soldier->get_weaponMap();
        all_data.dataSoldier = dataSoldier;
        all_data.dataMainWindow = (*dataMainWindow);
        typedef QByteArray (*serialize_to_byte_array)(const Data &data);
        serialize_to_byte_array serialize = NULL;
        for (int i = 0; i < ser_libs.size(); i++) {
            if (ui->radioButton_xml->isChecked())
                serialize = (serialize_to_byte_array)GetProcAddress(ser_libs.at(i), "serialize_to_xml_byte_array");
            else
                serialize = (serialize_to_byte_array)GetProcAddress(ser_libs.at(i), "serialize_to_json_byte_array");
            if (serialize != NULL)
                break;
        }
        QByteArray byteArray;
        if (serialize != NULL)
            byteArray = serialize(all_data);

        if (byteArray.size() != 0 && isEncryption)
            byteArray = cryptography_perform(byteArray, "callEncryptText");

        QFile file("TextFile.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            file.write(byteArray);
        file.close();
    }
    else {
        (*dataMainWindow).remaining_time = -1;
        (*dataMainWindow).ChooseWeapon_curr_index = -1;
        (*dataMainWindow).WeaponList_curr_index = -1;
        (*dataMainWindow).target_choose_curr_index = -1;
        typedef Data (*deserialize_from_byte_array)(const QByteArray &byteArray);
        deserialize_from_byte_array deserialize = NULL;
        QByteArray byteArray;
        QFile file("TextFile.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            byteArray = file.readAll();
            QXmlStreamReader xml(byteArray);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
            bool b = false;
            while (!xml.atEnd() && !xml.hasError()) {
                xml.readNext();
                if (xml.hasError()) {
                    b = true;
                    break;
                }
            }
            if (byteArray.size() != 0 && (jsonDoc.isNull() && b))
                byteArray = cryptography_perform(byteArray, "callDecryptText");
        }
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
        QXmlStreamReader xml(byteArray);
        for (int i = 0; i < ser_libs.size(); i++) {
            if (!jsonDoc.isNull())
                deserialize = (deserialize_from_byte_array)GetProcAddress(ser_libs.at(i), "deserialize_from_json_byte_array");
            else
                deserialize = (deserialize_from_byte_array)GetProcAddress(ser_libs.at(i), "deserialize_from_xml_byte_array");
            if (deserialize != NULL)
                break;
        }
        if (deserialize != NULL) {
            all_data = deserialize(byteArray);
#define GET_FUNCTION_AND_SET_VALUE(type, name, arg1, arg2) \
            if (vect_int.at(j).first == #name) { \
                    typedef void (*set_##name)(void*, type); \
                    set_##name set__##name = (set_##name)GetProcAddress(wepD.hDll, "set_" #name); \
                    if (set__##name != NULL) \
                    set__##name(arg1, arg2); \
                    else \
                    qDebug() << "set_"#name" method NULL DEFINE SET Dialog"; \
            }
            soldier->listAttackTexts = all_data.dataSoldier.listAttackTexts;
            soldier->listButtonTexts = all_data.dataSoldier.listButtonTexts;
            soldier->isAttack = all_data.dataSoldier.isAttack;
            soldier->set_weaponMap(all_data.dataSoldier.weaponMap);

            (*dataMainWindow) = all_data.dataMainWindow;

            for (int i = 0; i < all_data.dataWeapon.size(); i++) {
                wepData wepD;
                wepD.number = all_data.dataWeapon.at(i).number;
                wepD.true_number = all_data.dataWeapon.at(i).true_number;
                auto iterator = wepMap->find(wepD.number);
                if (iterator != wepMap->end()) {
                    wepD.hDll = iterator->second.first;
                    auto weapon = iterator->second.second(wepD.true_number);
                    vector<pair<QString, int>> &vect_int = all_data.dataWeapon.at(i).vect_int;
                    vector<pair<QString, QString>> &vect_QString = all_data.dataWeapon.at(i).vect_QString;
                    vector<pair<QString, QPointF>> &vect_QPointF= all_data.dataWeapon.at(i).vect_QPointF;
                    vector<pair<QString, bool>> &vect_bool = all_data.dataWeapon.at(i).vect_bool;
                    for (int j = 0; j < vect_int.size(); j++) {
                        GET_FUNCTION_AND_SET_VALUE(int, count, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, id, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, createNum, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, pixmapX, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, pixmapY, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, shotsNum, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, bullets_in_the_target, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, pistolCapacity, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, currentPistolCapacity, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, ammoType, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, shootSpeed, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, reloadSpeed, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, cleanSpeed, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, cutsNum, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, cuts_in_the_target, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, residual_cuts, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, bladeThick, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, bladeWidth, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, durability, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, slashNum, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, slashSpeed, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, stabSpeed, weapon, vect_int.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(int, sharpSpeed, weapon, vect_int.at(j).second)
                    }
                    for (int j = 0; j < vect_QString.size(); j++) {
                        GET_FUNCTION_AND_SET_VALUE(QString, modelName, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, shootSound, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, reloadSound, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, cleanSound, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, material, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, stabSound, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, slashSound, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, sharpSound, weapon, vect_QString.at(j).second)
                        GET_FUNCTION_AND_SET_VALUE(QString, img, weapon, vect_QString.at(j).second)
                    }
                    for (int j = 0; j < vect_bool.size(); j++) {
                        GET_FUNCTION_AND_SET_VALUE(bool, sharp, weapon, vect_bool.at(j).second)
                    }
                    for (int j = 0; j < vect_QPointF.size(); j++) {
                        GET_FUNCTION_AND_SET_VALUE(QPointF, widgetPos, weapon, vect_QPointF.at(j).second)
                    }
                    typedef void (*set_info)(void*, vector<QString>);
                    set_info set__info = (set_info)GetProcAddress(wepD.hDll, "set_info");
                    if (set__info != NULL)
                        set__info(weapon, all_data.dataWeapon.at(i).vect_info);
                    else
                        qDebug() << "set_info method NULL DEFINE SET Dialog";
                    wepD.weap = weapon;
                    weapon_vect->push_back(wepD);
                }
            }
        }
    }
    close();
}


void Dialog::on_pushButton_no_clicked()
{
    (*isDoing) = false;
    (*isClosing) = true;
    close();
}


