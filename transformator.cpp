#include "transformator.h"

Transformator::Transformator() {}
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
QByteArray Transformator::fromXmlToJson(const QByteArray &byteArray)
{
    QXmlStreamReader xml(byteArray);
    QJsonObject jsonProgram;
    QJsonObject jsonMainWindow;
    QJsonObject jsonSoldier;
    QJsonArray jsonWeapon;
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "MainWindow") {
                QJsonObject jsonWidgetPos;
                QJsonArray infoArray;
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "MainWindow")) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString name = xml.name().toString();
                        xml.readNext();
                        QString value = xml.text().trimmed().toString();
                        if (!value.isEmpty()) {
                            if (name == "ChooseWeapon_curr_index") {
                                jsonMainWindow["ChooseWeapon_curr_index"] = value.toInt();
                            } else if (name == "WeaponList_curr_index") {
                                jsonMainWindow["WeaponList_curr_index"] = value.toInt();
                            } else if (name == "target_choose_curr_index") {
                                jsonMainWindow["target_choose_curr_index"] = value.toInt();
                            } else if (name == "remaining_time") {
                                jsonMainWindow["remaining_time"] = value.toInt();
                            } else if (name == "time") {
                                jsonMainWindow["time"] = value.toInt();
                            } else if (name == "action") {
                                jsonMainWindow["action"] = (value == "true");
                            } else if (name == "fl") {
                                jsonMainWindow["fl"] = (value == "true");
                            } else if (name == "isMouseRelease") {
                                jsonMainWindow["isMouseRelease"] = (value == "true");
                            } else if (name == "moveDown") {
                                jsonMainWindow["moveDown"] = (value == "true");
                            } else if (name == "moveRight") {
                                jsonMainWindow["moveRight"] = (value == "true");
                            } else if (name == "pixmapX") {
                                jsonMainWindow["pixmapX"]= value.toInt();
                            } else if (name == "pixmapY") {
                                jsonMainWindow["pixmapY"] = value.toInt();
                            } else if (name == "speed") {
                                jsonMainWindow["speed"] = value.toInt();
                            } else if (name == "X") {
                                jsonWidgetPos["X"] = value.toInt();
                            } else if (name == "Y") {
                                jsonWidgetPos["Y"] = value.toInt();
                            } else if (name == "item") {
                                infoArray.append(value);
                            }
                        }
                    }
                    xml.readNext();
                }
                jsonMainWindow["widgetPos"] = jsonWidgetPos;
                jsonMainWindow["info"] = infoArray;
                jsonProgram["MainWindow"] = jsonMainWindow;
            } else if (xml.name().toString() == "Soldier") {
                QJsonArray listButtonTextsArray;
                QJsonArray listAttackTextsArray;
                QJsonObject json_weaponMap;
                unordered_map<QString, pair<unordered_map<QString, QString>, unordered_map<QString, int>>> weaponMap;
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "Soldier")) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString name = xml.name().toString();
                        xml.readNext();
                        QString value = xml.text().trimmed().toString();
                        if (!value.isEmpty()) {
                            if (name == "itemAT") {
                                listAttackTextsArray.append(value);
                            } else if (name == "itemBT") {
                                listButtonTextsArray.append(value);
                            } else if (name == "isAttack") {
                                jsonSoldier["isAttack"] = (value == "true");
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
                            weaponMap[wep_type] = make_pair(stringMap, intMap);
                        }
                    }
                    xml.readNext();
                }
                json_weaponMap = serializeWeaponMapJson(weaponMap);
                jsonSoldier["weaponMap"] = json_weaponMap;
                jsonSoldier["listButtonTexts"] = listButtonTextsArray;
                jsonSoldier["listAttackTexts"] = listAttackTextsArray;
                jsonProgram["Soldier"] = jsonSoldier;
            } else if (xml.name().toString() == "Weapon") {
                QJsonArray jsonWeapon;
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "Weapon")) {
                    QJsonArray weaponArray;
                    QJsonObject jsonObj;
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        QString name;
                        QString value;
                        while (name != "vect_int") {
                            if (xml.isStartElement()) {
                                name = xml.name().toString();
                                xml.readNext();
                                if (xml.isCharacters()) {
                                    value = xml.text().trimmed().toString();
                                }
                            } else {
                                xml.readNext();
                            }
                        }
                        if (name == "vect_int") {
                            while (name != "vect_bool") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (!value.isEmpty()) {
                                    jsonObj[name] = value;
                                }
                            }
                        }
                        if (name == "vect_bool") {
                            while (name != "vect_QString") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (!value.isEmpty()) {
                                    jsonObj[name] = value;
                                }
                            }
                        }
                        if (name == "vect_QString") {
                            while (name != "vect_QPointF") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (!value.isEmpty()) {
                                    jsonObj[name] = value;
                                }
                            }
                        }
                        if (name == "vect_QPointF") {
                            QJsonObject jsonWidgetPos;
                            while (name != "number") {
                                while (name != "X" && name != "number") {
                                    name = xml.name().toString();
                                    xml.readNext();
                                    value = xml.text().trimmed().toString();
                                }
                                if (name == "number")
                                    break;
                                jsonWidgetPos["X"] = value.toInt();
                                while (name != "Y" && name != "number") {
                                    name = xml.name().toString();
                                    xml.readNext();
                                    value = xml.text().trimmed().toString();
                                }
                                if (name == "number")
                                    break;
                                jsonWidgetPos["Y"] = value.toInt();
                                jsonObj["widgetPos"] = jsonWidgetPos;
                            }
                        }
                        if (name == "number") {
                            jsonObj["number"] = value.toInt();
                            while (name != "true_number") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                            }
                        }
                        if (name == "true_number") {
                            jsonObj["true_number"] = value.toInt();
                            while (name != "info") {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                            }
                        }
                        if (name == "info") {
                            QJsonArray infoArray;
                            while ( !(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "info") ) {
                                name = xml.name().toString();
                                xml.readNext();
                                value = xml.text().trimmed().toString();
                                if (name == "item" && !value.isEmpty())
                                    infoArray.append(value);
                            }
                            jsonObj["info"] = infoArray;
                        }
                        jsonWeapon.append(jsonObj);
                    }
                    xml.readNext();
                }
                jsonProgram["Weapon"] = jsonWeapon;
            }
        }
    }

    if (xml.hasError()) {
        qDebug() << "XML error: " << xml.errorString();
    }

    QJsonDocument jsonDoc(jsonProgram);
    return jsonDoc.toJson();
}

QByteArray Transformator::fromJsonToXml(const QByteArray &byteArray)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoc.object();
    QJsonObject mainObj = jsonObject["MainWindow"].toObject();
    QByteArray bArray;
    QXmlStreamWriter xmlProgram(&bArray);
    xmlProgram.setAutoFormatting(true);
    xmlProgram.writeStartElement("Program");
    xmlProgram.writeStartElement("MainWindow");
    xmlProgram.writeTextElement("ChooseWeapon_curr_index", QString::number(mainObj["ChooseWeapon_curr_index"].toInt()));
    xmlProgram.writeTextElement("WeaponList_curr_index", QString::number(mainObj["WeaponList_curr_index"].toInt()));
    xmlProgram.writeTextElement("target_choose_curr_index", QString::number(mainObj["target_choose_curr_index"].toInt()));
    xmlProgram.writeTextElement("remaining_time", QString::number(mainObj["remaining_time"].toInt()));
    xmlProgram.writeTextElement("time", QString::number(mainObj["time"].toInt()));
    xmlProgram.writeTextElement("action", mainObj["action"].toBool() ? "true" : "false");
    xmlProgram.writeTextElement("fl", mainObj["fl"].toBool()? "true" : "false");
    xmlProgram.writeTextElement("isMouseRelease", mainObj["isMouseRelease"].toBool() ? "true" : "false");
    xmlProgram.writeTextElement("moveDown", mainObj["moveDown"].toBool() ? "true" : "false");
    xmlProgram.writeTextElement("moveRight", mainObj["moveRight"].toBool() ? "true" : "false");
    xmlProgram.writeTextElement("pixmapX", QString::number(mainObj["pixmapX"].toInt()));
    xmlProgram.writeTextElement("pixmapY", QString::number(mainObj["pixmapY"].toInt()));
    xmlProgram.writeTextElement("speed", QString::number(mainObj["speed"].toInt()));
    xmlProgram.writeStartElement("widgetPos");
    xmlProgram.writeTextElement("X", QString::number(mainObj["widgetPos"].toObject()["X"].toInt()));
    xmlProgram.writeTextElement("Y", QString::number(mainObj["widgetPos"].toObject()["Y"].toInt()));
    xmlProgram.writeEndElement(); // Close widgetPos
    xmlProgram.writeStartElement("info");
    if (mainObj.contains("info")) {
        QJsonArray infoArray = mainObj["info"].toArray();
        for (const auto& info : infoArray)
            xmlProgram.writeTextElement("item", info.toString());
    }
    xmlProgram.writeEndElement();
    xmlProgram.writeEndElement(); // Close MainWindow
    QJsonObject soldierObj = jsonObject["Soldier"].toObject();
    xmlProgram.writeStartElement("Soldier");
    QJsonArray listButtonTextsArray = soldierObj["listButtonTexts"].toArray();
    QJsonArray listAttackTextsArray = soldierObj["listAttackTexts"].toArray();
    xmlProgram.writeStartElement("listButtonTexts");
    for (const auto& val : listButtonTextsArray)
        xmlProgram.writeTextElement("itemBT", val.toString());
    xmlProgram.writeEndElement(); // Close listButtonTexts
    xmlProgram.writeStartElement("listAttackTexts");
    for (const auto& val : listButtonTextsArray)
        xmlProgram.writeTextElement("itemAT", val.toString());
    xmlProgram.writeEndElement(); // Close listAttackTexts

    // Восстановление weaponMap
    unordered_map<QString, pair<unordered_map<QString, QString>, unordered_map<QString, int>>> weaponMap;
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

        weaponMap[key] = make_pair(stringMap, intMap);
    }
    serializeWeaponMapXml(weaponMap, xmlProgram);

    xmlProgram.writeTextElement("isAttack", soldierObj["isAttack"].isBool() ? "true" : "false");
    xmlProgram.writeEndElement(); // Close Soldier
// Восстановление данных Weapon
#define ADD_FIELD_TO_VECT(fieldName, fieldType, toType) \
    if (weaponDataObj.contains(#fieldName)) { \
            fieldType fieldValue = weaponDataObj.value(#fieldName).to##toType(); \
            weaponData.vect_##fieldType.push_back(std::make_pair(#fieldName, fieldValue)); \
    }
    vector<WeaponToSerialize> dataWeapon;
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
            dataWeapon.push_back(weaponData);
        }
    }
    if (dataWeapon.size()) {
        xmlProgram.writeStartElement("Weapon");
        for (int i = 0; i < dataWeapon.size(); i++) {
            xmlProgram.writeStartElement("wepItem");
            xmlProgram.writeStartElement("vect_int");
            for (int j = 0; j < dataWeapon.at(i).vect_int.size(); j++) {
                xmlProgram.writeTextElement(dataWeapon.at(i).vect_int.at(j).first,
                                            QString::number(dataWeapon.at(i).vect_int.at(j).second));
            }
            xmlProgram.writeEndElement();
            xmlProgram.writeStartElement("vect_bool");
            for (int j = 0; j < dataWeapon.at(i).vect_bool.size(); j++) {
                xmlProgram.writeTextElement(dataWeapon.at(i).vect_bool.at(j).first,
                                            dataWeapon.at(i).vect_bool.at(j).second ? "true" : "false");
            }
            xmlProgram.writeEndElement();
            xmlProgram.writeStartElement("vect_QString");
            for (int j = 0; j < dataWeapon.at(i).vect_QString.size(); j++) {
                xmlProgram.writeTextElement(dataWeapon.at(i).vect_QString.at(j).first, dataWeapon.at(i).vect_QString.at(j).second);
            }
            xmlProgram.writeEndElement();
            for (int j = 0; j < dataWeapon.at(i).vect_QPointF.size(); j++) {
                xmlProgram.writeStartElement("vect_QPointF");
                xmlProgram.writeTextElement("X", QString::number(dataWeapon.at(i).vect_QPointF.at(j).second.x()));
                xmlProgram.writeTextElement("Y", QString::number(dataWeapon.at(i).vect_QPointF.at(j).second.y()));
                xmlProgram.writeEndElement();
            }
            xmlProgram.writeTextElement("number", QString::number(dataWeapon.at(i).number));
            xmlProgram.writeTextElement("true_number", QString::number(dataWeapon.at(i).true_number));

            xmlProgram.writeStartElement("info");
            for (const QString& info : dataWeapon.at(i).vect_info)
                xmlProgram.writeTextElement("item", info);
            xmlProgram.writeEndElement();
            xmlProgram.writeEndElement();
        }
        xmlProgram.writeEndElement();
    }

    xmlProgram.writeEndElement();

    return bArray;
}

QByteArray fromXmlToJson(const QByteArray &byteArray)
{
    return Transformator::fromXmlToJson(byteArray);
}

QByteArray fromJsonToXml(const QByteArray &byteArray)
{
    return Transformator::fromJsonToXml(byteArray);
}
