#ifndef __WORLD__H_
#define __WORLD__H_

#include <vector>
#include <ctime>
#include <chrono>
#include <Window.h>
#include <Actors/Actor.h>
#include <Physics/CollisionSolver.h>

class World {
private:
    Window* _window;
    CollisionSolver* _collisionSolver;
    std::vector<Actor*> _actors;
    int _physicsFPS = 144;
    int _timeScale = 1;
    float _physicsStep = 1.0f / _physicsFPS;
    std::chrono::high_resolution_clock::time_point _t1, _t2;
    bool _canContinue = true;

    void solveDynamics(float delta);
    void solveCollisions();

    
public:
    static constexpr float G = 1e-11 * 6.673f;

    World(Window* window);
    void tick(float deltaTime);

    void addActor(Actor* actor);

    void setPhysicsFPS(int fps) { _physicsFPS = fps; _physicsStep = 1.0f / fps; };
    int physicsFPS() { return _physicsFPS; };
    void setTimeScale(int scale) { _timeScale = scale; };
    int timeScale() { return _timeScale; };
    std::vector<Actor*> actors() { return _actors; }
};

#endif