#include "pch.h"
#include "QuadCollider.h"

#if TRACK_COLLISIONS
#include "Debug/DebugDraw.h"
#endif


bool QuadCollider::doesIntersect(Collider* collider) const
{
	RectF thatRect = collider->scaledRect();
	Quad2D<float> thatQuad(thatRect);

	// Assuming 'thatRect' is orientated on the x/y axis, test for overlaps
	bool xOverlap = test1DOverlap(mQuad->xMin(), mQuad->xMax(), thatRect.LeftPoint(), thatRect.RightPoint());
	bool yOverlap = test1DOverlap(mQuad->yMin(), mQuad->yMax(), thatRect.TopPoint(), thatRect.BotPoint());

	if (!xOverlap || !yOverlap)
		return false;

	// [quad][min,max]
	float projections[2][2];

	// No need to use the normal_hat vectors
	VectorF normals[2]{ mQuad->normal0(), mQuad->normal1() };
	const Quad2D<float>* quads[2]{ mQuad, &thatQuad };

	// Project onto quad normals
	for (int n = 0; n < 2; n++)
	{
		for (int q = 0; q < 2; q++)
		{
			float projection = quads[q]->at(0).dotProduct(normals[n]);
			float minProjection = projection;
			float maxProjection = projection;
			for (int i = 1; i < quads[q]->sides(); i++)
			{
				projection = quads[q]->at(i).dotProduct(normals[n]);

				if (projection < minProjection)
					minProjection = projection;

				if (projection > maxProjection)
					maxProjection = projection;
			}

			projections[q][0] = minProjection;
			projections[q][1] = maxProjection;
		}

		bool nOverlap = test1DOverlap(projections[0][0], projections[0][1], projections[1][0], projections[1][1]);

		if (!nOverlap)
			return false;
	}

	return true;
}


float QuadCollider::xMin() const
{
	return mQuad->xMin();
}


float QuadCollider::xMax() const
{
	return mQuad->xMax();
}



#if TRACK_COLLISIONS
void QuadCollider::renderCollider()
{
	if (didHit())
	{
		debugDrawQuad(*mQuad, RenderColour::Blue);
	}
	else if (gotHit())
	{
		debugDrawQuad(*mQuad, RenderColour::Red);
	}
	else
	{
		debugDrawQuad(*mQuad, RenderColour::LightGrey);
	}
}
#endif