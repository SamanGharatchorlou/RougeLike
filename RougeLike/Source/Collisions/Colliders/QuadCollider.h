#pragma once

#include "Collider.h"


class QuadCollider : public Collider
{
public:
	QuadCollider() { }
	QuadCollider(Quad2D<float>* quad) : mQuad(quad) { }

	void init(Quad2D<float>* quad) { mQuad = quad; }

	bool doesIntersect(Collider* collider) const override;

	const Quad2D<float>& getQuad() const { return *mQuad;}
	RectF scaledRect() const;

	float xMin() const override;
	float xMax() const override;

#if TRACK_COLLISIONS
	void renderCollider() override;
#endif


private:
	const Quad2D<float>* mQuad;
};