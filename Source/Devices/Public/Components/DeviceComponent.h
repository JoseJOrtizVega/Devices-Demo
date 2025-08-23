#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeviceComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogDeviceComponent, Log, All);

/**
 * Base class used to define devices.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEVICES_API UDeviceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Gets the owner of this component's device type.
	 * @return EDeviceType
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	EDeviceType GetDeviceType() const;

	/**
	 * Gets the owner of this component's device ID.
	 * @return EDeviceType
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	EDeviceID GetDeviceID() const;

protected:
	/**
	 * Device type for the component
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Device")
	EDeviceType DeviceType;

	/**
	 * Device ID for the component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Device")
	EDeviceID DeviceID;
};
