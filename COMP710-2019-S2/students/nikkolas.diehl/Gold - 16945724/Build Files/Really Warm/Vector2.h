#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

class Vector2
{
public:
	Vector2();
	//Take in an x and a y
	Vector2(double in_x, double in_y);
	void Set(double in_x, double in_y);
	~Vector2();

	double x;
	double y;
	const float _inaccuracy = 5.0f; //Floating points compared with 1e-5

	//scaling enum
	enum Scaling {
		ADDITON,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,
		EQUAL
	};

	//Vector2 operator overrides
	bool operator<(Vector2& other) const;
	bool operator>(Vector2& other) const;
	bool operator<=(Vector2& other) const;
	bool operator>=(Vector2& other) const;
	//Vector2 special operator overrides
	std::unique_ptr<Vector2> operator-(Vector2& other);
	std::unique_ptr<Vector2> operator+(Vector2& other);
	std::unique_ptr<Vector2> operator*(Vector2& other);
	std::unique_ptr<Vector2> operator/(Vector2& other);

	//Vector2 math
	double Dot(Vector2* second);
	void Add(Vector2* other);
	void Minus(Vector2* other);
	void Times(Vector2* other);
	void Divide(Vector2* other);
	void Ceil();
	void Floor();
	double Heading();
	void Clear();

	//More Vector2 math
	void MakeCeil(Vector2* other);
	void MakeFloor(Vector2* other);

	//Scaler
	std::unique_ptr<Vector2> ScaleMult(double scale);
	std::unique_ptr<Vector2> ScaleDiv(double scale);

	//Vector2 returns
	std::unique_ptr<Vector2> GetNormalize();
	void Normalize();

	//Vector2 functions
	void Copy(Vector2* from);
	double Magnatude();
	double AngleRad(Vector2* other);
	double AngleDeg(Vector2* other);

	//Position Checking
	double Distance(Vector2* other);

	//Extra Utilities
	bool ValuesEqual(double one, double two);
	bool Lerp(Vector2* towards, double speed);
	bool LerpX(Vector2* towards, double speed);
	bool LerpY(Vector2* towards, double speed);
	void Negate();
	double Constrain(double n, double low, double high);
	std::string ToString();
};

#endif

