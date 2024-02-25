#include <cmath>
#include "LMath/Vec3.hpp"
#include "LMath/MathAux.hpp"

using namespace LMath;

LMath::Vec3::Vec3() : Vec3(0.f) {}
LMath::Vec3::Vec3(float xyz) : Vec3(xyz, xyz, xyz) {}
LMath::Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) { }


Vec3 LMath::Vec3::ONE = Vec3(1.f);
Vec3 LMath::Vec3::ZERO = Vec3(0.f);
Vec3 LMath::Vec3::Z = Vec3(0.f, 0.f, 1.f);
Vec3 LMath::Vec3::Y = Vec3(0.f, 1.f, 0.f);
Vec3 LMath::Vec3::X = Vec3(1.f, 0.f, 0.f);
Vec3 LMath::Vec3::FORWARD = Vec3(0.f, 0.f, 1.f);
Vec3 LMath::Vec3::RIGHT = Vec3(1.f, 0.f, 0.f);
Vec3 LMath::Vec3::UP = Vec3(0.f, 1.f, 0.f);

Vec3 LMath::Vec3::random() {
	return {randomFloat(), randomFloat(), randomFloat()};
}

Vec3 LMath::Vec3::operator+(const Vec3& other) const {
	return {this->x + other.x, this->y + other.y, this->z + other.z};
}

Vec3 LMath::Vec3::operator-(const Vec3& other) const {
	return {this->x - other.x, this->y - other.y, this->z - other.z};
}

Vec3& LMath::Vec3::operator=(const Vec3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vec3& LMath::Vec3::operator+=(const Vec3& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vec3& LMath::Vec3::operator*=(const Vec3& other)
{
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	return *this;
}

Vec3& LMath::Vec3::operator/=(const Vec3& other)
{
	this->x /= other.x;
	this->y /= other.y;
	this->z /= other.z;
	return *this;
}

Vec3 LMath::Vec3::operator*(float scalar) const {
	return {this->x * scalar, this->y * scalar, this->z * scalar};
}

Vec3 LMath::Vec3::operator/(float scalar) const {
	return {this->x / scalar, this->y / scalar, this->z / scalar};
}

Vec3 LMath::Vec3::operator+(float scalar) const {
	return {this->x + scalar, this->y + scalar, this->z + scalar};
}

Vec3 LMath::Vec3::operator-(float scalar) const {
	return {this->x - scalar, this->y - scalar, this->z - scalar};
}

Vec3& LMath::Vec3::operator+=(float s) {
	this->x += s;
	this->y += s;
	this->z += s;
	return *this;
}
Vec3& LMath::Vec3::operator-=(float s) {
	this->x -= s;
	this->y -= s;
	this->z -= s;
	return *this;
}
Vec3& LMath::Vec3::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}
Vec3& LMath::Vec3::operator/=(float s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}


// cmpf uses compares the floats with an error (EPSILON)
bool LMath::Vec3::operator==(const Vec3& other) const {
	return cmpf(this->x , other.x) && cmpf(this->y, other.y) && cmpf(this->z, other.z);
}

bool LMath::Vec3::operator!=(const Vec3& other) const {
	return !(*this == other);
}

float LMath::Vec3::angle(const Vec3& a, const Vec3& b) {
	float magnitudeA = a.magnitude();
	float magnitudeB = a.magnitude();

	if (cmpf(magnitudeA, 0.0f) || cmpf(magnitudeB, 0.0f))
		return 0.0f;

	return acosf(dot(a, b) / (magnitudeA * magnitudeB));
}


float LMath::Vec3::magnitude() const {
	return std::sqrt(this->x * this->x + this->y * this->y + this->z*this->z);
}

float LMath::Vec3::sqrMagnitude() const {
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

void LMath::Vec3::toOpenGLFormat(float array[3]) const {
	array[0] = this->x;
	array[1] = this->y;
	array[2] = this->z;
}


Vec3 LMath::Vec3::normalize() const {

	float magnitude = this->magnitude();

	if (cmpf(magnitude, 0.0f))
		return *this;

	// Cannot divide by 0
	//assert(magnitude > 0);

	return (*this) / magnitude;
}

Vec4 LMath::Vec3::toVec4() const {
	return {this->x, this->y, this->z, 0};
}

Vec3 LMath::Vec3::lerp(Vec3 initial, Vec3 final, float f) {
	return initial + f * (final - initial);
}

float LMath::dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 LMath::cross(const Vec3& a, const Vec3& b) {
	return {(a.y * b.z - a.z * b.y),(a.z * b.x - a.x * b.z),(a.x * b.y - a.y * b.x)};
}

// K has to be normalized
// Thetha should be converted to radians
// Vrot = v*cos() + (k x v)*sin() + k*(k.v) * (1- cos())  
Vec3 LMath::rodrigues(Vec3 v, float thetaDegrees, Vec3 k) {
	float thetaRadians = degreesToRadians(thetaDegrees);
	Vec3 unitK = k.normalize();

	return v * cosf(thetaRadians) + LMath::cross(unitK, v)* sinf(thetaRadians) + unitK * LMath::dot(unitK, v) * (1 - cosf(thetaRadians));
}

std::ostream& LMath::operator<<(std::ostream& os, const Vec3& vec3) {
	return os << "(" << vec3.x << "," << vec3.y << "," << vec3.z << ')';
}

Vec3 LMath::operator*(const float scalar, const Vec3& vec3) {
	return {scalar * vec3.x, scalar * vec3.y, scalar * vec3.z};
}

Vec3 LMath::operator+(float scalar, const Vec3& vec3) {
	return {scalar + vec3.x, scalar + vec3.y, scalar + vec3.z};
}

Vec3 LMath::operator-(float scalar, const Vec3& vec3) {
	return {scalar - vec3.x, scalar - vec3.y, scalar - vec3.z};
}

Vec3 Vec3::operator-() const {
    return {-x, -y, -z};
}
