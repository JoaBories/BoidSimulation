#pragma once
#include "raylib.h"

#include <iostream>
#include <vector>

// Note : 
// -rm for right member
// -I use rotation in degrees (unless otherwise specified)
// -I use float (unless otherwise specified)

namespace Math
{
	template <typename T>
	constexpr T min(T a, T b) noexcept											{ return a <= b ? a : b; };

	template <typename T>
	constexpr T max(T a, T b) noexcept											{ return a >= b ? a : b; };
	
	template <typename T>
	constexpr T clamp(T value, T minimum, T maximum) noexcept					{ return max(min(value, maximum), minimum); }
	
	template <typename T>
	constexpr T abs(T value) noexcept											{ return (value < 0) ? -value : value; }
	
	template <typename T>
	constexpr T sign(T value) noexcept											{ return value > 0 ? T(1) : value < 0 ? T(-1) : T(0); }

	template <typename T>
	constexpr T lerp(T a, T b, const float t) noexcept							{ return a + (T)((float)(b - a) * clamp(t, 0.0f, 1.0f)); }
	
	template <typename T>
	constexpr bool nearlyEqual(const T a, const T b, const T epsilon = static_cast<T>(0.00001f)) noexcept	{ return abs( a - b ) < epsilon; }
	
	int randInt(int min, int max);
	float randFloat(float min, float max);
}

//Vector2 with a lot of function and operators for easy use | Int version if needed for integral coordinates
template<typename T>
struct Vec2
{
	T x;
	T y;

	static const Vec2 Zero;
	static const Vec2 One;
	static const Vec2 Up;
	static const Vec2 Down;
	static const Vec2 Right;
	static const Vec2 Left;

	Vec2() = default;
	constexpr Vec2(T x_, T y_) noexcept :					x{ x_ }, y{ y_ } {}
	constexpr Vec2(Vector2 vector2_) noexcept :	x{ (T)vector2_.x }, y{ (T)vector2_.y } {}

	//Addition
	constexpr Vec2 operator+(const Vec2& rm) const noexcept { return { x + rm.x, y + rm.y }; }
	constexpr Vec2& operator+=(const Vec2& rm) noexcept		{ x += rm.x; y += rm.y; return *this; }

	//Substraction
	constexpr Vec2 operator-(const Vec2& rm) const noexcept { return { x - rm.x, y - rm.y }; }
	constexpr Vec2& operator-=(const Vec2& rm) noexcept		{ x -= rm.x; y -= rm.y; return *this; }

	//Negation
	constexpr Vec2 operator-() const noexcept					{ return { -x, -y }; }

	//Scale Up
	constexpr Vec2 operator*(const float& rm) const noexcept	{ return { x * rm, y * rm }; }
	constexpr Vec2& operator*=(const float& rm) noexcept		{ x *= rm; y *= rm; return *this; }
	constexpr Vec2 operator*(const int& rm) const noexcept	{ return { x * rm, y * rm }; }
	constexpr Vec2& operator*=(const int& rm)	noexcept		{ x *= rm; y *= rm; return *this; }

	//Scale Down
	constexpr Vec2 operator/(const float& rm) const noexcept	{ return { x / rm, y / rm }; }
	constexpr Vec2& operator/=(const float& rm) noexcept		{ x /= rm; y /= rm; return *this; }
	constexpr Vec2 operator/(const int& rm) const	noexcept	{ return { x / rm, y / rm }; }
	constexpr Vec2& operator/=(const int& rm)	noexcept		{ x /= rm; y /= rm; return *this; }

	//Multiplication
	constexpr Vec2 operator*(const Vec2& rm) const noexcept { return { x * rm.x, y * rm.y }; }
	constexpr Vec2& operator*=(const Vec2& rm) noexcept		{ x *= rm.x; y *= rm.y; return *this; }

	//Division
	constexpr Vec2 operator/(const Vec2& rm) const noexcept { return { x / rm.x, y / rm.y }; }
	constexpr Vec2& operator/=(const Vec2& rm) noexcept		{ x /= rm.x; y /= rm.y; return *this; }

	//Boolean
	bool operator==(const Vec2& rm) const noexcept { return Math::nearlyEqual(x, rm.x) && Math::nearlyEqual(y, rm.y); }
	constexpr bool operator!=(const Vec2& rm) const noexcept	{ return !(*this == rm); }
	
	template<typename D>
	constexpr Vec2<D> to() const { return Vec2<D>((D)x, (D)y); }

	//Dot Product
	[[nodiscard]] constexpr T dot(const Vec2& other) const noexcept		{ return x * other.x + y * other.y; }

	//Rotation from Vec2
	[[nodiscard]] constexpr float getRot() const noexcept
	{
		if (x == 0 && y == 0) return 0;

		float a = atan2f(y, x) * RAD2DEG;
		if (a < 0) a += 360.0f;

		return a;
	}

	//Perpendicular ClockWise or CounterClockWise 
	[[nodiscard]] constexpr Vec2 perpendicularCcw() const			{ return { -y, x }; }
	[[nodiscard]] constexpr Vec2 perpendicularCw() const			{ return { y, -x }; }

	//Squared Length and Length
	[[nodiscard]] constexpr float sqrLength() const	noexcept		{ return x * x + y * y; }
	[[nodiscard]] constexpr float length() const noexcept			{ return sqrtf(sqrLength()); }

	[[nodiscard]] constexpr Vec2 absolute() const noexcept			{ return { Math::abs(x), Math::abs(y) }; }
	
	[[nodiscard]] constexpr Vec2 normalized() const noexcept
	{
		if (*this == Zero) return Zero; 
		float l = length(); 
		return { x / l, y / l };
	}

	[[nodiscard]] constexpr Vector2 toRaylib() const noexcept		{ return { (float)x, (float)y }; }
};

typedef Vec2<float> Vec2F;
typedef Vec2<int> Vec2I;

//Handle Collision infos
struct Collision
{
	Vec2F axis;
	float overlap;
	bool collided;

	[[nodiscard]] constexpr Vec2F getForce() const { return axis * overlap; }

	constexpr operator bool() const { return collided; }
};

//Simple Transform with : position / scale / rotation for 2d objects
struct Transform2D
{
	Vec2F position = Vec2F::Zero;
	Vec2F scale = Vec2F::One;
	float rotation;
};

//Oriented Rectangle struct with Collision | origin is the center
struct Rect2
{
	Vec2F center;
	Vec2F halfSize;
	float rotation;

	[[nodiscard]] std::vector<Vec2F> getCorners() const;

	//Collision
	[[nodiscard]] Collision checkAabb(const Rect2& other) const;	// ignore rot
	[[nodiscard]] Collision checkObb(const Rect2& other) const;

	[[nodiscard]] bool containPoint(const Vec2F& point) const;	// ignore rot

	void drawDebug(float scale) const; // Draw green points at corners and red point at center

	[[nodiscard]] constexpr Rect2 toObjectSpace(const Transform2D& transform) const { return { transform.position + center, transform.scale * halfSize, transform.rotation + rotation }; }
	// use the Rect2 offset with the object Transform

	[[nodiscard]] constexpr Rectangle toRaylib() const { return { center.x - halfSize.x, center.y - halfSize.y, halfSize.x * 2, halfSize.y * 2 }; } // ignore rotation

};

//Write and Read Operators

//Vec2F
inline std::ostream& operator<<(std::ostream& os, const Vec2F& v) { os << v.x << " " << v.y; return os; }
inline std::istream& operator>>(std::istream& is, Vec2F& v) { is >> v.x >> v.y; return is; }
//Vec2I
inline std::ostream& operator<<(std::ostream& os, const Vec2I& v) { os << v.x << " " << v.y; return os; }
inline std::istream& operator>>(std::istream& is, Vec2I& v) { is >> v.x >> v.y; return is; }

constexpr Vec2F vec2FLerp(const Vec2F a, const Vec2F b, const float t)		{ return { Math::lerp(a.x, b.x, t), Math::lerp(a.y, b.y, t)}; };

Vec2F vec2FromRot(float rot);

float overlapOnAxis(const std::vector<Vec2F>& a,const std::vector<Vec2F>& b, Vec2F axis);

Vec2F trigToCoord(float angle, float radius);

inline Color colorLerp(const Color& a, const Color& b, const float t) { return { Math::lerp(a.r, b.r, t), Math::lerp(a.g, b.g, t), Math::lerp(a.b, b.b, t), 255 }; }

bool pointInCircle(const Vec2F& point, const Vec2F& circleCenter, float circleRadius);

inline Vec2F randVec2()										{ return Vec2F{Math::randFloat(-1.0, 1.0f), Math::randFloat(-1.0f, 1.0f)}.normalized(); }