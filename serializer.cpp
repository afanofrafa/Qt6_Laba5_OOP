#include "serializer.h"
template<typename Type>
void Serializer::toJson(QJsonObject &jsonObj, const vector<pair<QString, Type>> &vect)
{
    for (int i = 0; i < vect.size(); i++)
        jsonObj[vect.at(i).first] = vect.at(i).second;
}

Serializer::Serializer() {}

void serializeWeaponMapXml(const unordered_map<QString, pair<unordered_map<QString, QString>, unordered_map<QString, int>>>& weaponMap, QXmlStreamWriter &xmlProgram) {
    xmlProgram.writeStartElement("weaponMap");
    for (auto it = weaponMap.begin(); it != weaponMap.end(); ++it) {
        const auto& key = it->first;
        const auto& valuePair = it->second;
        const auto& stringMap = valuePair.first;
        const auto& intMap = valuePair.second;

        xmlProgram.writeStartElement(key);

        // Serialize the inner string map
        xmlProgram.writeStartElement("stringMap");
        for (auto sit = stringMap.begin(); sit != stringMap.end(); ++sit) {
            xmlProgram.writeStartElement("pair");
            xmlProgram.writeTextElement("first", sit->first);
            xmlProgram.writeTextElement("second", sit->second);
            xmlProgram.writeEndElement();
        }
        xmlProgram.writeEndElement(); // Close stringMap

        // Serialize the inner int map
        xmlProgram.writeStartElement("intMap");
        for (auto iit = intMap.begin(); iit != intMap.end(); ++iit) {
            xmlProgram.writeStartElement("pair");
            xmlProgram.writeTextElement("first", iit->first);
            xmlProgram.writeTextElement("second", QString::number(iit->second));
            xmlProgram.writeEndElement();
        }
        xmlProgram.writeEndElement(); // Close intMap

        xmlProgram.writeEndElement(); // Close pair
    }
    xmlProgram.writeEndElement(); // Close weaponMap
}
QByteArray Serializer::serialize_to_xml_byte_array(const Data &data)
{
    QByteArray byteArray;
    QXmlStreamWriter xmlProgram(&byteArray);
    xmlProgram.setAutoFormatting(true);
    xmlProgram.writeStartElement("Program");
    xmlProgram.writeStartElement("MainWindow");
    xmlProgram.writeTextElement("ChooseWeapon_curr_index", QString::number(data.dataMainWindow.ChooseWeapon_curr_index));
    xmlProgram.writeTextElement("WeaponList_curr_index", QString::number(data.dataMainWindow.WeaponList_curr_index));
    xmlProgram.writeTextElement("target_choose_curr_index", QString::number(data.dataMainWindow.target_choose_curr_index));
    xmlProgram.writeTextElement("remaining_time", QString::number(data.dataMainWindow.remaining_time));
    xmlProgram.writeTextElement("time", QString::number(data.dataMainWindow.time));
    xmlProgram.writeTextElement("action", data.dataMainWindow.action ? "true" : "false");
    xmlProgram.writeTextElement("fl", data.dataMainWindow.fl ? "true" : "false");
    xmlProgram.writeTextElement("isMouseRelease", data.dataMainWindow.isMouseRelease ? "true" : "false");
    xmlProgram.writeTextElement("moveDown", data.dataMainWindow.moveDown ? "true" : "false");
    xmlProgram.writeTextElement("moveRight", data.dataMainWindow.moveRight ? "true" : "false");
    xmlProgram.writeTextElement("pixmapX", QString::number(data.dataMainWindow.pixmapX));
    xmlProgram.writeTextElement("pixmapY", QString::number(data.dataMainWindow.pixmapY));
    xmlProgram.writeTextElement("speed", QString::number(data.dataMainWindow.speed));
    xmlProgram.writeStartElement("widgetPos");
    xmlProgram.writeTextElement("X", QString::number(data.dataMainWindow.widgetPos.x()));
    xmlProgram.writeTextElement("Y", QString::number(data.dataMainWindow.widgetPos.y()));
    xmlProgram.writeEndElement(); // Close widgetPos
    xmlProgram.writeStartElement("info");
    for (const QString& info : data.dataMainWindow.vect_info)
        xmlProgram.writeTextElement("item", info);
    xmlProgram.writeEndElement();
    xmlProgram.writeEndElement(); // Close MainWindow
    xmlProgram.writeStartElement("Soldier");
    xmlProgram.writeStartElement("listButtonTexts");
    for (const QString& buttonText : data.dataSoldier.listButtonTexts)
        xmlProgram.writeTextElement("itemBT", buttonText);
    xmlProgram.writeEndElement(); // Close listButtonTexts
    xmlProgram.writeStartElement("listAttackTexts");
    for (const QString& attackText : data.dataSoldier.listAttackTexts)
        xmlProgram.writeTextElement("itemAT", attackText);
    xmlProgram.writeEndElement(); // Close listAttackTexts
    serializeWeaponMapXml(data.dataSoldier.weaponMap, xmlProgram);
    xmlProgram.writeTextElement("isAttack", data.dataSoldier.isAttack ? "true" : "false");
    xmlProgram.writeEndElement(); // Close Soldier
    if (data.dataWeapon.size()) {
        xmlProgram.writeStartElement("Weapon");
        for (int i = 0; i < data.dataWeapon.size(); i++) {
            xmlProgram.writeStartElement("wepItem");
            xmlProgram.writeStartElement("vect_int");
            for (int j = 0; j < data.dataWeapon.at(i).vect_int.size(); j++) {
                xmlProgram.writeTextElement(data.dataWeapon.at(i).vect_int.at(j).first,
                                            QString::number(data.dataWeapon.at(i).vect_int.at(j).second));
            }
            xmlProgram.writeEndElement();
            xmlProgram.writeStartElement("vect_bool");
            for (int j = 0; j < data.dataWeapon.at(i).vect_bool.size(); j++) {
                xmlProgram.writeTextElement(data.dataWeapon.at(i).vect_bool.at(j).first,
                                            data.dataWeapon.at(i).vect_bool.at(j).second ? "true" : "false");
            }
            xmlProgram.writeEndElement();
            xmlProgram.writeStartElement("vect_QString");
            for (int j = 0; j < data.dataWeapon.at(i).vect_QString.size(); j++) {
                xmlProgram.writeTextElement(data.dataWeapon.at(i).vect_QString.at(j).first, data.dataWeapon.at(i).vect_QString.at(j).second);
            }
            xmlProgram.writeEndElement();
            for (int j = 0; j < data.dataWeapon.at(i).vect_QPointF.size(); j++) {
                xmlProgram.writeStartElement("vect_QPointF");
                xmlProgram.writeTextElement("X", QString::number(data.dataWeapon.at(i).vect_QPointF.at(j).second.x()));
                xmlProgram.writeTextElement("Y", QString::number(data.dataWeapon.at(i).vect_QPointF.at(j).second.y()));
                xmlProgram.writeEndElement();
            }
            xmlProgram.writeTextElement("number", QString::number(data.dataWeapon.at(i).number));
            xmlProgram.writeTextElement("true_number", QString::number(data.dataWeapon.at(i).true_number));

            xmlProgram.writeStartElement("info");
            for (const QString& info : data.dataWeapon.at(i).vect_info)
                xmlProgram.writeTextElement("item", info);
            xmlProgram.writeEndElement();
            xmlProgram.writeEndElement();
        }
        xmlProgram.writeEndElement();
    }

    xmlProgram.writeEndElement();

    return byteArray;
}

Data Serializer::deserialize_from_xml_byte_array(const QByteArray &byteArray) {
    Data data;

    QXmlStreamReader xml(byteArray);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "MainWindow") {
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "MainWindow")) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString name = xml.name().toString();
                        xml.readNext();
                        QString value = xml.text().trimmed().toString();
                        if (!value.isEmpty()) {
                            if (name == "ChooseWeapon_curr_index") {
                                data.dataMainWindow.ChooseWeapon_curr_index = value.toInt();
                            } else if (name == "WeaponList_curr_index") {
                                data.dataMainWindow.WeaponList_curr_index = value.toInt();
                            } else if (name == "target_choose_curr_index") {
                                data.dataMainWindow.target_choose_curr_index = value.toInt();
                            } else if (name == "remaining_time") {
                                data.dataMainWindow.remaining_time = value.toInt();
                            } else if (name == "time") {
                                data.dataMainWindow.time = value.toInt();
                            } else if (name == "action") {
                                data.dataMainWindow.action = (value == "true");
                            } else if (name == "fl") {
                                data.dataMainWindow.fl = (value == "true");
                            } else if (name == "isMouseRelease") {
                                data.dataMainWindow.isMouseRelease = (value == "true");
                            } else if (name == "moveDown") {
                                data.dataMainWindow.moveDown = (value == "true");
                            } else if (name == "moveRight") {
                                data.dataMainWindow.moveRight = (value == "true");
                            } else if (name == "pixmapX") {
                                data.dataMainWindow.pixmapX = value.toInt();
                            } else if (name == "pixmapY") {
                                data.dataMainWindow.pixmapY = value.toInt();
                            } else if (name == "speed") {
                                data.dataMainWindow.speed = value.toInt();
                            } else if (name == "X") {
                                data.dataMainWindow.widgetPos.setX(xml.text().toInt());
                            } else if (name == "Y") {
                                data.dataMainWindow.widgetPos.setY(xml.text().toInt());
                            } else if (name == "item") {
                                data.dataMainWindow.vect_info.push_back(xml.text().toString());
                            }
                        }
                    }
                    xml.readNext();
                }
            } else if (xml.name().toString() == "Soldier") {
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "Soldier")) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString name = xml.name().toString();
                        xml.readNext();
                        QString value = xml.text().trimmed().toString();
                        if (!value.isEmpty()) {
                            if (name == "itemAT") {
                                data.dataSoldier.listAttackTexts.push_back(value);
                            } else if (name == "itemBT") {
                                data.dataSoldier.listButtonTexts.push_back(value);
                            } else if (name == "isAttack") {
                                data.dataSoldier.isAttack = (value == "true");
                            }
                        } else if ( (name == "Firearm") || (name == "BladedWeapon") || (name == "TraumaticWeapon") ) {
                            QString wep_type(name);
                            unordered_map<QString, QString> stringMap;
                            unordered_map<QString, int> intMap;
                            while (!(name == "stringMap")) {
                                xml.readNext();
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                            }
                            if (name == "stringMap") {
                                while (name != "intMap") {
                                    QString first("");
                                    while (name != "first") {
                                        xml.readNext();
                                        name = xml.name().toString();
                                        if (name == "intMap")
                                            break;
                                        xml.readNext();
                                        value = xml.text().trimmed().toString();
                                    }
                                    if (name == "intMap")
                                        break;
                                    first = value;
                                    while (name != "second") {
                                        xml.readNext();
                                        name = xml.name().toString();
                                        if (name == "intMap")
                                            break;
                                        xml.readNext();
                                        value = xml.text().trimmed().toString();
                                    }
                                    if (name == "intMap")
                                        break;
                                    stringMap[first] = value;
                                }
                            }
                            if (name == "intMap") {
                                while (!((xml.name().toString() == "Firearm")
                                         || (xml.name().toString() == "BladedWeapon")
                                         || (xml.name().toString() == "TraumaticWeapon") )) {
                                    QString first("");
                                    while (name != "first") {
                                        xml.readNext();
                                        if ( (xml.name().toString() == "Firearm") || (xml.name().toString() == "BladedWeapon") || (xml.name().toString() == "TraumaticWeapon") )
                                            break;
                                        if (xml.isStartElement()) {
                                            name = xml.name().toString();
                                            xml.readNext();
                                            if ( (xml.name().toString() == "Firearm") || (xml.name().toString() == "BladedWeapon") || (xml.name().toString() == "TraumaticWeapon") )
                                                break;
                                            if (xml.isCharacters()) {
                                                value = xml.text().trimmed().toString();
                                            }
                                        }
                                    }
                                    if ( (xml.name().toString() == "Firearm") || (xml.name().toString() == "BladedWeapon") || (xml.name().toString() == "TraumaticWeapon") )
                                        break;
                                    first = value;
                                    while (name != "second") {
                                        xml.readNext();
                                        if ( (xml.name().toString() == "Firearm") || (xml.name().toString() == "BladedWeapon") || (xml.name().toString() == "TraumaticWeapon") )
                                            break;
                                        if (xml.isStartElement()) {
                                            name = xml.name().toString();
                                            xml.readNext();
                                            if ( (xml.name().toString() == "Firearm") || (xml.name().toString() == "BladedWeapon") || (xml.name().toString() == "TraumaticWeapon") )
                                                break;
                                            if (xml.isCharacters()) {
                                                value = xml.text().trimmed().toString();
                                            }
                                        }
                                    }
                                    if ( (xml.name().toString() == "Firearm") || (xml.name().toString() == "BladedWeapon") || (xml.name().toString() == "TraumaticWeapon") )
                                        break;
                                    intMap[first] = value.toInt();
                                }
                            }
                            data.dataSoldier.weaponMap[wep_type] = make_pair(stringMap, intMap);
                        }
                    }
                    xml.readNext();
                }
            } else if (xml.name().toString() == "Weapon") {
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "Weapon")) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        WeaponToSerialize weap;
                        QString name;
                        QString value;
                        while (name != "vect_int") {
                            if (xml.isStartElement()) {
                                name = xml.name().toString();
                                xml.readNext();
                                if (xml.isCharacters())
                                    value = xml.text().trimmed().toString();
                            } else {
                                xml.readNext();
                            }
                        }
                        if (name == "vect_int") {
                            while (name != "vect_bool") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (!value.isEmpty())
                                    weap.vect_int.push_back(make_pair(name, value.toInt()));
                            }
                        }
                        if (name == "vect_bool") {
                            while (name != "vect_QString") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (!value.isEmpty())
                                    weap.vect_bool.push_back(make_pair(name, (value == "true") ? true : false));
                            }
                        }
                        if (name == "vect_QString") {
                            while (name != "vect_QPointF") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (!value.isEmpty())
                                    weap.vect_QString.push_back(make_pair(name, value));
                            }
                        }
                        if (name == "vect_QPointF") {
                            while (name != "number") {
                                QPointF p;
                                while (name != "X" && name != "number") {
                                    name = xml.name().toString();
                                    xml.readNext();
                                    value = xml.text().trimmed().toString();
                                }
                                if (name == "number")
                                    break;
                                p.setX(value.toInt());
                                while (name != "Y" && name != "number") {
                                    name = xml.name().toString();
                                    xml.readNext();
                                    value = xml.text().trimmed().toString();
                                }
                                if (name == "number")
                                    break;
                                p.setY(value.toInt());
                                weap.vect_QPointF.push_back(make_pair(name, p));
                            }
                        }
                        if (name == "number") {
                            weap.number = value.toInt();
                            while (name != "true_number") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                            }
                        }
                        if (name == "true_number") {
                            weap.true_number = value.toInt();
                            while (name != "info") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                            }
                        }
                        if (name == "info") {
                            while ( !(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "info") ) {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (name == "item" && !value.isEmpty())
                                    weap.vect_info.push_back(value);
                            }
                        }
                        data.dataWeapon.push_back(weap);
                    }
                    xml.readNext();
                }
            }
        }
    }

    if (xml.hasError()) {
        qDebug() << "XML error: " << xml.errorString();
    }

    return data;
}

QJsonObject serializeWeaponMapJson(const unordered_map<QString, pair<unordered_map<QString, QString>, unordered_map<QString, int>>>& weaponMap) {
    QJsonObject jsonWeaponMap;
    for (const auto& entry : weaponMap) {
        const QString& key = entry.first;
        const auto& valuePair = entry.second;
        const auto& stringMap = valuePair.first;
        const auto& intMap = valuePair.second;

        QJsonObject jsonPair;
        QJsonObject jsonStringMap;
        QJsonObject jsonIntMap;

        // Serialize the inner string map
        for (const auto& stringEntry : stringMap) {
            jsonStringMap[stringEntry.first] = stringEntry.second;
        }

        // Serialize the inner int map
        for (const auto& intEntry : intMap) {
            jsonIntMap[intEntry.first] = intEntry.second;
        }

        // Add the serialized maps to the pair object
        jsonPair["stringMap"] = jsonStringMap;
        jsonPair["intMap"] = jsonIntMap;

        // Add the pair object to the weapon map
        jsonWeaponMap[key] = jsonPair;
    }
    return jsonWeaponMap;
}
QByteArray Serializer::serialize_to_json_byte_array(const Data &data)
{
    QJsonArray jsonWeapon;
    for (int i = 0; i < data.dataWeapon.size(); i++) {
        QJsonObject jsonObj;
        toJson(jsonObj, data.dataWeapon.at(i).vect_int);
        toJson(jsonObj, data.dataWeapon.at(i).vect_bool);
        toJson(jsonObj, data.dataWeapon.at(i).vect_QString);
        for (int j = 0; j < data.dataWeapon.at(i).vect_QPointF.size(); j++) {
            QJsonObject json_Obj;
            json_Obj["X"] = data.dataWeapon.at(i).vect_QPointF.at(j).second.x();
            json_Obj["Y"] = data.dataWeapon.at(i).vect_QPointF.at(j).second.y();
            jsonObj[data.dataWeapon.at(i).vect_QPointF.at(j).first] = json_Obj;
        }
        jsonObj["number"] = data.dataWeapon.at(i).number;
        jsonObj["true_number"] = data.dataWeapon.at(i).true_number;
        QJsonArray infoArray;
        for (const QString& info : data.dataWeapon.at(i).vect_info)
            infoArray.append(info);
        jsonObj["info"] = infoArray;
        jsonWeapon.append(jsonObj);
    }
    QJsonObject jsonSoldier;
    QJsonArray listButtonTextsArray;
    for(const auto& buttonText : data.dataSoldier.listButtonTexts) {
        listButtonTextsArray.append(buttonText);
    }
    jsonSoldier["listButtonTexts"] = listButtonTextsArray;

    QJsonArray listAttackTextsArray;
    for(const auto& attackText : data.dataSoldier.listAttackTexts) {
        listAttackTextsArray.append(attackText);
    }
    jsonSoldier["listAttackTexts"] = listAttackTextsArray;
    jsonSoldier["isAttack"] = data.dataSoldier.isAttack;
    QJsonObject json_weaponMap = serializeWeaponMapJson(data.dataSoldier.weaponMap);
    jsonSoldier["weaponMap"] = json_weaponMap;
    QJsonObject jsonMainWindow;
    jsonMainWindow["ChooseWeapon_curr_index"] = data.dataMainWindow.ChooseWeapon_curr_index;
    jsonMainWindow["WeaponList_curr_index"] = data.dataMainWindow.WeaponList_curr_index;
    jsonMainWindow["target_choose_curr_index"] = data.dataMainWindow.target_choose_curr_index;
    jsonMainWindow["remaining_time"] = data.dataMainWindow.remaining_time;
    jsonMainWindow["time"] = data.dataMainWindow.time;
    jsonMainWindow["action"] = data.dataMainWindow.action;
    jsonMainWindow["fl"] = data.dataMainWindow.fl;
    jsonMainWindow["isMouseRelease"] = data.dataMainWindow.isMouseRelease;
    jsonMainWindow["moveDown"] = data.dataMainWindow.moveDown;
    jsonMainWindow["moveRight"] = data.dataMainWindow.moveRight;
    jsonMainWindow["pixmapX"] = data.dataMainWindow.pixmapX;
    jsonMainWindow["pixmapY"] = data.dataMainWindow.pixmapY;
    jsonMainWindow["speed"] = data.dataMainWindow.speed;
    QJsonObject jsonWidgetPos;
    jsonWidgetPos["X"] = data.dataMainWindow.widgetPos.x();
    jsonWidgetPos["Y"] = data.dataMainWindow.widgetPos.y();
    jsonMainWindow["widgetPos"] = jsonWidgetPos;
    QJsonArray infoArray;
    for (const QString& info : data.dataMainWindow.vect_info)
        infoArray.append(info);
    jsonMainWindow["info"] = infoArray;
    QJsonObject jsonProgram;
    if (data.dataWeapon.size() != 0)
        jsonProgram["Weapon"] = jsonWeapon;
    jsonProgram["Soldier"] = jsonSoldier;
    jsonProgram["MainWindow"] = jsonMainWindow;
    QJsonDocument jsonDoc(jsonProgram);
    return jsonDoc.toJson();
}

Data Serializer::deserialize_from_json_byte_array(const QByteArray &byteArray)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoc.object();

    // Восстановление данных MainWindow
    QJsonObject mainObj = jsonObject["MainWindow"].toObject();
    MainWindowToSerialize mainWindowData;
    mainWindowData.action = mainObj["action"].toBool();
    mainWindowData.isMouseRelease = mainObj["isMouseRelease"].toBool();
    mainWindowData.remaining_time = mainObj["remaining_time"].toInt();
    mainWindowData.moveRight = mainObj["moveRight"].toBool();
    mainWindowData.moveDown = mainObj["moveDown"].toBool();
    mainWindowData.fl = mainObj["fl"].toBool();
    mainWindowData.widgetPos = QPointF(mainObj["widgetPos"].toObject()["X"].toInt(),
                                       mainObj["widgetPos"].toObject()["Y"].toInt());
    mainWindowData.pixmapX = mainObj["pixmapX"].toInt();
    mainWindowData.pixmapY = mainObj["pixmapY"].toInt();
    mainWindowData.WeaponList_curr_index = mainObj["WeaponList_curr_index"].toInt();
    mainWindowData.ChooseWeapon_curr_index = mainObj["ChooseWeapon_curr_index"].toInt();
    mainWindowData.target_choose_curr_index = mainObj["target_choose_curr_index"].toInt();
    mainWindowData.speed = mainObj["speed"].toInt();
    mainWindowData.time = mainObj["time"].toInt();
    // Восстановление данных info
    if (mainObj.contains("info")) {
        QJsonArray infoArray = mainObj["info"].toArray();
        mainWindowData.vect_info.clear(); // Очищаем вектор перед восстановлением
        for (const auto& infoValue : infoArray)
            mainWindowData.vect_info.push_back(infoValue.toString());
    }

    // Восстановление данных Soldier
    QJsonObject soldierObj = jsonObject["Soldier"].toObject();
    SoldierToSerialize soldierData;
    soldierData.isAttack = soldierObj["isAttack"].toBool();
    // Восстановление weaponMap
    QJsonObject weaponMapObj = soldierObj["weaponMap"].toObject();
    for (auto it = weaponMapObj.begin(); it != weaponMapObj.end(); ++it) {
        QString key = it.key();
        QJsonObject innerObj = it.value().toObject();
        unordered_map<QString, QString> stringMap;
        unordered_map<QString, int> intMap;
        QJsonObject stringMapObj = innerObj["stringMap"].toObject();
        for (auto strIt = stringMapObj.begin(); strIt != stringMapObj.end(); strIt++)
            stringMap[strIt.key()] = strIt.value().toString();
        QJsonObject intMapObj = innerObj["intMap"].toObject();
        for (auto intIt = intMapObj.begin(); intIt != intMapObj.end(); intIt++)
            intMap[intIt.key()] = intIt.value().toInt();

        soldierData.weaponMap[key] = make_pair(stringMap, intMap);
    }
    // Восстановление listButtonTexts и listAttackTexts
    QJsonArray listButtonTextsArray = soldierObj["listButtonTexts"].toArray();
    soldierData.listButtonTexts.clear();
    for (const auto& val : listButtonTextsArray)
        soldierData.listButtonTexts.push_back(val.toString());
    QJsonArray listAttackTextsArray = soldierObj["listAttackTexts"].toArray();
    soldierData.listAttackTexts.clear();
    for (const auto& val : listAttackTextsArray)
        soldierData.listAttackTexts.push_back(val.toString());

    Data data;
    // Восстановление данных Weapon
    #define ADD_FIELD_TO_VECT(fieldName, fieldType, toType) \
        if (weaponDataObj.contains(#fieldName)) { \
                fieldType fieldValue = weaponDataObj.value(#fieldName).to##toType(); \
                weaponData.vect_##fieldType.push_back(std::make_pair(#fieldName, fieldValue)); \
        }
    if (jsonObject.contains("Weapon")) {
        QJsonArray weaponArray = jsonObject["Weapon"].toArray();
        for (const auto& it : weaponArray) {
            WeaponToSerialize weaponData;
            QJsonObject weaponDataObj = it.toObject();
            // Проверка и добавление каждого поля, если ключ существует
            ADD_FIELD_TO_VECT(count, int, Int);
            ADD_FIELD_TO_VECT(id, int, Int);
            ADD_FIELD_TO_VECT(createNum, int, Int);
            ADD_FIELD_TO_VECT(pixmapX, int, Int);
            ADD_FIELD_TO_VECT(pixmapY, int, Int);
            ADD_FIELD_TO_VECT(shootSpeed, int, Int);
            ADD_FIELD_TO_VECT(reloadSpeed, int, Int);
            ADD_FIELD_TO_VECT(cleanSpeed, int, Int);
            ADD_FIELD_TO_VECT(cutsNum, int, Int);
            ADD_FIELD_TO_VECT(cuts_in_the_target, int, Int);
            ADD_FIELD_TO_VECT(residual_cuts, int, Int);
            ADD_FIELD_TO_VECT(shotsNum, int, Int);
            ADD_FIELD_TO_VECT(bullets_in_the_target, int, Int);
            ADD_FIELD_TO_VECT(pistolCapacity, int, Int);
            ADD_FIELD_TO_VECT(currentPistolCapacity, int, Int);
            ADD_FIELD_TO_VECT(ammoType, int, Int);
            ADD_FIELD_TO_VECT(bladeThick, int, Int);
            ADD_FIELD_TO_VECT(bladeWidth, int, Int);
            ADD_FIELD_TO_VECT(durability, int, Int);
            ADD_FIELD_TO_VECT(slashNum, int, Int);
            ADD_FIELD_TO_VECT(slashSpeed, int, Int);
            ADD_FIELD_TO_VECT(stabSpeed, int, Int);
            ADD_FIELD_TO_VECT(sharpSpeed, int, Int);

            ADD_FIELD_TO_VECT(sharp, bool, Bool);

            ADD_FIELD_TO_VECT(modelName, QString, String);
            ADD_FIELD_TO_VECT(material, QString, String);
            ADD_FIELD_TO_VECT(shootSound, QString, String);
            ADD_FIELD_TO_VECT(reloadSound, QString, String);
            ADD_FIELD_TO_VECT(cleanSound, QString, String);
            ADD_FIELD_TO_VECT(stabSound, QString, String);
            ADD_FIELD_TO_VECT(slashSound, QString, String);
            ADD_FIELD_TO_VECT(sharpSound, QString, String);
            ADD_FIELD_TO_VECT(img, QString, String);


            if (weaponDataObj.contains("widgetPos")) {
                QJsonObject widgetPosObj = weaponDataObj["widgetPos"].toObject();
                if (widgetPosObj.contains("X") && widgetPosObj.contains("Y"))
                    weaponData.vect_QPointF.push_back(make_pair("widgetPos", QPointF(widgetPosObj["X"].toInt(), widgetPosObj["Y"].toInt())));
            }

            // Восстановление данных info
            if (weaponDataObj.contains("info")) {
                QJsonArray infoArray = weaponDataObj["info"].toArray();
                weaponData.vect_info.clear(); // Очищаем вектор перед восстановлением
                for (const auto& infoValue : infoArray)
                    weaponData.vect_info.push_back(infoValue.toString());
            }
            if (weaponDataObj.contains("number"))
                weaponData.number = weaponDataObj.value("number").toInt();
            if (weaponDataObj.contains("true_number"))
                weaponData.true_number = weaponDataObj.value("true_number").toInt();
            data.dataWeapon.push_back(weaponData);
        }
    }
    data.dataMainWindow = mainWindowData;
    data.dataSoldier = soldierData;
    return data;
}

Serializer::~Serializer() {}

extern "C" __declspec(dllexport) QByteArray serialize_to_json_byte_array(const Data &data) {
    return Serializer::serialize_to_json_byte_array(data);
}

Data deserialize_from_json_byte_array(const QByteArray &byteArray)
{
    return Serializer::deserialize_from_json_byte_array(byteArray);
}
extern "C" __declspec(dllexport) QByteArray serialize_to_xml_byte_array(const Data &data) {
    return Serializer::serialize_to_xml_byte_array(data);
}

Data deserialize_from_xml_byte_array(const QByteArray &byteArray)
{
    return Serializer::deserialize_from_xml_byte_array(byteArray);
}
