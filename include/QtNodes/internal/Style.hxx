///
/// @file Style.hpp
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef STYLE_HXX_92A1829EAA1E
#define STYLE_HXX_92A1829EAA1E

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace QtNodes {

class Style  // : public QObject
{
    // Q_OBJECT

public:
    virtual ~Style() = default;

public:
    virtual void loadJson(const QJsonObject& json) = 0;

    virtual QJsonObject toJson() const = 0;

    ///
    /// @brief Loads from utf-8 byte array.
    ///
    /// @param byteArray
    ///
    virtual void loadJsonFromByteArray(const QByteArray& byteArray)
    {
        auto json = QJsonDocument::fromJson(byteArray).object();

        loadJson(json);
    }

    virtual void loadJsonText(QString jsonText)
    {
        loadJsonFromByteArray(jsonText.toUtf8());
    }

    virtual void loadJsonFile(QString fileName)
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Couldn't open file " << fileName;

            return;
        }

        loadJsonFromByteArray(file.readAll());
    }
};

}  // namespace QtNodes

#endif  // STYLE_HXX_92A1829EAA1E
