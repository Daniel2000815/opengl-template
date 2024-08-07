#ifndef __COLLIDER__H_
#define __COLLIDER__H_

#include <glm/gtc/matrix_transform.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;

enum ColliderType {
	BOX,
	LINE,
	SPHERE,
	N
};

// Data for checking collision of A into B
struct CollisionData {
	vec3 p1;		// Deepest point of A into B	
	vec3 p2;		// Deepest point of B into A
	vec3 normal;
	vec3 mtv;
	float depth;
	bool hit;

	CollisionData() : p1(0.0f), p2(0.0f), normal(0.0f), mtv(0.0f), depth(0.0f), hit(false) {}
	CollisionData(vec3 p1, vec3 p2, vec3 mtv) {
		this->p1 = p1;
		this->p2 = p2;
		this->mtv = mtv;
		vec3 dir = p2 - p1;

		normal = glm::normalize(dir);
		//printf("Normal2: (%f, %f, %f)\n", normal.x, normal.y, normal.z);
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
	virtual ~Collider() {} // Destructor virtual
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

class BoxCollider : public Collider {
public:
	BoxCollider() : Collider(BOX), _center(0.0f), _dimensions(0.0f) {}
	BoxCollider(vec3 center, vec3 dimensions) : Collider(BOX), _center(center), _dimensions(dimensions) {}

private:
	vec3 _center;
	vec3 _dimensions;
};

#endif