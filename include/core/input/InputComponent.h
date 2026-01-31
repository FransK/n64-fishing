#pragma once

template <typename InputStrategyT, typename UpdateStrategyT>
class InputComponent
{
public:
    InputComponent(InputStrategyT inputStrategy, UpdateStrategyT updateStrategy)
        : mInputStrategy(std::move(inputStrategy)),
          mUpdateStrategy(std::move(updateStrategy))
    {
    }

    void update(float deltaTime);

private:
    InputStrategyT mInputStrategy{};
    UpdateStrategyT mUpdateStrategy{};
};

template <typename InputStrategyT, typename UpdateStrategyT>
void InputComponent<InputStrategyT, UpdateStrategyT>::update(float deltaTime)
{
    mUpdateStrategy.update(mInputStrategy.getInputState(), deltaTime);
}