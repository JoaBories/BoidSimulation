#include "Util.h"
#include <random>

using Struct::Vec2F;
using Struct::Vec2I;
using Struct::Rect2;
using Struct::Collision;

#pragma region MathUtils

Vec2F Struct::vec2FromRot(const float rot)
{
	const Vec2F vector = { cosf(rot * DEG2RAD), sinf(rot * DEG2RAD) };
	return vector.normalized();
}

float Struct::overlapOnAxis(const std::vector<Vec2F>& a, const std::vector<Vec2F>& b, const Vec2F axis) // Check if two polygons (list of points) are overlaping on a certain axis | return overlap : negative -> false / positive -> true
{
	float aMin = FLT_MAX, aMax = -FLT_MAX;
	float bMin = FLT_MAX, bMax = -FLT_MAX;

	for (const auto& point : a) 
	{
		const float projection = point.dot(axis);
		aMin = min(projection, aMin);
		aMax = max(projection, aMax);
	}

	for (const auto& point : b)
	{
		const float projection = point.dot(axis);
		bMin = min(projection, bMin);
		bMax = max(projection, bMax);
	}

	return min(aMax, bMax) - max(aMin, bMin);
}

int Math::randInt(const int min, const int max)
{
	const int random = min + rand() % (min + max);

	return random;
}

float Math::randFloat(const float min, const float max)
{
	const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	return min + random * (max - min);
}

Vec2F Struct::trigToCoord(const float angle, const float radius)
{
	float x = radius * cos(angle);
	float y = radius * sin(angle);
	return { x,y };
}

bool Struct::pointInCircle(const Vec2F& point, const Vec2F& circleCenter, float circleRadius)
{
	const Vec2F distance = point - circleCenter;
	return distance.sqrLength() <= circleRadius * circleRadius;
}

#pragma endregion

#pragma region Structs

#pragma region Vectors

//Vec2F

const Vec2F Vec2F::Zero = { 0,0 };
const Vec2F Vec2F::One = { 1,1 };
const Vec2F Vec2F::Up = { 0,1 };
const Vec2F Vec2F::Down = { 0,-1 };
const Vec2F Vec2F::Right = { 1,0 };
const Vec2F Vec2F::Left = { -1,0 };

//Vec2I

const Vec2I Vec2I::Zero = { 0,0 };
const Vec2I Vec2I::One = { 1,1 };
const Vec2I Vec2I::Up = { 0,1 };
const Vec2I Vec2I::Down = { 0,-1 };
const Vec2I Vec2I::Right = { 1,0 };
const Vec2I Vec2I::Left = { -1,0 };

#pragma endregion

#pragma region Rectangle

std::vector<Vec2F> Rect2::getCorners() const
{
	std::vector<Vec2F> corners(4);

	const float cosA = cosf(rotation * DEG2RAD);
	const float sinA = sinf(rotation * DEG2RAD);

	const Vec2F right = { cosA, sinA };
	const Vec2F up = { -sinA, cosA };

	corners[0] = center + right * halfSize.x + up * halfSize.y;
	corners[1] = center + right * -halfSize.x + up * halfSize.y;
	corners[3] = center + right * halfSize.x + up * -halfSize.y;
	corners[2] = center + right * -halfSize.x + up * -halfSize.y;

	return corners;
}

Collision Rect2::checkAabb(const Rect2& other) const
{
	Collision result{};

	const Vec2F aMin = { center.x - halfSize.x, center.y - halfSize.y };
	const Vec2F aMax = { center.x + halfSize.x, center.y + halfSize.y };
	const Vec2F bMin = { other.center.x - other.halfSize.x, other.center.y - other.halfSize.y };
	const Vec2F bMax = { other.center.x + other.halfSize.x, other.center.y + other.halfSize.y };

	if (aMin.x <= bMax.x && aMax.x >= bMin.x && aMin.y <= bMax.y && aMax.y >= bMin.y)
	{
		result.collided = true;

		Vec2F overlap{};
		overlap.x = Math::min(aMax.x, bMax.x) - Math::max(aMin.x, bMin.x);
		overlap.y = Math::min(aMax.y, bMax.y) - Math::max(aMin.y, bMin.y);

		if (overlap.x < overlap.y) // Getting minimal overlap and his axis
		{
			result.overlap = overlap.x;
			result.axis = (center.x > other.center.x) ? Vec2F::Right : Vec2F::Left;
		}
		else
		{
			result.overlap = overlap.y;
			result.axis = (center.y > other.center.y) ? Vec2F::Up : Vec2F::Down;
		}
	}

	return result; // if AABB false collided -> true, axis -> {0,0} and overlap -> 0 by default
}

bool Rect2::containPoint(const Vec2F& point) const
{
	const Vec2F aMin = { center.x - halfSize.x, center.y - halfSize.y };
	const Vec2F aMax = { center.x + halfSize.x, center.y + halfSize.y };

	//Same as AABB but there is just onepoint so the max and the min are the same
	return point.x >= aMin.x && point.x <= aMax.x && point.y >= aMin.y && point.y <= aMax.y;
}

void Rect2::drawDebug(const float scale) const
{
	DrawCircleV(center.toRaylib(), scale, RED);

	std::vector<Vec2F> corners = getCorners();

	Vec2F& lastCorner = corners[3];

	for (const Vec2F& corner : corners)
	{
		DrawCircleV(corner.toRaylib(), scale * 0.5f, GREEN);
		DrawLineV(corner.toRaylib(), lastCorner.toRaylib(), GREEN);
		lastCorner = corner;
	}
}

Collision Rect2::checkObb(const Rect2& other) const
{
	Collision result{};

	if (rotation == 0.0f && other.rotation == 0.0f) // I can improve by using AABB for same rot rectangle or 90 rotated rectangles | I believe
	{
		result = checkAabb(other); // more optimized
	}
	else
	{
		const std::vector<Vec2F> aCorners = getCorners();
		const std::vector<Vec2F> bCorners = other.getCorners();

		std::vector<Vec2F> axes;

		axes.push_back((aCorners[0] - aCorners[1]).perpendicularCw());	// 2 axes is enough for a rectangle
		axes.push_back((aCorners[1] - aCorners[2]).perpendicularCw()); 

		axes.push_back((bCorners[0] - bCorners[1]).perpendicularCw());
		axes.push_back((bCorners[1] - bCorners[2]).perpendicularCw());

		for (const auto& axis : axes)									// Testing overlap on all axes. If there is overlap on all of them then there is collision
		{
			if (const float overlap = Struct::overlapOnAxis(aCorners, bCorners, axis); overlap < 0)
			{
				return result;
			}
			else if (overlap < result.overlap)							// Returning the minimal overlap and his axis 
			{
				result.axis = axis;
				result.overlap = overlap;
			}
		}

		result.collided = true;

	}

	return result;
}

#pragma endregion

#pragma endregion

