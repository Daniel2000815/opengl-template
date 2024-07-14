#include "World.h"

World::World() {
    actors = std::vector<Actor*>{};
}

void World::AddActor(Actor* actor)
{
    actors.push_back(actor);
}
void World::Tick(float deltaTime) {
    for (Actor* actor : actors) {
        //actor->UpdateVelocity(actors, deltaTime);
        //actor->UpdatePosition(deltaTime);
        actor->tick(deltaTime);
    }
}
