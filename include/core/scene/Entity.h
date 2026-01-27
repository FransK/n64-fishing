#pragma once

#include "Vector2.h"
#include "Vector3.h"

static int NextEntityId = 1;

class Entity
{
public:
    Entity() : mEntityId(GetNextEntityId()) {}

    [[nodiscard]] Math::Vector3 getPosition() const { return mPosition; }
    [[nodiscard]] Math::Vector2 getRotation() const { return mRotation; }
    [[nodiscard]] int getEntityId() const { return mEntityId; }

    void setPosition(const Math::Vector3 &pos) { mPosition = pos; }
    void setRotation(const Math::Vector2 &rot) { mRotation = rot; }

private:
    static int GetNextEntityId()
    {
        return NextEntityId++;
    }

    int mEntityId;
    Math::Vector3 mPosition{};
    Math::Vector2 mRotation{};
};