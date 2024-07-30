#ifndef __COLLIDER__H_
#define __COLLIDER__H_

#include <glm/gtc/matrix_transform.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;

enum ColliderType {
	BOX,
	PLANE,
	LINE,
	SPHERE,
	N
};

// Data for checking collision of A into B
struct CollisionData {
	vec3 p1;		// Deepest point of A into B	
	vec3 p2;		// Deepest point of B into A
	vec3 normal;	
	float depth;
	bool hit;

	CollisionData() : p1(0.0f), p2(0.0f), normal(0.0f), depth(0.0f), hit(false) {}
	CollisionData(vec3 p1, vec3 p2) {
		this->p1 = p1;
		this->p2 = p2;
		vec3 dir = p2 - p1;

		normal = glm::normalize(dir);
		depth = dir.length();
		hit = true;
	}
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
	SphereCollider(vec3 c, float r) : Collider(SPHERE), _center(c), _radius(r) {}

private:
	vec3 _center;
	float _radius;

public:
	vec3 center() const { return _center; }
	float radius() const { return _radius; }
};

class PlaneCollider : public Collider {
public:
	PlaneCollider() : Collider(PLANE) {}

private:
	vec3 _normal;
	float _distance;
};

#endif