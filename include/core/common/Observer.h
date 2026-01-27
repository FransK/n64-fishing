#pragma once

#include <functional>

template <typename Subject>
class Observer
{
public:
    using OnUpdate = std::function<void(Subject const &)>;

    explicit Observer(OnUpdate onUpdate)
        : mOnUpdate{std::move(onUpdate)}
    {
    }

    void update(Subject const &subject)
    {
        mOnUpdate(subject);
    }

private:
    OnUpdate mOnUpdate;
};