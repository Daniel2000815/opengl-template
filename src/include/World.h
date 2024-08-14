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
    
    int _timeScale = 1;
    
    vec3 _gravity = vec3(0.0f);
    float _frictionCoefficient = 0.1f;
    float _angularFrictionCoefficient = 0.02f;

    void solveDynamics(float delta);
    void solveCollisions();
    
public:
    static constexpr double G = 1e-11 * 6.673;

    World(Window* window);

    void tick(float deltaTime);

    std::vector<Actor*> actors() const { return _actors; }
    void addActor(Actor* actor);

    template<typename... Actors>
    void addActors(Actors... actors) {
        (addActor(actors), ...);
    }

    int timeScale() const { return _timeScale; };
    void setTimeScale(int scale) { _timeScale = scale; };
    
    vec3 gravity() const { return _gravity; }
    void setGravity(vec3 gravity) { _gravity = gravity; }
    
};

#endif