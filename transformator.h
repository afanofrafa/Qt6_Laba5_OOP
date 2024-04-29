#ifndef TRANSFORMATOR_H
#define TRANSFORMATOR_H
#include <QByteArray>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QPointF>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
using namespace std;struct WeaponToSerialize {
    int number;
    int true_number;
    vector<pair<QString, int>> vect_int;
    vector<pair<QString, bool>> vect_bool;
    vector<pair<QString, QString>> vect_QString;
    vector<pair<QString, QPointF>> vect_QPointF;
    vector<QString> vect_info;
};
class Transformator
{
public:
    Transformator();
    static QByteArray fromXmlToJson(const QByteArray &byteArray);
    static QByteArray fromJsonToXml(const QByteArray &byteArray);
};
extern "C"  __declspec(dllexport) QByteArray fromXmlToJson(const QByteArray &byteArray);
extern "C"  __declspec(dllexport) QByteArray fromJsonToXml(const QByteArray &byteArray);
#endif // TRANSFORMATOR_H
