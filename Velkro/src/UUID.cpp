#include "UUID.h"

#include <uuid_v4.h>

namespace Velkro::UUID
{
	UUIDv4::UUID GenerateUUID()
	{
		UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

		return uuidGenerator.getUUID();
	}
}