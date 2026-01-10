#pragma once

namespace Velkro
{
	class UUID
	{
	public:
		UUID();
		~UUID();

		void GenerateUUID();
		void GenerateUUIDFromBytes(const char* bytes);
		void GenerateUUIDFromString(const char* string);

		const char* GetUUIDString() const;
		const char* GetUUIDBytes() const;

		bool operator==(const UUID& other) const;
		bool operator!=(const UUID& other) const;
		
	private:
		class Data;
		Data* m_Data;
	};
}