///
/// @file MultiplicationModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef MULTIPLICATION_MODEL_HXX_A4F1FD7BC9A9
#define MULTIPLICATION_MODEL_HXX_A4F1FD7BC9A9

#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>
#include <QtWidgets/QLabel>

#include "DecimalData.hxx"
#include "MathOperationDataModel.hxx"

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class MultiplicationModel : public MathOperationDataModel
{
public:
    virtual ~MultiplicationModel() {}

    QString caption() const override
    {
        return QStringLiteral("Multiplication");
    }

    QString name() const override
    {
        return QStringLiteral("Multiplication");
    }

private:
    void compute() override
    {
        const PortIndex outPortIndex = 0;

        auto n1 = _number1.lock();
        auto n2 = _number2.lock();

        if (n1 && n2)
        {
            // modelValidationState = NodeValidationState::Valid;
            // modelValidationError = QString();
            _result
                = std::make_shared<DecimalData>(n1->number() * n2->number());
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

#endif  // MULTIPLICATION_MODEL_HXX_A4F1FD7BC9A9
