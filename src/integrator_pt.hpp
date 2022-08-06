#pragma once
#include "integrator.hpp"

namespace specter {

class PTIntegrator : public IIntegrator {

public:

	PTIntegrator(int pathDepth)
		: pathDepth(pathDepth)
	{}

	virtual vec3f render(const Accel& accel, const Ray& ray) override;

protected:

	vec3f render_rec(const Accel& accel, const Ray& ray, int traversal_depth);

protected:

	int pathDepth;
};

}