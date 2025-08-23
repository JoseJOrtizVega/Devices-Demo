#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "DeviceCollectorVolume.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogDeviceCollectorVolume, Log, All);

/***********************/
/* Forward Declaration */
class UDeviceComponent;
class USenderComponent;

/**
 * Volume responsible for collecting all the actors within its boundaries that implement [SenderComponents] or [ReceiverComponents]
 */
UCLASS()
class DEVICES_API ADeviceCollectorVolume : public AVolume
{
	GENERATED_BODY()
public:
	ADeviceCollectorVolume();

protected:
	virtual void BeginPlay() override;

	/**
	 * Collects all the [DeviceSenderComponents] and [DeviceReceiverComponents] that are inside this actor's volume
	 * @param InstigatorActor Whoever initiated the data collection from the volume
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	virtual void CollectData(AActor* InstigatorActor);

	/**
	 * Sends an unsubscription call to all the linked devices to this volume.
	 * @param InstigatorActor Whoever initiated the unsubscription call to all the linked devices
	 */
	UFUNCTION(BlueprintCallable, Category = "Device")
	void SendUnsubscribeCalls(AActor* InstigatorActor);

	/**
	 * An Array that contains a list of actors in a scene that have a [SenderComponent] attached to them.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Device")
	TArray<TObjectPtr<AActor>> ActorsWithSenderComponents;

	/**
	 * An Array of Devices connected to this component.
	 */
	UPROPERTY()
	TArray<TObjectPtr<USenderComponent>> LinkedDeviceSenderComponents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Device")
	bool bBeginCollectionOnBeginPlay;
};
