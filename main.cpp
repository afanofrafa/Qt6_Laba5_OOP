#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QStringList>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <QCryptographicHash>
QString generateSignature(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка: не удалось открыть файл.";
        return "";
    }
    QByteArray fileData = file.readAll();
    QByteArray hashData = QCryptographicHash::hash(fileData, QCryptographicHash::Sha256);
    QString signature = hashData.toHex();

    file.close();
    return signature;
}
bool verifySignature(const QString &filePath, const QString &expectedSignature)
{
    QString actualSignature = generateSignature(filePath);
    return (actualSignature == expectedSignature);
}
bool checkLibraries(vector<pair<QString, QString>> &dllPaths, const vector<QString> &vector_verify)
{
    for (int i = 0; i < dllPaths.size(); i++) {
        bool isFound = false;
        for (int j = 0; j < vector_verify.size(); j++) {
            if (verifySignature(dllPaths[i].first, vector_verify[j])) {
                isFound = true;
                dllPaths[i].second = vector_verify[j];
                break;
            }
        }
        if (!isFound) {
            dllPaths.erase(dllPaths.begin() + i);
            i--;
        }
    }
    if (dllPaths.size() == 0)
        return false;
    else
        return true;
}

/*wstring s2ws(const string& str) {
    int len;
    int slength = (int)str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
    wstring r(buf);
    delete[] buf;
    return r;
}*/
int main(int argc, char *argv[])
{
    //QString actualSignature = generateSignature("libSerializer.dll");
    QApplication a(argc, argv);
    vector<QString> vector_verify;
    //vector_verify.push_back("dab82a2a11c5484eed683845610ab72411882cdf79bdf342d8185ba1d38bfd15"); //libTraumaticWeapon.dll
    vector_verify.push_back("c9ca6a3df2a549d91b0ec955acc1e9e34cfca0cbe40f173461094406801dd439"); //libTraumaticWeaponTemplate.dll
    //vector_verify.push_back("7d925865727812175289d1d547d41cb37cf890586beec2f8b0b6907b3ee0421b"); //libBladedWeapon.dll
    vector_verify.push_back("68e7dd48c0739d7203d6a215b604ce5d302023dd8d7b0a85b4564a93f4c56d21"); //libBladedWeaponTemplate.dll
    //vector_verify.push_back("fe9413a326190c313fc6821c4cac2ed7747e8143b70f09839b3305654f1a8a11"); //libFirearm.dll
    vector_verify.push_back("8dc7d97973949c7992e25b25f3b66d2bf815593296e355495a155d364e8c0196"); //libFirearmTemplate.dll
    vector_verify.push_back("6335d829acdb660f30a632cf6c042af279a8c14d2c4daf6a0a29f99877568526"); //libSerializer.dll
    vector_verify.push_back("39c16cf10d59cffe31acdb0f04a6fa8653ec569aa1e7db39cc06c0d12de83e87"); //libEncryption.dll
    vector_verify.push_back("b801b82d1cc7895bf5a1a685f32a28705f38baec1c40e845dc15c1485e7e1d1c"); //libTransformator.dll
    vector<HINSTANCE> libraries;
    vector<pair<QString, QString>> dllPaths;
    if (argc > 1) {
        for (int i = 1; i < argc; i++)
            dllPaths.push_back(make_pair(argv[i], ""));
    }
    else
    {
        QDir directory(QCoreApplication::applicationDirPath());
        QStringList filters;
        filters << "*.dll";
        QStringList files = directory.entryList(filters, QDir::Files);
        foreach (QString file, files)
            dllPaths.push_back(make_pair(directory.relativeFilePath(file), ""));
    }
    if (!checkLibraries(dllPaths, vector_verify)) {
        qDebug() << "Ошибка при загрузке библиотек";
        QMessageBox::information(nullptr, "Error", "Ошибка при загрузке библиотек");
        return 1;
    }
    else {
        for (int i = 0; i < dllPaths.size(); i++)
            libraries.push_back(LoadLibrary(dllPaths.at(i).first.toStdWString().c_str()));
    }
    vector<HINSTANCE> librs;
    vector<HINSTANCE> ser_libs;
    for (int i = 0; i < libraries.size(); i++) {
        if (libraries.at(i) == NULL) {
            libraries.erase(libraries.begin() + i);
            dllPaths.erase(dllPaths.begin() + i);
            i--;
        }
        else {
            if (dllPaths.at(i).second == "c9ca6a3df2a549d91b0ec955acc1e9e34cfca0cbe40f173461094406801dd439" ||
                dllPaths.at(i).second == "68e7dd48c0739d7203d6a215b604ce5d302023dd8d7b0a85b4564a93f4c56d21" ||
                dllPaths.at(i).second == "8dc7d97973949c7992e25b25f3b66d2bf815593296e355495a155d364e8c0196") {
                librs.push_back(libraries.at(i));
            }
            else {
                if (dllPaths.at(i).second == "6335d829acdb660f30a632cf6c042af279a8c14d2c4daf6a0a29f99877568526" ||
                    dllPaths.at(i).second == "b801b82d1cc7895bf5a1a685f32a28705f38baec1c40e845dc15c1485e7e1d1c" ||
                    dllPaths.at(i).second == "39c16cf10d59cffe31acdb0f04a6fa8653ec569aa1e7db39cc06c0d12de83e87") {
                    ser_libs.push_back(libraries.at(i));
                }
            }
        }
    }
    if (libraries.size() == 0) {
        qDebug() << "Ошибка при загрузке библиотек";
        QMessageBox::information(nullptr, "Error", "Ошибка при загрузке библиотек");
        return 1;
    }
    MainWindow w;
    w.setLibraries(libraries, librs, ser_libs);
    w.Recovering();
    w.setWindowFlags(Qt::CustomizeWindowHint);
    w.showFullScreen();
    return a.exec();
}
