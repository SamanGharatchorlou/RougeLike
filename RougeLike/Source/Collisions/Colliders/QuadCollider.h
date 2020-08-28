#pragma once

#include "Collider.h"
#include "Utilities/Quad2D.h"


class QuadCollider : public Collider
{
public:
	QuadCollider() { }
	QuadCollider(Quad2D<float>* quad) : mQuad(quad) { }

	void init(Quad2D<float>* quad) { mQuad = quad; }

	bool doesIntersect(Collider* collider) const override;

#if TRACK_COLLISIONS
	void renderCollider() override;
#endif


private:
	const Quad2D<float>* mQuad;
};