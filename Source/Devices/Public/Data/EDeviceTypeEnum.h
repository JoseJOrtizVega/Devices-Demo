#pragma once

#include "CoreMinimal.h"
#include "EDeviceTypeEnum.generated.h"

/**
 * Enum responsible for defining the ID for any given device.
 */
UENUM(BlueprintType)
enum class EDeviceID : uint8
{
	Room_1				UMETA(DisplayName = "Room 1"),
	Room_2				UMETA(DisplayName = "Room 2"),
	Room_3				UMETA(DisplayName = "Room 3"),
	Room_4				UMETA(DisplayName = "Room 4"),
	Room_5				UMETA(DisplayName = "Room 5"),
	Room_6				UMETA(DisplayName = "Room 6"),
	Room_7				UMETA(DisplayName = "Room 7"),
	Room_8				UMETA(DisplayName = "Room 8"),
	Room_9				UMETA(DisplayName = "Room 9"),
	Room_10				UMETA(DisplayName = "Room 10")
};

/**
 * Enum responsible for defining a special ID for any given [DeviceReceiverComponent] for special purposes.
 */
UENUM(BlueprintType)
enum class EDeviceSpecialID : uint8
{
	ID_1				UMETA(DisplayName = "ID 1"),
	ID_2				UMETA(DisplayName = "ID 2"),
	ID_3				UMETA(DisplayName = "ID 3"),
	ID_4				UMETA(DisplayName = "ID 4"),
	ID_5				UMETA(DisplayName = "ID 5"),
	ID_6				UMETA(DisplayName = "ID 6"),
	ID_7				UMETA(DisplayName = "ID 7"),
	ID_8				UMETA(DisplayName = "ID 8"),
	ID_9				UMETA(DisplayName = "ID 9"),
	ID_10				UMETA(DisplayName = "ID 10")
};

/**
 * Enum Responsible for defining the type of device.
 */
UENUM(BlueprintType)
enum class EDeviceType : uint8
{
	Sender				UMETA(DisplayName = "Sender"),
	Receiver			UMETA(DisplayName = "Receiver")
};