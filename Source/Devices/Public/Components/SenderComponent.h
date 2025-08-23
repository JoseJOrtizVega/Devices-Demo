#pragma once

#include "CoreMinimal.h"
#include "Components/DeviceComponent.h"
#include "SenderComponent.generated.h"

/***********************/
/* Forward Declaration */
class UReceiverComponent;

/**
 * Actor Component responsible for enabling the owner of such component to act as a sender of data.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class DEVICES_API USenderComponent : public UDeviceComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Device")
	static USenderComponent* GetDeviceSenderComponent(AActor* TargetActor);
	
	USenderComponent();

protected:
	/**
	 * An Array of Devices connected to this component.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UReceiverComponent>> LinkedDeviceReceiverComponents;

public:
	/**
	 * Registers a [DeviceReceiverComponent] to this one.
	 * @param DeviceReceiverComponent The [DeviceReceiverComponent] to register.
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void ReceiveComponentForRegistration(AActor* InstigatorActor, UReceiverComponent* DeviceReceiverComponent);

	/**
	 * Sends a call to all the [DeviceReceiverComponents] that are inside the [DeviceCollectorVolume]
	 * @param InstigatorActor Whoever initiated the data collection from the volume
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void SendGenericCalls(AActor* InstigatorActor);

	/**
	 * Returns an array of all the linked receiver devices to this one.
	 * @return returns an array of all the linked devices
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	TArray<UReceiverComponent*> GetLinkedDevices();

	/**
	 * Clears all linked devices from this one.
	 * @param InstigatorActor whoever initiated the unregistration process
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void ClearLinkedDevices(AActor* InstigatorActor);

	/**
	 * Unregisters a single linked device from this one.
	 * @param DeviceReceiverComponent The [DeviceReceiverComponent] to unregister.
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void ClearSingleLinkedDevice(UReceiverComponent* DeviceReceiverComponent);

private:	
	void ProcessUnregisterQueue();
	void ProcessSingleUnregister(UReceiverComponent* DeviceReceiverComponent);
	
	//timer
	FTimerHandle UnregisterQueueTimerHandle;
	
	// queue managers
	TArray<UReceiverComponent*> PendingUnregisterQueue;
	bool bIsProcessingCalls;
};
