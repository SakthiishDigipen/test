#pragma once

#include "Quaternion.h"

namespace BE
{

	// Constructors ----------------------------------------------------------------------------------------------------------

	template <typename T>
	Vec3<T>::Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z)
	{
	}

	template <typename T>
	Vec3<T>::Vec3() : x(0), y(0), z(0)
	{
	}

	template <typename T>
	Vec3<T>::Vec3(T horiAngle_Deg, T vertAngle_Deg)
	{
		x = Math::CosDeg(horiAngle_Deg) * Math::CosDeg(vertAngle_Deg);
		y = Math::SinDeg(vertAngle_Deg);
		z = Math::SinDeg(horiAngle_Deg) * Math::CosDeg(vertAngle_Deg);
	}

	// Operators -------------------------------------------------------------------------------------------------------------

	template <typename T>
	Vec3<T> Vec3<T>::operator+(const Vec3<T>& rhs) const
	{
		return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator-(const Vec3<T>& rhs) const
	{
		return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator*(const T scalar) const
	{
		// This is only defined in Vector operation
		return Vec3<T>(x * scalar, y * scalar, z * scalar);
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator*(const Vec3<T>& rhs) const
	{
		return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator/(const T scalar) const
	{
		// This is only defined in Vector operation
		return Vec3<T>(x / scalar, y / scalar, z / scalar);
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator/(const Vec3<T>& rhs) const
	{
		return Vec3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	template <typename T>
	Vec3<T> Vec3<T>::operator-() const
	{
		return Vec3<T>(-x, -y, -z);
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& rhs)
	{
		*this = *this - rhs;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator*=(const T scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator/=(const T scalar)
	{
		*this = *this / scalar;
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator/=(const Vec3<T>& rhs)
	{
		*this = *this / rhs;
		return *this;
	}

	template <typename T>
	bool Vec3<T>::operator==(const Vec3<T>& rhs) const
	{
		return Math::IsEqual(x, rhs.x) && Math::IsEqual(y, rhs.y) && Math::IsEqual(z, rhs.z);
	}

	template <typename T>
	bool Vec3<T>::operator!=(const Vec3<T>& rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T>
	T& Vec3<T>::operator[](unsigned int index)
	{
		return const_cast<T&>(static_cast<const Vec3<T>&>(*this)[index]);
	}

	template <typename T>
	const T& Vec3<T>::operator[](unsigned int index) const
	{
		switch (index)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return x;
			break;
		}
	}

	// Functions that returns vector -----------------------------------------------------------------------------------------

	template <typename T>
	Vec3<T>& Vec3<T>::Normalise()
	{
		T magnitude = this->Mag();

		// Deals with 0 vectors
		if (Math::IsEqual(magnitude, T{}))
			return *this;

		x /= magnitude;
		y /= magnitude;
		z /= magnitude;

		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Normalised() const
	{
		T magnitude = this->Mag();

		// Deals with 0 vectors
		if (Math::IsEqual(magnitude, T{}))
			return *this;

		return Vec3<T>(x / magnitude, y / magnitude, z / magnitude);
	}

	template <typename T>
	Vec3<T> Vec3<T>::Cross(const Vec3<T>& rhs) const
	{
		return Vec3<T>(y * rhs.z - rhs.y * z,
			z * rhs.x - rhs.z * x,
			x * rhs.y - rhs.x * y);
	}

	template <typename T>
	Vec3<T> Vec3<T>::VecTowards(const Vec3<T>& endPoint) const
	{
		return endPoint - *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::DirTowards(const Vec3<T>& endPoint) const
	{
		return this->VecTowards(endPoint).Normalise();
	}

	template <typename T>
	Vec3<T> Vec3<T>::ProjectOnto(const Vec3<T>& projOnto) const
	{
		return (this->Dot(projOnto) / projOnto.Dot(projOnto)) * projOnto;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Rotate_Deg(const Vec3& axis, T angle) const
	{
		return Quaternion<T>(axis, angle) * *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::Rotate_Rad(const Vec3& axis, T angle) const
	{
		return Rotate_Deg(axis, Math::RadToDeg(angle));
	}

	// Functions that returns value ---------------------------------------------------------------------------------------------

	template <typename T>
	T Vec3<T>::Mag() const
	{
		return Math::Sqrt(x * x + y * y + z * z);
	}

	template <typename T>
	T Vec3<T>::SqMag() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	T Vec3<T>::Dot(const Vec3<T>& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	}

	template <typename T>
	T Vec3<T>::AngleBetween(const Vec3<T>& rhs, bool isDegree) const
	{
		T mag = this->Mag() * rhs.Mag();

		// If either vector is a 0 vector, then treat the angle between the 2 vectors to be 0
		if (Math::IsEqual(mag, T{}))
			return T{};

		if (isDegree)
			return Math::ACosDeg(this->Dot(rhs) / mag);
		else
			return Math::ACosRad(this->Dot(rhs) / mag);
	}

	template <typename T>
	T Vec3<T>::DistBetween(const Vec3<T>& endPoint) const
	{
		return this->VecTowards(endPoint).Mag();
	}

	template <typename T>
	T Vec3<T>::SqDistBetween(const Vec3<T>& endPoint) const
	{
		return this->VecTowards(endPoint).SqMag();
	}

	template <typename T>
	T Vec3<T>::CrossProductMag(const Vec3<T>& rhs) const
	{
		// | v X u | = |v||u| sin(A)
		return this->Mag() * rhs.Mag() * Math::SinRad(this->AngleBetween_Rad(rhs));
	}

	// Non member functions -----------------------------------------------------------------------------------------------------

	template <typename T>
	Vec3<T> operator*(T scalar, const Vec3<T>& rhs)
	{
		return rhs * scalar;
	}

	template <typename T>
	Vec3<T> operator/(T scalar, const Vec3<T>& rhs)
	{
		return Vec3<T>(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vec3<T>& rhs)
	{
		os << "(  XYZ: ";
		os << std::fixed << std::setprecision(5) << std::setw(11) << rhs.x << " ";
		os << std::fixed << std::setprecision(5) << std::setw(11) << rhs.y << " ";
		os << std::fixed << std::setprecision(5) << std::setw(11) << rhs.z << " )";
		return os;
	}
}