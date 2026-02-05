#pragma once

#include <type_traits>

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
        if constexpr (std::is_pointer_v<InputStrategyT>)
        {
            return mInputStrategy->inputState();
        }
        else
        {
            return mInputStrategy.inputState();
        }
    }

private:
    InputStrategyT mInputStrategy{};
};