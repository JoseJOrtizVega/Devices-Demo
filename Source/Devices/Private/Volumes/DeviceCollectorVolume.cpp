#include "Volumes/DeviceCollectorVolume.h"

#include "Components/BrushComponent.h"
#include "Components/ReceiverComponent.h"
#include "Components/SenderComponent.h"
#include "Data/EDeviceTypeEnum.h"
#include "Interfaces/RegistrationInterface.h"

ADeviceCollectorVolume::ADeviceCollectorVolume()
{
	bBeginCollectionOnBeginPlay = false;
	
	GetBrushComponent()->SetGenerateOverlapEvents(false);
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetBrushComponent()->SetCollisionProfileName("OverlapAll");
}

void ADeviceCollectorVolume::BeginPlay()
{
	Super::BeginPlay();

	if (bBeginCollectionOnBeginPlay)
	{
		CollectData(this);
	}
}

void ADeviceCollectorVolume::CollectData(AActor* InstigatorActor)
{
	// don't perform any actions if array of actors is empty
	if (ActorsWithSenderComponents.IsEmpty())
	{
		UE_LOG(LogDeviceCollectorVolume, Error, TEXT("[%s] doesn't have any actors with sender components. CollectData function will not run"), *GetNameSafe(this));
		return;
	}

	// Clear existing connections first to prevent duplicates
	if (!LinkedDeviceSenderComponents.IsEmpty())
	{
		UE_LOG(LogDeviceCollectorVolume, Warning, TEXT("[%s] already has linked devices. Clearing them before reconnecting."), *GetNameSafe(this));
		SendUnsubscribeCalls(InstigatorActor);
	}
	
	// allow overlap events to be generated
	GetBrushComponent()->SetGenerateOverlapEvents(true);
	
	// refresh what's inside the bounds of the volume
	GetBrushComponent()->UpdateOverlaps();

	// Get all the actors inside the bounds of the volume
	TArray<AActor*> FoundActors;
	GetBrushComponent()->GetOverlappingActors(FoundActors);

	// Turn off generated events
	GetBrushComponent()->SetGenerateOverlapEvents(false);

	// No actors found within the volume's boundaries'
	if (FoundActors.IsEmpty())
	{
		// No actors found
		UE_LOG(LogDeviceCollectorVolume, Warning, TEXT("[%s] found no actors within its boundaries"), *GetNameSafe(this));
		return;
	}

	// Map for easier registration
	TMap<EDeviceID, TArray<USenderComponent*>> SenderMap;

	// Store sender components in a map
	for (AActor* attachedActor : ActorsWithSenderComponents)
	{
		if (!IsValid(attachedActor))
		{
			UE_LOG(LogDeviceCollectorVolume, Error, TEXT("[%s] found an invalid actor attached to the array of this actor. Please look at the [ActorsWithSenderComponents] array inside of this actor"), *GetNameSafe(this));
		}
		else if (USenderComponent* SenderComponent = attachedActor->FindComponentByClass<USenderComponent>())
		{
			// Add sender component to the map
			SenderMap.FindOrAdd(SenderComponent->GetDeviceID()).Add(SenderComponent);

			// Add a reference into an array of linked devices for unsubscription reasons
			LinkedDeviceSenderComponents.Add(SenderComponent);

			// send a message to the owner of the component for registration
			if (attachedActor->Implements<URegistrationInterface>())
			{
				IRegistrationInterface::Execute_RegisterCallSent(attachedActor, InstigatorActor);
			}
		}
		else
		{
			UE_LOG(LogDeviceCollectorVolume, Warning, TEXT("%s found in array [ActorsWithSenderComponents] of [%s] doesn't have a sender component"), *GetNameSafe(attachedActor), *GetNameSafe(this));
		}

		// remove this actor if it exists in the array
		FoundActors.Remove(attachedActor);
	}

	for (int i = FoundActors.Num() - 1; i >= 0; i--)
	{
		// if it has a receiver component, add it to 
		if (UReceiverComponent* receiverComp = FoundActors[i]->FindComponentByClass<UReceiverComponent>())
		{
			if (SenderMap.Contains(receiverComp->GetDeviceID()))
			{
				for (USenderComponent* SenderComp : SenderMap[receiverComp->GetDeviceID()])
				{
					SenderComp->ReceiveComponentForRegistration(InstigatorActor, receiverComp);
					// The function from the sender components handles sending the registration call to the owner of the component
				}
			}
		}

		//delete the actor from the array after the process are complete
		FoundActors.RemoveAt(i);
	}
}

void ADeviceCollectorVolume::SendUnsubscribeCalls(AActor* InstigatorActor)
{
	if (LinkedDeviceSenderComponents.IsEmpty())
	{
		UE_LOG(LogDeviceCollectorVolume, Warning, TEXT("[%s] doesn't have linked devices"), *GetNameSafe(this));
		return;
	}

	// Send Unregister call to all linked devices
	for (int i = LinkedDeviceSenderComponents.Num() - 1; i >= 0; i--)
	{
		LinkedDeviceSenderComponents[i]->ClearLinkedDevices(InstigatorActor);

		if (LinkedDeviceSenderComponents[i]->GetOwner()->Implements<URegistrationInterface>())
		{
			IRegistrationInterface::Execute_UnRegisterCallSent(LinkedDeviceSenderComponents[i]->GetOwner(), InstigatorActor);
		}

		LinkedDeviceSenderComponents.RemoveAt(i);
	}
}
