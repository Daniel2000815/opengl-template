#ifndef __WORLD__H_
#define __WORLD__H_

#include <vector>
#include <ctime>
#include <chrono>
#include <Actor.h>

class World {
private:
    std::vector<Actor*> actors;
    int physicsFPS = 144;
    int timeScale = 1;
    float physicsStep = 1.0f / physicsFPS;
    std::chrono::high_resolution_clock::time_point t1, t2;
    bool canContinue = true;
public:
    static constexpr float G = 1e-11 * 6.673f;

    World();
    void Tick(float deltaTime);

    void AddActor(Actor* actor);

    void SetPhysicsFPS(int fps) { physicsFPS = fps; physicsStep = 1.0f / fps; };
    int GetPhysicsFPS() { return physicsFPS; };
    void SetTimeScale(int scale) { timeScale = scale; };
    int GetTimeScale() { return timeScale; };
    std::vector<Actor*> getActors() { return actors; }
};

#endif