#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QPointF>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
using namespace std;
struct WeaponToSerialize {
    int number;
    int true_number;
    vector<pair<QString, int>> vect_int;
    vector<pair<QString, bool>> vect_bool;
    vector<pair<QString, QString>> vect_QString;
    vector<pair<QString, QPointF>> vect_QPointF;
    vector<QString> vect_info;
};
struct SoldierToSerialize {
    bool isAttack;
    unordered_map<QString, pair<unordered_map<QString, QString>, unordered_map<QString, int>>> weaponMap;
    list<QString> listButtonTexts;
    list<QString> listAttackTexts;
};
struct MainWindowToSerialize
{
    bool action;
    bool isMouseRelease;
    int remaining_time;
    bool moveRight;
    bool moveDown;
    bool fl;
    QPointF widgetPos;
    int pixmapX;
    int pixmapY;
    int WeaponList_curr_index;
    int ChooseWeapon_curr_index;
    int target_choose_curr_index;
    int speed;
    int time;
    vector<QString> vect_info;
};
struct Data {
    MainWindowToSerialize dataMainWindow;
    vector<WeaponToSerialize> dataWeapon;
    SoldierToSerialize dataSoldier;
};
class Serializer
{
private:
    template<typename Type>
    static void toJson(QJsonObject &jsonObj, const vector<pair<QString, Type>> &vect);
public:
    Serializer();
    static QByteArray serialize_to_json_byte_array(const Data &data);
    static Data deserialize_from_json_byte_array(const QByteArray &byteArray);
    static QByteArray serialize_to_xml_byte_array(const Data &data);
    static Data deserialize_from_xml_byte_array(const QByteArray &byteArray);
    ~Serializer();
};
extern "C" __declspec(dllexport) QByteArray serialize_to_json_byte_array(const Data &data);
extern "C" __declspec(dllexport) Data deserialize_from_json_byte_array(const QByteArray &byteArray);
extern "C" __declspec(dllexport) QByteArray serialize_to_xml_byte_array(const Data &data);
extern "C" __declspec(dllexport) Data deserialize_from_xml_byte_array(const QByteArray &byteArray);
#endif // SERIALIZER_H
