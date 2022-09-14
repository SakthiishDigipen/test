#include "pch.h"
#include "BEID.h"

namespace BE
{
	//Using a separate random number generator to avoid issuing IDs using uninitalised RNG engine.
	static std::mt19937_64 s_BEIDGenerator{ std::random_device()() };

	//Generate a random ID
	BEID::BEID() : m_id( std::uniform_int_distribution<IDType>(1ui64, 0xffffffffffffffffui64)(s_BEIDGenerator))
	{
		
	}

	//Creates a predefined ID, such as through deserialisation
	BEID::BEID(IDType _id) : m_id(_id)
	{
		
	}

	//Print ID
	std::ostream& operator<<(std::ostream& os, const BEID& _id)
	{
		os << "BEID: [" << std::setw(20) << _id.m_id << "]";
		return os;
	}

	//Conversion operator
	BEID::operator IDType() const
	{
		return m_id;
	}

	//Compares 2 BEID
	bool BEID::operator==(const BEID& _rhs) const
	{
		return m_id == _rhs.m_id;
	}
		
	bool BEID::operator!=(const BEID& _rhs) const
	{
		return !(*this == _rhs);
	}

	bool BEID::operator<(const BEID& _rhs)
	{
		return m_id < _rhs.m_id;
	}
}