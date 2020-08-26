#pragma once

#include "Collider.h"
#include "Utilities/Quad2D.h"


class QuadCollider : public Collider
{
public:
	QuadCollider(const Quad2D<float>& quad) : mQuad(quad) { }
	void init(const Quad2D<float>& quad) { mQuad = quad; }

	bool doesIntersect(Collider* collider) const override;


private:
	Quad2D<float> mQuad;
};