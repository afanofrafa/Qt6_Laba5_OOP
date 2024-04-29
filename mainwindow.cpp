#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QTimer>
#include <QFile>
Ui::MainWindow *u;
bool action;
bool isMouseRelease;
QTimer *timerA;
QTimer *timerB;
QTimer *timerZ;
QTimer *timer_notAttack;
QTimer *timerStart;
QTimer *timer_time;
bool moveRight = true;
bool moveDown = true;
bool fl;
QString button_action;
QPointF widgetPos;
int pixmapX = 0;
int pixmapY = 0;
int remaining_time = 0;

void MainWindow::SetButtonsEnabled() {
    if (ui->listWidget_Info->isEnabled() != fl) {
        ui->listWidget_Info->setEnabled(fl);
        for (int i = 0; i < buttons.size(); i++)
            buttons[i]->setEnabled(fl);
    }
    if (timerZ->isActive())
        timerZ->stop();
}
void MainWindow::SetButtonsVisible() {
    if (ui->listWidget_Info->isEnabled() != fl) {
        ui->listWidget_Info->setVisible(fl);
        ui->label_Info->setVisible(fl);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white); // Заливаем фон виджета белым цветом
    painter.drawPixmap(pixmapX, pixmapY, pixmap); // Рисуем pixmap по заданным координатам
    painter.drawRect(QRect(280, 10, 930, 700));
}

void MainWindow::closeEvent(QCloseEvent *event) {
    typedef void (*DestroyInstance)(Weapon*);
    for (int i = 0; i < weapon_vect.size(); i++) {
        DestroyInstance destroyInstance = (DestroyInstance)GetProcAddress(weapon_vect.at(i).hDll, "destroyInstance");
        if (destroyInstance != NULL)
            destroyInstance(weapon_vect.at(i).weap);
        else
            qDebug() << "mainwindow 112 method NULL";
    }
    for (int i = 0; i < libraries.size(); i++)
        FreeLibrary(libraries[i]);
    delete timerA;
    delete timerB;
    delete timerZ;
    delete timerStart;
    delete timer_notAttack;
    delete timer_time;
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), soldier(&soldier)
{
    ui->setupUi(this);
    u = ui;
    pixmap = QPixmap();
    QFile file("TextFile.txt");
    file.open(QIODevice::ReadOnly);
    int y = file.size();
    file.close();
    update();
    timerA = new QTimer(this);
    timerB = new QTimer(this);
    timerZ = new QTimer(this);
    timer_time = new QTimer(this);
    timerStart = new QTimer(this);
    timer_notAttack = new QTimer(this);

    comboBox = nullptr;

    connect(timerA, SIGNAL(timeout()), this, SLOT(doAction()));
    connect(timerStart, SIGNAL(timeout()), this, SLOT(drawTarget()));
    connect(timer_time, &QTimer::timeout, this, &MainWindow::stopShooting);
    connect(timerZ, &QTimer::timeout, this, &MainWindow::SetButtonsEnabled);
    connect(timer_notAttack, &QTimer::timeout, this, &MainWindow::allow_attack);
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();
    this->setFixedSize(screenSize);
    this->move(0, 0);

    ui->label_BuyNum->setText("0");
    ui->comboBox_ChooseWeapon->setIconSize(QSize(iconSize, iconSize));
    ui->comboBox_WeaponList->setIconSize(QSize(iconSize, iconSize));
    ui->radioButton_is_cypher->setChecked(true);

    ui->comboBox_target_choose->setIconSize(QSize(100, 100));
    ui->comboBox_target_choose->addItem(QIcon(":/img/target0.png"), "");
    ui->comboBox_target_choose->addItem(QIcon(":/img/target1.jpg"), "");
    ui->comboBox_target_choose->addItem(QIcon(":/img/target2.png"), "");
    ui->comboBox_target_choose->addItem(QIcon(":/img/target3.png"), "");

    targets[0] = ":/img/target0.png";
    targets[1] = ":/img/target1.jpg";
    targets[2] = ":/img/target2.png";
    targets[3] = ":/img/target3.png";

    fl = false;
    SetButtonsVisible();
    SetButtonsEnabled();
}

MainWindow::~MainWindow()
{
}
void moveTarget(int moveValue, bool &flag, int &PixmapV, int v1, int v2) {
    if (flag) {
        PixmapV += moveValue;
        if (PixmapV >= v1)
            flag = false;
    }
    else {
        PixmapV -= moveValue;
        if (PixmapV <= v2)
            flag = true;
    }
}
void MainWindow::drawTarget() {
    setPixmapPosition(pixmapX, pixmapY);
    int moveValue = ui->spinBox_speed->value();
    moveTarget(moveValue, moveRight, pixmapX, 1110, 280);
    moveTarget(moveValue, moveDown, pixmapY, 610, 10);
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (action)
        isMouseRelease = false;
    else
        isMouseRelease = true;
    if (event->button() == Qt::LeftButton && timerStart->isActive() && !action && !timerA->isActive() && !weapon_vect.empty()) {
        QPointF globalPos = event->globalPosition();
        widgetPos = mapFromGlobal(globalPos);
        if (widgetPos.x() >= 280 && widgetPos.y() >= 10 && widgetPos.x() <= 1210 && widgetPos.y() <= 710) {
            button_action = comboBox->itemData(comboBox->currentIndex(), Qt::UserRole).toString();
            action = true;
            doAction();
        }
        //else
            //doAfterAction();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (comboBox != nullptr && isMouseRelease) {
        //QPointF globalPos = event->globalPosition();
        //widgetPos = mapFromGlobal(globalPos);
        if (widgetPos.x() >= 280 && widgetPos.y() >= 10 && widgetPos.x() <= 1210 && widgetPos.y() <= 710) {
            if (button_action == comboBox->itemData(comboBox->currentIndex(), Qt::UserRole).toString())
                doAfterAction();
        }
    }
}

void MainWindow::setPixmapPosition(int x, int y)
{
    pixmapX = x;
    pixmapY = y;
    update(); // Перерисовываем виджет
}

void MainWindow::setPixmap(const QPixmap &newPixmap)
{
    if (!newPixmap.isNull())
        pixmap = newPixmap;
    update(); // Перерисовываем виджет
}

void MainWindow::setLibraries(const vector<HINSTANCE> &libs, const vector<HINSTANCE> &librs, const vector<HINSTANCE> &ser_libs)
{
    this->libraries = libs;
    this->librs = librs;
    this->ser_libs = ser_libs;
    int j = 0;
    typedef void (*SetImg)(QComboBox *comboBox);
    for (int i = 0; i < this->librs.size(); i++) {
        typedef void (*SetTimer)(QTimer*);
        SetTimer setTimerA = (SetTimer)GetProcAddress(this->librs[i], "SetTimerA");
        SetTimer setTimerB = (SetTimer)GetProcAddress(this->librs[i], "SetTimerB");
        if (setTimerA != NULL && setTimerB != NULL) {
            setTimerA(timerA);
            setTimerB(timerB);
        }
        else
            qDebug() << "setTimerA setTimerB NULL";
        SetImg setImgs = (SetImg)GetProcAddress(this->librs[i], "SetImgs");
        if (setImgs != NULL)
            setImgs(ui->comboBox_ChooseWeapon);
        else
            qDebug() << "setImgs NULL";
        CreateInstance createInstance = (CreateInstance)GetProcAddress(this->librs[i], "createInstance");
        if (createInstance != NULL) {
            typedef int (*WeaponsCount)();
            WeaponsCount weaponsCount = (WeaponsCount)GetProcAddress(this->librs[i], "WeaponsCount");
            if (weaponsCount != NULL) {
                int wN = weaponsCount();
                for (int ii = 0; ii < wN; ii++)
                    wepMap[j++] = make_pair(this->librs[i], createInstance);
            }
        }
        else
            qDebug() << "mainwindow 111 method NULL j == " + QString::number(j);
    }
    soldier.set_maps(this->librs);
}

vector<HINSTANCE> MainWindow::getLibraries()
{
    return libraries;
}

void MainWindow::Recovering()
{
    bool isDoing;
    bool isClosing;
    Dialog dialog;
    dialog.setWindowTitle("Recovering...");
    dialog.setLabelText("Восcтановить последнюю\nсессию?");
    dialog.setIsSaving(false);
    dialog.set_soldier(&soldier);
    dialog.set_isClosing(&isClosing);
    dialog.set_ser_libs(ser_libs);
    MainWindowToSerialize dataMainWindow;
    dialog.set_dataMainWindow(&dataMainWindow);
    dialog.set_isDoing(&isDoing);
    dialog.set_wepMap(&wepMap);
    dialog.set_weapon_vect(&weapon_vect);
    dialog.show();
    dialog.exec();
    if (isDoing) {
        if (!(dataMainWindow.remaining_time == -1 && dataMainWindow.ChooseWeapon_curr_index == -1
              && dataMainWindow.WeaponList_curr_index == -1 && dataMainWindow.target_choose_curr_index == -1)) {
            action = dataMainWindow.action;
            isMouseRelease = dataMainWindow.isMouseRelease;
            moveRight = dataMainWindow.moveRight;
            moveDown = dataMainWindow.moveDown;
            fl = dataMainWindow.fl;
            widgetPos = dataMainWindow.widgetPos;
            pixmapX = dataMainWindow.pixmapX;
            pixmapY = dataMainWindow.pixmapY;
            typedef void (*SetImgByIndex)(QComboBox*, int);
            for (int i = 0; i < weapon_vect.size(); i++) {
                SetImgByIndex setImgByIndex = (SetImgByIndex)GetProcAddress(weapon_vect.at(i).hDll, "SetImgByIndex");
                if (setImgByIndex != NULL)
                    setImgByIndex(ui->comboBox_WeaponList, weapon_vect.at(i).true_number);
                else
                    qDebug() << "mainwindow 334 method NULL";
            }
            ui->label_BuyNum->setText(QString::number(weapon_vect.size()));
            if (dataMainWindow.WeaponList_curr_index < ui->comboBox_WeaponList->count())
                ui->comboBox_WeaponList->setCurrentIndex(dataMainWindow.WeaponList_curr_index);
            if (dataMainWindow.ChooseWeapon_curr_index < ui->comboBox_ChooseWeapon->count())
                ui->comboBox_ChooseWeapon->setCurrentIndex(dataMainWindow.ChooseWeapon_curr_index);
            if (dataMainWindow.target_choose_curr_index < ui->comboBox_target_choose->count())
                ui->comboBox_target_choose->setCurrentIndex(dataMainWindow.target_choose_curr_index);
            ui->spinBox_speed->setValue(dataMainWindow.speed);
            ui->spinBox_time->setValue(dataMainWindow.time);
            if (dataMainWindow.remaining_time > 0) {
                ui->pushButton_start->setText("Продолжить");
                remaining_time = dataMainWindow.remaining_time;
            }
            ui->listWidget_Info->setEnabled(true);
            ui->listWidget_Info->setVisible(true);
        }
    }
}


void MainWindow::on_pushButton_Buy_clicked()
{
    buying_weapon(ui->comboBox_ChooseWeapon->currentIndex());
}

void MainWindow::buying_weapon(int number)
{
    auto iterator = wepMap.find(number);
    int n = number;
    if (iterator != wepMap.end()) {
        for (int i = 0; i <= number && i < wepMap.size() - 1; i++) {
            if (wepMap.at(i).second == wepMap.at(number).second) {
                number -= i;
                break;
            }
        }
        auto weapon = iterator->second.second(number);
        wepData wepD;
        wepD.hDll = iterator->second.first;
        wepD.weap = weapon;
        wepD.number = n;
        wepD.true_number = number;
        weapon_vect.push_back(wepD);
        typedef void (*SetImgByIndex)(QComboBox*, int);
        SetImgByIndex setImgByIndex = (SetImgByIndex)GetProcAddress(iterator->second.first, "SetImgByIndex");
        if (setImgByIndex != NULL)
            setImgByIndex(ui->comboBox_WeaponList, number);
        else
            qDebug() << "mainwindow 235 method NULL";
        ui->label_BuyNum->setText(QString::number(weapon_vect.size()));
    }
}


void MainWindow::on_pushButton_Throw_clicked()
{
    if (!weapon_vect.empty()) {
        buttonsClear();
        int index = ui->comboBox_WeaponList->currentIndex();
        auto weapon = weapon_vect.at(index);
        typedef void (*DestroyInstance)(Weapon*);
        DestroyInstance destroyInstance = (DestroyInstance)GetProcAddress(weapon.hDll, "destroyInstance");
        if (destroyInstance != NULL)
            destroyInstance(weapon.weap);
        else
            qDebug() << "mainwindow 227 method NULL";
        weapon_vect.erase(weapon_vect.begin() + index);
        ui->label_BuyNum->setText(QString::number(weapon_vect.size()));
        ui->comboBox_WeaponList->removeItem(index);
    }
}

void MainWindow::on_comboBox_WeaponList_currentIndexChanged(int index)
{
    if (!weapon_vect.empty()) {
        soldier.listAttackTexts.clear();
        soldier.listButtonTexts.clear();
        accepting("", index);
        buttonsClear();
        if (!soldier.listButtonTexts.empty()) {
            int x = 1295;
            int y = 280;
            for (list<QString>::iterator it = soldier.listButtonTexts.begin(); it != soldier.listButtonTexts.end(); it++) {
                int length = (*it).length();
                char* path = new char[length + 1]; // +1 для завершающего нулевого символа
                for (int i = 0; i < length; ++i) {
                    QChar ch = (*it).at(i);
                    path[i] = ch.unicode();
                }
                path[length] = '\0';
                QToolButton *button = new QToolButton(this);
                button->setIcon(QIcon(path));
                button->setFixedSize(100, 100);
                button->setAutoFillBackground(true);
                button->setIconSize(button->size());
                button->setStyleSheet("border-radius: 50px;");
                connect(button, &QToolButton::clicked, this, &MainWindow::onButtonClicked);
                button->setProperty(path, (*it));
                delete[] path;
                button->move(x, y);
                y += 110;
                button->show();
                buttonsImgs[button] = *it;
                buttons.push_back(button);
            }
        }
        if (!soldier.listAttackTexts.empty()) {
            int i = 0;
            comboBox = new QComboBox(this);
            comboBox->setFixedSize(80, 50);
            comboBox->setIconSize(QSize(50, 50));
            comboBox->move(1305, 220);
            for (list<QString>::iterator it = soldier.listAttackTexts.begin(); it != soldier.listAttackTexts.end(); it++) {
                comboBox->addItem(QIcon(*it), "");
                comboBox->setItemData(i, *it, Qt::UserRole);
                i++;
            }
            comboBox->show();
        }
        fl = true;
    }
    else
        fl = false;
    SetButtonsVisible();
    SetButtonsEnabled();
}
bool MainWindow::checkActionAttack() {
    unordered_map<QString, pair<unordered_map<QString, QString>, unordered_map<QString, int>>> weaponMap;
    weaponMap = soldier.get_weaponMap();
    typedef QString (*GetWeaponType)();
    bool is_notFound = false;
    bool is_notAttack = false;
    for (auto lib : librs) {
        GetWeaponType getWeaponType = (GetWeaponType)GetProcAddress(lib, "get_weaponType");
        if (getWeaponType != NULL) {
            auto it = weaponMap.find(getWeaponType());
            if (it != weaponMap.end()) {
                auto it1 = it->second.first.find(button_action);
                if (it1 == it->second.first.end())
                    is_notFound = true;
                else {
                    if (it1->second != Attack_str)
                        is_notAttack = true;
                    is_notFound = false;
                    break;
                }
            }
        }
        else
            qDebug() << "mainwindow  method NULL";

    }
    if (is_notFound)
        return false;
    if (is_notAttack)
        return true;
    if (soldier.isAttack && (widgetPos.x() < 280 || widgetPos.x() > 1210 || widgetPos.y() < 10 || widgetPos.y() > 710)) {
        doAfterAction();
        return false;
    }
    else {
        if (timer_time->isActive()) {
            QPoint globalPos = QCursor::pos();
            widgetPos = mapFromGlobal(globalPos);
            typedef void (*SetCoordinates)(void*, QPointF, int, int);
            SetCoordinates setCoordinates =
                (SetCoordinates)GetProcAddress(weapon_vect[ui->comboBox_WeaponList->currentIndex()].hDll,"SetCoordinates");
            setCoordinates(weapon_vect[ui->comboBox_WeaponList->currentIndex()].weap, widgetPos, pixmapX, pixmapY);
            return true;
        }
        else
            return false;
    }
}
void MainWindow::doAction() {
    if (checkActionAttack()) {
        soldier.listAttackTexts.clear();
        soldier.listButtonTexts.clear();
        accepting(button_action, ui->comboBox_WeaponList->currentIndex());
    }
    //else
        //mouseReleaseEvent(nullptr);
}
void MainWindow::allow_attack() {
    action = false;
    if (timer_notAttack->isActive())
        timer_notAttack->stop();
}

void MainWindow::start_animation(int x, int y, bool bRight, bool bDown, int remaining_time)
{
    setPixmapPosition(x, y);
    timerStart->start(1);
    timer_time->start(remaining_time);
    moveRight = bRight;
    moveDown = bDown;
    ui->pushButton_start->setText("Завершить");
}
void MainWindow::doAfterAction() {
    fl = false;
    QTimer *timer = nullptr;
    if (!soldier.isAttack)
        timer = timerB;
    else
        timer = timerA;
    if (timer->isActive()) {
        SetButtonsEnabled();
        fl = true;
        timerZ->start(timer->remainingTime());
        timer_notAttack->start(timer->remainingTime());
        timer->stop();
    }
    else {
        if (widgetPos.x() >= 280 && widgetPos.y() >= 10 && widgetPos.x() <= 1210 && widgetPos.y() <= 710)
            action = false;
    }
}


void MainWindow::stopShooting() {
    if (timerStart->isActive()) {
        timerStart->stop();
        timer_time->stop();
        setPixmap(QPixmap());
        ui->pushButton_start->setText("Старт");
    }
}
void MainWindow::on_pushButton_start_clicked()
{
    if (timerStart->isActive()) {
        stopShooting();
    }
    else {
        if (ui->pushButton_start->text() != "Продолжить") {
            setPixmap(QPixmap(targets.at(ui->comboBox_target_choose->currentIndex())));
            start_animation(getRandomInt(280, 1110), getRandomInt(10, 610), getRandomInt(0, 1), getRandomInt(0, 1), ui->spinBox_time->value() * 1000);
        }
        else {
            QString filePath = QCoreApplication::applicationDirPath() + "/saved_target.png";
            setPixmap(QPixmap(filePath));
            start_animation(pixmapX, pixmapY, moveRight, moveDown, remaining_time);
        }
    }
}

void MainWindow::accepting(QString action, int index)
{
    auto weapon = weapon_vect.at(index);
    typedef QPixmap& (*Accept)(Soldier*, QString, HINSTANCE, Weapon*, QPixmap*);
    Accept accept = (Accept)GetProcAddress(weapon.hDll, "Accept");
    setPixmap(accept(&soldier, action, weapon.hDll, weapon.weap, &pixmap));
    //weapon.second->accept(&soldier, "", weapon.first, weapon.second, &pixmap);

    typedef int (*GetInfoSize)(void*);
    GetInfoSize getInfoSize = (GetInfoSize)GetProcAddress(weapon.hDll, "get_infoSize");
    int len = 0;
    if (getInfoSize != NULL)
        len = getInfoSize(weapon.weap);
    else
        qDebug() << "mainwindow 258 method NULL";
    ui->listWidget_Info->clear();
    typedef QString (*Indexator)(void*, int);
    for (int i = 0; i < len; i++) {
        QListWidgetItem *listItem = new QListWidgetItem;
        Indexator indexator = (Indexator)GetProcAddress(weapon.hDll, "indexator");
        if (indexator != NULL) {
            listItem->setText(indexator(weapon.weap, i));
            ui->listWidget_Info->addItem(listItem);
        }
        else
            qDebug() << "mainwindow 270 method NULL";
        //delete listItem;
    }
}

void MainWindow::onButtonClicked()
{
    QToolButton *clickedButton = qobject_cast<QToolButton*>(sender());
    button_action = buttonsImgs.at(clickedButton);
    action = true;
    soldier.isAttack = false;
    doAction();
    doAfterAction();
}

void MainWindow::buttonsClear()
{
    for (int i = 0; i < buttons.size(); i++) {
        delete buttons[i];
        buttons[i] = nullptr;
    }
    buttons.clear();
    buttonsImgs.clear();
    if (comboBox != nullptr) {
        delete comboBox;
        comboBox = nullptr;
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    if (Dialog::get_count() != 0) {
        Dialog::dialogs_close();
    }
    Dialog dialog;
    dialog.setWindowTitle("Saving...");
    dialog.setLabelText("  Желаете сохранить?");
    dialog.setIsSaving(true);
    bool isDoing;
    dialog.set_isDoing(&isDoing);
    bool isClosing = false;
    dialog.set_isClosing(&isClosing);
    dialog.set_soldier(&soldier);
    dialog.set_weapon_vect(&weapon_vect);
    dialog.set_ser_libs(ser_libs);
    dialog.set_isEncryption(ui->radioButton_is_cypher->isChecked());
    vector<QString> infoVector;
    int itemCount = ui->listWidget_Info->count();
    for (int i = 0; i < itemCount; i++) {
        QListWidgetItem* item = ui->listWidget_Info->item(i);
        if (item) {
            QString itemText = item->text();
            infoVector.push_back(itemText);
        }
    }
    MainWindowToSerialize dataMainWindow;
    dataMainWindow.action = action;
    dataMainWindow.isMouseRelease = isMouseRelease;
    dataMainWindow.remaining_time = timer_time->remainingTime();
    dataMainWindow.moveRight = moveRight;
    dataMainWindow.moveDown = moveDown;
    dataMainWindow.fl = fl;
    dataMainWindow.widgetPos = widgetPos;
    dataMainWindow.pixmapX = pixmapX;
    dataMainWindow.pixmapY = pixmapY;
    dataMainWindow.WeaponList_curr_index = ui->comboBox_WeaponList->currentIndex();
    dataMainWindow.ChooseWeapon_curr_index = ui->comboBox_ChooseWeapon->currentIndex();
    dataMainWindow.target_choose_curr_index = ui->comboBox_target_choose->currentIndex();
    dataMainWindow.speed = ui->spinBox_speed->value();
    dataMainWindow.time = ui->spinBox_time->value();
    dataMainWindow.vect_info.clear();
    for (int i = 0; i < ui->listWidget_Info->count(); i++)
        dataMainWindow.vect_info.push_back(ui->listWidget_Info->item(i)->text());
    dialog.set_dataMainWindow(&dataMainWindow);
    dialog.show();
    dialog.exec();
    if (isClosing) {
        if (!pixmap.isNull()) {
            QString filePath = QCoreApplication::applicationDirPath() + "/saved_target.png";
            if (pixmap.save(filePath))
                qDebug() << "Изображение успешно сохранено.";
            else
                qDebug() << "Ошибка при сохранении изображения.";
        }
        else
            qDebug() << "Изображение не загружено или не найдено.";
        close();
    }
}

void MainWindow::on_pushButton_transformator_clicked()
{
    QFile file("TextFile.txt");
    QByteArray byteArray;
    bool wasTransformed = false;
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
        if (byteArray.size() != 0 && (jsonDoc.isNull() && b)) {
            byteArray = Dialog::cryptography_perform(byteArray, "callDecryptText");
            wasTransformed = true;
        }
    }
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QXmlStreamReader xml(byteArray);
    typedef QByteArray (*transform)(const QByteArray &byteArray);
    transform perform_transformation = NULL;
    for (int i = 0; i < ser_libs.size(); i++) {
        if (!jsonDoc.isNull())
            perform_transformation = (transform)GetProcAddress(ser_libs.at(i), "fromJsonToXml");
        else
            perform_transformation = (transform)GetProcAddress(ser_libs.at(i), "fromXmlToJson");
        if (perform_transformation != NULL) {
            QFile file1("TextFile.txt");
            byteArray = perform_transformation(byteArray);
            if (wasTransformed)
                byteArray = Dialog::cryptography_perform(byteArray, "callEncryptText");
            if (file1.open(QIODevice::WriteOnly | QIODevice::Text))
                file1.write(byteArray);
            file1.close();
            break;
        }
    }
}

