#pragma once
#include "raylib.h"

#include <iostream>
using std::cout, std::endl;

#include <random>
#include <cmath>

#include <vector>

// Note : 
// -rm for right member
// -i use rotation in degrees (unless otherwise specified)
// -i use float (unless otherwise specified)

namespace Struct {

	//Vector2 with lot of function and operators for easy use | Int version if needed for integral coordinates
	struct Vect2F
	{
		float x;
		float y;

		static const Vect2F zero;
		static const Vect2F one;
		static const Vect2F up;
		static const Vect2F down;
		static const Vect2F right;
		static const Vect2F left;

		Vect2F() = default;
		inline Vect2F(float x_, float y_) :	x{ x_ }, y{ y_ } {}
		inline Vect2F(Vector2 vector2_) :	x{ vector2_.x }, y{ vector2_.y } {}

		//Addition
		inline Vect2F operator+(const Vect2F& rm) const { return { x + rm.x, y + rm.y }; }
		inline Vect2F& operator+=(const Vect2F& rm)		{ x += rm.x; y += rm.y; return *this; }

		//Substraction
		inline Vect2F operator-(const Vect2F& rm) const { return { x - rm.x, y - rm.y }; }
		inline Vect2F& operator-=(const Vect2F& rm)		{ x -= rm.x; y -= rm.y; return *this; }

		//Negation
		inline Vect2F operator-() const					{ return { -x, -y }; }

		//Scale Up
		inline Vect2F operator*(const float& rm) const	{ return { x * rm, y * rm }; }
		inline Vect2F& operator*=(const float& rm)		{ x *= rm; y *= rm; return *this; }
		inline Vect2F operator*(const int& rm) const	{ return { x * rm, y * rm }; }
		inline Vect2F& operator*=(const int& rm)		{ x *= rm; y *= rm; return *this; }

		//Scale Down
		inline Vect2F operator/(const float& rm) const	{ return { x / rm, y / rm }; }
		inline Vect2F& operator/=(const float& rm)		{ x /= rm; y /= rm; return *this; }
		inline Vect2F operator/(const int& rm) const	{ return { x / rm, y / rm }; }
		inline Vect2F& operator/=(const int& rm)		{ x /= rm; y /= rm; return *this; }

		//Multiplication
		inline Vect2F operator*(const Vect2F& rm) const { return { x * rm.x, y * rm.y }; }
		inline Vect2F& operator*=(const Vect2F& rm)		{ x *= rm.x; y *= rm.y; return *this; }

		//Division
		inline Vect2F operator/(const Vect2F& rm) const { return { x / rm.x, y / rm.y }; }
		inline Vect2F& operator/=(const Vect2F& rm)		{ x /= rm.x; y /= rm.y; return *this; }

		//Boolean
		bool operator==(const Vect2F& rm) const;
		inline bool operator!=(const Vect2F& rm) const	{ return !(*this == rm); }

		//Dot Product
		inline float dot(const Vect2F& other) const		{ return x * other.x + y * other.y; }

		//Rotation from Vect2F
		float getRot() const;

		//Perpendicular ClockWise or CounterClockWise 
		inline Vect2F PerpendicularCCW() const			{ return { -y, x }; }
		inline Vect2F PerpendicularCW() const			{ return { y, -x }; }

		//Squared Length and Length
		inline float sqrLength() const					{ return x * x + y * y; }
		inline float length() const						{ return sqrtf(sqrLength()); }

		Vect2F absolute() const;
		Vect2F normalized() const;
		Vect2F clamp(float min, float max) const;

		inline Vector2 toRaylib() const					{ return { x, y }; }
	};

	struct Vect2I
	{
		int x;
		int y;

		static const Vect2I zero;
		static const Vect2I one;
		static const Vect2I up;
		static const Vect2I down;
		static const Vect2I right;
		static const Vect2I left;

		Vect2I() = default;
		inline Vect2I(int x_, int y_) : x{ x_ }, y{ y_ } {}

		//Addition
		inline Vect2I operator+(const Vect2I& rm) const { return { x + rm.x, y + rm.y }; }
		inline Vect2I& operator+=(const Vect2I& rm)		{ x += rm.x; y += rm.x; return *this; }

		//Substraction
		inline Vect2I operator-(const Vect2I& rm) const { return { x - rm.x, y - rm.y }; }
		inline Vect2I& operator-=(const Vect2I& rm)		{ x -= rm.x; y -= rm.x; return *this; }

		//Negation
		inline Vect2I operator-() const					{ return { -x, -y }; }

		//Scale Up
		inline Vect2I operator*(const int& rm) const	{ return { x * rm, y * rm }; }
		inline Vect2I& operator*=(const int& rm)		{ x *= rm; y *= rm; return *this; }

		//Scale Down
		inline Vect2I operator/(const int& rm) const	{ return { x / rm, y / rm }; }
		inline Vect2I& operator/=(const int& rm)		{ x /= rm; y /= rm; return *this; }

		//Multiplication
		inline Vect2I operator*(const Vect2I& rm) const { return { x * rm.x, y * rm.y }; }
		inline Vect2I& operator*=(const Vect2I& rm)		{ x *= rm.x; y *= rm.x; return *this; }

		//Division
		inline Vect2I operator/(const Vect2I& rm) const { return { x / rm.x, y / rm.y }; }
		inline Vect2I& operator/=(const Vect2I& rm)		{ x /= rm.x; y /= rm.x; return *this; }

		//Boolean
		inline bool operator==(const Vect2I& rm) const	{ return (x == rm.x && y == rm.y); }
		inline bool operator!=(const Vect2I& rm) const	{ return !(*this == rm); }

		//Dot product
		inline int dot(const Vect2I& other) const		{ return x * other.x + y * other.y; }

		//Rotation form Vect2I
		float getRot() const;

		//Perpendicular ClockWise or CounterClockWise 
		inline Vect2I PerpendicularCCW() const			{ return { -y, x }; }
		inline Vect2I PerpendicularCW() const			{ return { y, -x }; }

		//Squared Length and Lenght
		inline int sqrLength() const					{ return x * x + y * y; }
		inline float length() const						{ return sqrtf(static_cast<float>(sqrLength())); }

		Vect2I absolute() const;

		inline Vector2 toRaylib() const					{ return { static_cast<float>(x), static_cast<float>(y) }; }
	};

	//Handle Collision infos
	struct Collision
	{
		bool collided;
		Vect2F axis;
		float overlap;

		inline Vect2F getForce() const { return axis * overlap; }

		inline operator bool() const { return collided; }
	};

	//Simple Transform with : position / scale / rotation for 2d objects
	struct Transform2D
	{
		Vect2F position = Vect2F::zero;
		Vect2F scale = Vect2F::one;
		float rotation;
	};

	//Transform with : position in percentage / anchor / scale / rotation for ui objects
	struct UiTransform
	{
		Vect2F mPosition;
	};

	//Oriented Rectangle struct with Collision | origin is the center
	struct Rect2
	{
		Vect2F center;
		Vect2F halfSize;
		float rotation;

		std::vector<Vect2F> getCorners() const;

		//Collision
		Collision CheckAABB(const Rect2& other) const;	// ignore rot
		Collision CheckOBB(const Rect2& other) const;

		bool ContainPoint(const Vect2F& point) const;	// ignore rot

		void DrawDebug(float scale) const; // Draw green points at corners and red point at center

		inline Rect2 toObjectSpace(const Transform2D& transform) const { return { transform.position + center, transform.scale * halfSize, transform.rotation + rotation }; }
		// use the Rect2 offset with the object Transform

		inline Rectangle toRaylib() const { return { center.x - halfSize.x, center.y - halfSize.y, halfSize.x * 2, halfSize.y * 2 }; } // ignore rotation
	
	};

	//Write and Read Operators

	//Vect2F
	inline std::ostream& operator<<(std::ostream& os, const Vect2F& v) { os << v.x << " " << v.y; return os; }
	inline std::istream& operator>>(std::istream& is, Vect2F& v) { is >> v.x >> v.y; return is; }
	//Vect2I
	inline std::ostream& operator<<(std::ostream& os, const Vect2I& v) { os << v.x << " " << v.y; return os; }
	inline std::istream& operator>>(std::istream& is, Vect2I& v) { is >> v.x >> v.y; return is; }
}

namespace MathUtils {

	template <typename T>
	inline T Min(T a, T b)										{ return (a <= b) ? a : b; };

	template <typename T>
	inline T Max(T a, T b)										{ return (a >= b) ? a : b; };
	
	template <typename T>
	inline T Clamp(T value, T min, T max)						{ return Max(Min(value, max), min); };
	
	template <typename T>
	inline T Abs(T value)										{ return (value < 0) ? -value : value; };
	
	template <typename T>
	inline T Sign(T value)										{ return (value > 0) ? T(1) : (value < 0 ? T(-1) : T(0)); };

	template <typename T>
	inline T Lerp(T a, T b, T t)								{ return a + (b - a) * Clamp(t, T(0), T(1)); };

	using Struct::Vect2F;
	using Struct::Vect2I;

	inline Vect2F Vect2FLerp(Vect2F a, Vect2F b, float t)		{ return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t)}; };
	
	Vect2F Vect2FromRot(float rot);

	float OverlapOnAxis(const std::vector<Vect2F>& a,const std::vector<Vect2F>& b, Vect2F axis);
	
	bool NearlyEqual(const float a, const float b);

	int RandInt(int min, int max);

	float RandFloat(float min, float max);
	
	inline Vect2F RandVect2Normalized()							{ return Vect2F{RandFloat(-1.0, 1.0f), RandFloat(-1.0f, 1.0f)}.normalized(); };
};