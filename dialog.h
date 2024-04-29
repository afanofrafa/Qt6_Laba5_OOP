#ifndef DIALOG_H
#define DIALOG_H
#include "Static/visitor_drawvisitor.h"
#include <QDialog>
#include <QListWidget>
#include <QSpinBox>
#include <QJsonDocument>
#include <utility>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
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
typedef Weapon* (*CreateInstance)(int);
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void setLabelText(const QString str);
    void setIsSaving(const bool isSaving);
    void set_weapon_vect(vector<wepData> *weapon_vect);
    void set_wepMap(map<int, pair<HINSTANCE, CreateInstance>> *wepMap);
    void set_soldier(Soldier *soldier);
    void set_ser_libs(vector<HINSTANCE> &ser_libs);
    void set_dataMainWindow(MainWindowToSerialize *dataMainWindow);
    void set_isDoing(bool *isDoing);
    void set_isClosing(bool *isClosing);
    void set_isEncryption(bool isEncryption);
    static uchar get_count();
    static void dialogs_close();
    static QByteArray cryptography_perform(QByteArray &byteArray, const char* str);
    ~Dialog();

private slots:
    void on_pushButton_yes_clicked();

    void on_pushButton_no_clicked();

private:
    Soldier *soldier;
    bool isSaving;
    bool *isDoing;
    bool isEncryption;
    bool *isClosing;
    map<int, pair<HINSTANCE, CreateInstance>> *wepMap;
    vector<wepData> *weapon_vect;
    static vector<HINSTANCE> ser_libs;
    MainWindowToSerialize *dataMainWindow;
    static uchar count;
    static vector<Dialog*> vect_dialogs;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
