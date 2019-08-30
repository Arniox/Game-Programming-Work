#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

class Vector2
{
public:
	Vector2();
	~Vector2();

	static Vector2& CreateVector(float in_x, float in_y);

	float x;
	float y;

	//Vector2 operator overrides
	bool operator==(Vector2& other) const;
	bool operator!=(Vector2& other) const;
	bool operator<(Vector2& other) const;
	bool operator>(Vector2& other) const;
	Vector2& operator-(Vector2& other);
	Vector2& operator+(Vector2& other);

	//Vector2 functions
	float Magnatude();
	float Dot(Vector2& other);
	float Cross2D(Vector2& other_p1, Vector2& other_p2);
	float AngleRad(Vector2& other);
	float AngleDeg(Vector2& other);
	Vector2& Scale(float& scale);
	Vector2& Normalized();

protected:
	static Vector2* mp_Instance;
};

