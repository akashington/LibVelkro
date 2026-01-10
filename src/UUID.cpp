#include "UUID.h"

#include <uuid_v4.h>
#include <functional>

namespace Velkro
{
	class UUID::Data
	{
	public:
		Data() = default;
		~Data() = default;

		std::string& GetUUIDString()
		{
			return m_UUIDString;
		}

		std::string& GetUUIDBytes()
		{
			return m_UUIDBinary;
		}

	private:
		std::string m_UUIDString;
		std::string m_UUIDBinary;
	};

	UUID::UUID()
	{
		m_Data = new Data();
	}

	UUID::~UUID()
	{
		delete m_Data;
	}

	void UUID::GenerateUUID()
	{
		UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
		UUIDv4::UUID uuid = uuidGenerator.getUUID();
		
		m_Data->GetUUIDString() = uuid.str();
		m_Data->GetUUIDBytes() = uuid.bytes();
	}

	void UUID::GenerateUUIDFromBytes(const char* bytes)
	{
		UUIDv4::UUID uuid(bytes);
		
		m_Data->GetUUIDString() = uuid.str();
		m_Data->GetUUIDBytes() = uuid.bytes();
	}

	void UUID::GenerateUUIDFromString(const char* string)
	{
		UUIDv4::UUID uuid;
		uuid.fromStr(string);

		m_Data->GetUUIDString() = uuid.str();
		m_Data->GetUUIDBytes() = uuid.bytes();
	}

	const char* UUID::GetUUIDString() const
	{
		return m_Data->GetUUIDString().c_str();
	}
	const char* UUID::GetUUIDBytes() const
	{
		return m_Data->GetUUIDBytes().c_str();
	}
	
	bool UUID::operator==(const UUID& other) const
	{
		return this->m_Data->GetUUIDBytes() == other.m_Data->GetUUIDBytes();
	}
	bool UUID::operator!=(const UUID& other) const
	{
		return !(*this == other);
	}
}