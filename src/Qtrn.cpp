#include "LMath/Qtrn.hpp"
#include <math.h>
#include "LMath/MathAux.hpp"
#include <assert.h>

using namespace LMath;

LMath::Qtrn::Qtrn() : Qtrn(1.0f,0.0f,0.0f,0.0f) {}

LMath::Qtrn::Qtrn(const float t,const float x,const float y,const float z) : t(t), x(x), y(y), z(z) { }

LMath::Qtrn::Qtrn(const float thetaRad,const Vec3& axis) {
	Vec3 axisNormalized = axis.normalize();

	float halfTheta = thetaRad * 0.5f;

	t = cosf(halfTheta);

	float s = sinf(halfTheta);

	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;

	clean();
}

Qtrn LMath::Qtrn::IDENTITY = Qtrn(1.f, 0.f, 0.f, 0.f);

void LMath::Qtrn::toAngleAxis(float& thetaRad, Vec3& axis) const {
	
	Qtrn normalized = this->normalize();

	thetaRad = 2 * acosf(normalized.t);
	float s = sqrtf(1 - normalized.t * normalized.t);

	if (cmpf(s, 0.0f)) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
	}
	else {
		float sInv = 1.0f / s;
		axis.x = sInv * normalized.x;
		axis.y = sInv * normalized.y;
		axis.z = sInv * normalized.z;
	}
}

void LMath::Qtrn::clean() {
	if (cmpf(t, 0.0f)) t = 0.0f;
	if (cmpf(x, 0.0f)) x = 0.0f;
	if (cmpf(y, 0.0f)) y = 0.0f;
	if (cmpf(z, 0.0f)) z = 0.0f;
}

float LMath::Qtrn::quadrance() const {
	return SQR(t) + SQR(x) + SQR(y) + SQR(z);
}

float LMath::Qtrn::norm() const {
	return sqrtf(quadrance());
}

float Qtrn::angle() const
{
	const Qtrn normalized = this->normalize();
	return 2.f * acosf(normalized.t);
}

Qtrn LMath::Qtrn::normalize() const {

	float n = norm();

	// Cannot divide by zero
	assert(n > 0.0f);

	float s = 1 / norm();

	return (*this) * s;
}

Qtrn LMath::Qtrn::conjugate() const {
	return {t, -x, -y, -z};
}

Qtrn LMath::Qtrn::inverse() const {

	float quad = quadrance();

	assert(quad > 0.0f);

	return conjugate() * (1.0f/quad);
}

Qtrn LMath::Qtrn::operator*(const float s) const {
	return { t*s, x*s, y*s, z*s };
}

Qtrn LMath::operator*(const float s, const Qtrn& q) {
	return { s*q.t, s*q.x, s*q.y, s*q.z};
}

Qtrn LMath::Qtrn::operator*(const Qtrn& q) const {
	Qtrn prod;
	prod.t = t * q.t - x * q.x - y * q.y - z * q.z;
	prod.x = t * q.x + x * q.t + y * q.z - z * q.y;
	prod.y = t * q.y + y * q.t + z * q.x - x * q.z;
	prod.z = t * q.z + z * q.t + x * q.y - y * q.x;
	return prod;
}

Qtrn LMath::Qtrn::operator+(const Qtrn& q) const {
	return { t + q.t, x + q.x, y + q.y, z + q.z };
}

Qtrn& LMath::Qtrn::operator*=(const float s) {
	t *= s;
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Qtrn& LMath::Qtrn::operator*=(const Qtrn& q) {
	t = t * q.t - x * q.x - y * q.y - z * q.z;
	x = t * q.x + x * q.t + y * q.z - z * q.y;
	y = t * q.y + y * q.t + z * q.x - x * q.z;
	z = t * q.z + z * q.t + x * q.y - y * q.x;
	return *this;
}

Qtrn& LMath::Qtrn::operator+=(const Qtrn& q) {
	t += q.t;
	x += q.x;
	y += q.y;
	z += q.z;
	return *this;
}

Qtrn& LMath::Qtrn::operator=(const Qtrn& q) {
	t = q.t;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

bool LMath::Qtrn::operator==(const Qtrn& q) const {
	return cmpf(t, q.t) && cmpf(x, q.x) && cmpf(y, q.y) && cmpf(z, q.z);
}

bool LMath::Qtrn::operator!=(const Qtrn& q) const {
	return !(*this == q);
}

Qtrn LMath::Qtrn::lerp(const Qtrn& q0, const Qtrn& q1, float k) {
	float cosAngle = q0.t * q1.t + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
	float k0 = 1.0f - k;
	float k1 = (cosAngle > 0) ? k : -k;
	return (q0 * k0 + q1 * k1).normalize();
}

Qtrn LMath::Qtrn::slerp(const Qtrn& q0, const Qtrn& q1, float k) {

	// Difference at which to LERP instead of SLERP
	constexpr float delta = 0.0001f;

	// Calc cosine
	float sign_scale1 = 1.0f;
	float cos_omega = q1.quadrance();

	// Adjust signs (if necessary)
	if (cos_omega < 0.0f)
	{
		cos_omega = -cos_omega;
		sign_scale1 = -1.0f;
	}

	// Calculate coefficients
	float scale0, scale1;
	if (1.0f - cos_omega > delta)
	{
		// Standard case (slerp)
		const float omega = acosf(cos_omega);
		const float sin_omega = asinf(omega);
		scale0 = sinf((1.0f - k) * omega) / sin_omega;
		scale1 = sign_scale1 * sinf(k * omega) / sin_omega;
	}
	else
	{
		// Quaternions are very close so we can do a linear interpolation
		scale0 = 1.0f - k;
		scale1 = sign_scale1 * k;
	}

	// Interpolate between the two quaternions
	return Qtrn(q0*scale0 +q1*scale1).normalize();
}

Qtrn LMath::Qtrn::fromDir(const Vec3& dir, const Vec3& ref) {

	/*Vec3 axis = cross(dir, ref);
	float t = 1 + dot(dir, ref);
	Qtrn q(t, -axis.x, axis.y, -axis.z);
	q = q.normalize();*/
	Vec3 axis = cross(ref, dir);
	float dotProduct = dot(ref, dir);
	float halfAngle = acosf(dotProduct) * .5f;

	// Handle special case where ref and dir are parallel
	if (axis.magnitude() == 0.0f) {
		// If ref and dir are parallel
		if (dotProduct > 0.9999f) {
			// Identity quaternion (no rotation)
			return Qtrn(1, 0, 0, 0);
		} else {
			// 180-degree rotation around any axis perpendicular to ref (e.g., the X or Y axis)
			// Assuming ref is normalized, if not, normalize it first
			return Qtrn(0, 1, 0, 0);  // Example: 180-degree rotation around X-axis
		}
	}

	Qtrn q(cos(halfAngle), axis.x*sin(halfAngle), axis.y*sin(halfAngle), axis.z*sin(halfAngle));
	q = q.normalize();

	return q;
}

Mat4 LMath::Qtrn::toRotationMatrix() const {
	
	Qtrn qn = this->normalize();

	float xt = x * t;
	float xx = x * x;
	float xy = x * y;
	float xz = x * z;
	float yt = y * t;
	float yy = y * y;
	float yz = y * z;
	float zt = z * t;
	float zz = z * z;

	Mat4 rot;

	rot[0][0] = 1.0f - 2.0f * (yy + zz);
	rot[1][0] = 2.0f * (xy + zt);
	rot[2][0] = 2.0f * (xz - yt);
	rot[3][0] = 0.0f;
	
	rot[0][1] = 2.0f * (xy - zt);
	rot[1][1] = 1.0f - 2.0f * (xx + zz);
	rot[2][1] = 2.0f * (yz + xt);
	rot[3][1] = 0.0f;
	
	rot[0][2] = 2.0f * (xz + yt);
	rot[1][2] = 2.0f * (yz - xt);
	rot[2][2] = 1.0f - 2.0f * (xx + yy);
	rot[3][2] = 0.0f;
	
	rot[0][3] = 0.0f;
	rot[1][3] = 0.0f;
	rot[2][3] = 0.0f;
	rot[3][3] = 1.0f;

	return rot;
}

std::ostream& LMath::operator<<(std::ostream& os, const Qtrn& qtrn) {
	os << "(" << qtrn.t << "," << qtrn.x << "," << qtrn.y << "," << qtrn.z << "," << ")";
	return os;
}
void LMath::Qtrn::printAngleAxis() {
	float angleRad;
	Vec3 axis;
	toAngleAxis(angleRad, axis);
	std::cout << "Angle: " << radiansToDegrees(angleRad) << " Axis: " << axis;
}