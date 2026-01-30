#pragma once

#include <utility>
#include "Vector2.h"
#include "Vector3.h"

class Entity
{
public:
    Entity() : mEntityId(GetNextEntityId()) {}
    Entity(const Entity &other)
        : mEntityId(GetNextEntityId()), mPosition(other.mPosition), mRotation(other.mRotation) {}
    Entity &operator=(const Entity &other)
    {
        mPosition = other.mPosition;
        mRotation = other.mRotation;
        return *this;
    }
    Entity(Entity &&other) noexcept
        : mEntityId(other.mEntityId), mPosition(std::move(other.mPosition)), mRotation(std::move(other.mRotation))
    {
        other.mEntityId = 0;
    }
    Entity &operator=(Entity &&other) noexcept
    {
        mEntityId = other.mEntityId;
        mPosition = std::move(other.mPosition);
        mRotation = std::move(other.mRotation);
        other.mEntityId = 0;
        return *this;
    }
    ~Entity() = default;

    [[nodiscard]] Math::Vector3 getPosition() const { return mPosition; }
    [[nodiscard]] Math::Vector2 getRotation() const { return mRotation; }
    [[nodiscard]] int getEntityId() const { return mEntityId; }
    [[nodiscard]] bool isValid() const { return mEntityId != 0; }

    void setPosition(const Math::Vector3 &pos) { mPosition = pos; }
    void setRotation(const Math::Vector2 &rot) { mRotation = rot; }

private:
    static int GetNextEntityId()
    {
        static int nextId = 1;
        return nextId++;
    }

    int mEntityId;
    Math::Vector3 mPosition{};
    Math::Vector2 mRotation{};
};