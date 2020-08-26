#include "pch.h"
#include "QuadCollider.h"


bool QuadCollider::doesIntersect(Collider* collider) const
{
	RectF thatRect = collider->scaledRect();
	Quad2D<float> thatQuad(thatRect);


	bool xOverlaps = mQuad.xMax() > thatRect.LeftPoint() && mQuad.xMin() < thatRect.RightPoint();
	bool yOverlaps = mQuad.yMin() < thatRect.BotPoint() && mQuad.yMax() > thatRect.TopPoint();

	// normal / quad / min,max
	float projections[2][2][2];

	VectorF normals[2]{ mQuad.normal0(), mQuad.normal1() };
	Quad2D<float> quads[2]{ mQuad, thatQuad };


	for (int n = 0; n < 2; n++)
	{
		for (int q = 0; q < 2; q++)
		{
			float minProjection = quads[q].at(0).dotProduct(normals[n]);
			for (int i = 1; i < quads[q].sides(); i++)
			{
				float projection = quads[q].at(i).dotProduct(normals[n]);

				if (projection < minProjection)
					minProjection = projection;
			}

			float maxProjection = quads[q].at(0).dotProduct(normals[n]);
			for (int i = 1; i < quads[q].sides(); i++)
			{
				float projection = quads[q].at(i).dotProduct(normals[n]);

				if (projection > maxProjection)
					maxProjection = projection;
			}

			projections[n][q][0] = minProjection;
			projections[n][q][1] = maxProjection;
		}
	}

	bool pOverlap = projections[0][0][1] > projections[0][1][0] && projections[0][0][0] < projections[0][1][1];
	bool qOverlap = projections[1][0][1] > projections[1][1][0] && projections[1][0][0] < projections[1][1][1];
	return xOverlaps && yOverlaps && pOverlap && qOverlap;
}