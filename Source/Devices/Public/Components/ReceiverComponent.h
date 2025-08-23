#pragma once

#include "CoreMinimal.h"
#include "Components/DeviceComponent.h"
#include "ReceiverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCallReceived, AActor*, InstigatorActor);

/***********************/
/* Forward Declaration */
class USenderComponent;

/**
 * Personal Enum for receiver information. Used to when returning if the owner of this component should receive calls.
 */
UENUM(BlueprintType)
enum class EReceiverState : uint8
{
	CanReceiveCalls			UMETA(DisplayName = "Can Receive Calls"),
	CanNotReceiveCalls		UMETA(DisplayName = "Can Not Receive Calls")
};

/**
 * Component that denotes the holder as a receiver component.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class DEVICES_API UReceiverComponent : public UDeviceComponent
{
	GENERATED_BODY()
public:
	static UReceiverComponent* GetDeviceReceiverComponent(AActor* TargetActor);
	
	UReceiverComponent();

protected:
	/**
	 * Whether this component should receive input from the Device Sender Component.
	 */
	UPROPERTY(SaveGame, EditAnywhere, Category = "Device")
	bool bShouldBeRegistered;

	/**
	 * Dictates if the owner of this component can receive calls from the device sender linked to this one.
	 */
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "Device")
	bool bCanReceiveCalls;
	
	/**
	 * The device's special ID for special logic.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Device")
	EDeviceSpecialID DeviceSpecialID;

	/**
	 * The Device Sender Component linked to this one.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Device")
	TObjectPtr<USenderComponent> LinkedDeviceSenderComponent;

public:
	/**
	 * Sets the linked Device Sender Component.
	 * @param DeviceSenderComponent The Device Sender Component linked to this one.
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void SetLinkedSenderDevice(USenderComponent* DeviceSenderComponent);

	/**
	 * Sends a message to the linked Device sender to unsubscribe from its calls.
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void UnRegisterFromCalls();

	/**
	 * Gets the device's special ID for this component.
	 * @return returns the device's special ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	EDeviceSpecialID GetDeviceSpecialID() const;

	/**
	 * Returns the object's status on allowing registration. If it returns false, then the object won't be recorded or registered.
	 * @return returns true if the object holding the component should be recorded.
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	bool ShouldBeRegistered() const;

	UFUNCTION(BlueprintCallable, Category = "Device")
	bool CanReceiveCalls() const;

	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void FlipDeviceState(UPARAM(ref) EReceiverState& ReceiverState);

	/**
	 * This event gets called when a DeviceSenderComponent linked to this one sends a call.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Device")
	FOnCallReceived OnCallReceived;
};
