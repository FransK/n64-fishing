#pragma once

#include <memory>
#include "containers/Vector.h"

#include "Collider.h"
#include "Epa.h"

#include "common/Observer.h"

class Player;

namespace Collision
{
    class CollisionScene : Observer<Player>
    {
    public:
        CollisionScene() : Observer<Player>([this](const Player &player)
                                            { this->onPlayerStateChange(player); }) {};
        ~CollisionScene() = default;
        CollisionScene(const CollisionScene &) = delete;
        CollisionScene &operator=(const CollisionScene &) = delete;
        CollisionScene(CollisionScene &&) = delete;
        CollisionScene &operator=(CollisionScene &&) = delete;

        void add(std::unique_ptr<Collider> collider, bool isActive = true);
        void remove(int entityId);
        void activate(int entityId);
        void deactivate(int entityId);

        void update(float fixedTimeStep);
        void debugDraw() const;

    private:
        Containers::vector<std::unique_ptr<Collider>> mInactiveColliders{};
        Containers::vector<std::unique_ptr<Collider>> mActiveColliders{};

        void runCollision();
        void constrainToWorld(const std::unique_ptr<Collider> &object);
        void collide(const std::unique_ptr<Collider> &a, const std::unique_ptr<Collider> &b);
        void correctOverlap(const std::unique_ptr<Collider> &object, EpaResult *result, float ratio, float friction, float bounce);
        void correctVelocity(const std::unique_ptr<Collider> &object, EpaResult *result, float ratio, float friction, float bounce);

        void onPlayerStateChange(const Player &player);
    };
}