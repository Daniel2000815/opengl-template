#ifndef __COLLIDER__H_
#define __COLLIDER__H_

#include <glm/gtc/matrix_transform.hpp>

using vec3 = glm::vec3;

enum ColliderType {
	BOX,
	PLANE,
	LINE,
	SPHERE,
	N
};

struct CollisionData {
	vec3 p1;
	vec3 p2;
	vec3 normal;
	float depth;
};

class Collider {
protected:
	ColliderType _type;

	Collider(ColliderType type) : _type(type) {}

public:
	ColliderType type() const { return _type; }
};

class SphereCollider : public Collider {
public:
	SphereCollider() : Collider(SPHERE) {}

private:
	vec3 _center;
	float _radius;
};

class PlaneCollider : public Collider {
public:
	PlaneCollider() : Collider(PLANE) {}

private:
	vec3 _normal;
	float _distance;
};

#endif