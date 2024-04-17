///
/// @file DivisionModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef DIVISION_MODEL_HXX_F209DF6921CE
#define DIVISION_MODEL_HXX_F209DF6921CE

#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>
#include <QtWidgets/QLabel>

#include "DecimalData.hxx"
#include "MathOperationDataModel.hxx"

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class DivisionModel : public MathOperationDataModel
{
public:
    virtual ~DivisionModel() {}

    QString caption() const override
    {
        return QStringLiteral("Division");
    }

    bool portCaptionVisible(PortType portType, PortIndex portIndex)
        const override
    {
        Q_UNUSED(portType);
        Q_UNUSED(portIndex);
        return true;
    }

    QString portCaption(PortType portType, PortIndex portIndex) const override
    {
        switch (portType)
        {
        case PortType::In:
            if (portIndex == 0)
            {
                return QStringLiteral("Dividend");
            }
            else if (portIndex == 1)
            {
                return QStringLiteral("Divisor");
            }

            break;

        case PortType::Out: return QStringLiteral("Result");

        default: break;
        }
        return QString();
    }

    QString name() const override
    {
        return QStringLiteral("Division");
    }

private:
    void compute() override
    {
        const PortIndex outPortIndex = 0;

        auto n1 = _number1.lock();
        auto n2 = _number2.lock();

        if (n2 && (n2->number() == 0.0))
        {
            // modelValidationState = NodeValidationState::Error;
            // modelValidationError = QStringLiteral("Division by zero error");
            _result.reset();
        }
        else if (n1 && n2)
        {
            // modelValidationState = NodeValidationState::Valid;
            // modelValidationError = QString();
            _result
                = std::make_shared<DecimalData>(n1->number() / n2->number());
        }
        else
        {
            // modelValidationState = NodeValidationState::Warning;
            // modelValidationError = QStringLiteral("Missing or incorrect
            // inputs");
            _result.reset();
        }

        Q_EMIT dataUpdated(outPortIndex);
    }
};

#endif  // DIVISION_MODEL_HXX_F209DF6921CE