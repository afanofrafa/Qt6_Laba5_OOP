#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <map>
#include <QMainWindow>
#include <windows.h>
#include <QScreen>
#include <QSize>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>
#include <QPushButton>
#include <QEvent>
#include <QByteArray>
#include <QMouseEvent>
#include <QToolButton>
#include <QComboBox>
#include "dialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Soldier soldier;
    QPixmap pixmap;
    void setPixmapPosition(int x, int y);
    void setPixmap(const QPixmap &newPixmap);
    void setLibraries(const vector<HINSTANCE> &libs, const vector<HINSTANCE> &librs, const vector<HINSTANCE> &ser_libs);
    vector<HINSTANCE> getLibraries();
    void Recovering();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

    void doAfterAction();

    void doAction();

    void on_pushButton_Buy_clicked();

    void buying_weapon(int number);

    void on_pushButton_Throw_clicked();

    void on_comboBox_WeaponList_currentIndexChanged(int index);

    void on_pushButton_start_clicked();

    void drawTarget();

    void stopShooting();

    bool checkActionAttack();

    void accepting(QString action, int index);

    void onButtonClicked();

    void buttonsClear();

    void SetButtonsEnabled();

    void SetButtonsVisible();

    void allow_attack();

    void start_animation(int x, int y, bool bRight, bool bDown, int remaining_time);

    void on_pushButton_close_clicked();

    void on_pushButton_transformator_clicked();

private:
    Ui::MainWindow *ui;
    int const iconSize = 150;
    map<QToolButton*, QString> buttonsImgs;
    map<int, QString> targets;
    vector<QToolButton*> buttons;
    vector<HINSTANCE> libraries;
    vector<HINSTANCE> librs;
    vector<HINSTANCE> ser_libs;
    typedef Weapon* (*CreateInstance)(int);
    map<int, pair<HINSTANCE, CreateInstance>> wepMap;
    vector<wepData> weapon_vect;
    QComboBox *comboBox;
};
#endif // MAINWINDOW_H
