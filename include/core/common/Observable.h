#pragma once

#include <set>
#include "Observer.h"

template <typename T>
class Observable
{
public:
    bool attach(Observer<T> *observer)
    {
        auto [pos, success] = mObservers.insert(observer);
        return success;
    }

    bool detach(Observer<T> *observer)
    {
        return mObservers.erase(observer) > 0U;
    }

    void detachAll()
    {
        mObservers.clear();
    }

    void notify()
    {
        for (auto *observer : mObservers)
        {
            observer->update(static_cast<const T &>(*this));
        }
    }

private:
    std::set<Observer<T> *> mObservers{};
};