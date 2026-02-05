#pragma once

/*
concept InputStrategyT = requires(T a)
{
    { a.inputState() } -> InputStateType;
}
*/
template <typename InputStrategyT>
class InputComponent
{
public:
    InputComponent(InputStrategyT inputStrategy)
        : mInputStrategy(std::move(inputStrategy))
    {
    }

    auto inputState() const
    {
        return mInputStrategy.inputState();
    }

private:
    InputStrategyT mInputStrategy{};
};