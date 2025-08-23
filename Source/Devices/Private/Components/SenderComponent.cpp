#include "Components/SenderComponent.h"
#include "Components/ReceiverComponent.h"
#include "Data/EDeviceTypeEnum.h"
#include "Interfaces/RegistrationInterface.h"

USenderComponent* USenderComponent::GetDeviceSenderComponent(AActor* TargetActor)
{
	return TargetActor->FindComponentByClass<USenderComponent>();
}

USenderComponent::USenderComponent()
{
	// Set this device to be a sender
	DeviceType = EDeviceType::Sender;

	// set this device's default state to not be processing calls
	bIsProcessingCalls = false;
}

void USenderComponent::ReceiveComponentForRegistration(AActor* InstigatorActor, UReceiverComponent* DeviceReceiverComponent)
{
	if (IsValid(DeviceReceiverComponent))
	{
		// Add device to array
		LinkedDeviceReceiverComponents.Add(DeviceReceiverComponent);

		//Give Device a reference of the sender
		DeviceReceiverComponent->SetLinkedSenderDevice(this);

		// Send registration message to the owner of the components
		if (AActor* ReceiverActor = DeviceReceiverComponent->GetOwner())
		{
			if (ReceiverActor->Implements<URegistrationInterface>())
			{
				// Send registration message if the owner implements the interface 
				IRegistrationInterface::Execute_RegisterCallSent(ReceiverActor, InstigatorActor);
			}
		}
	}
	else
	{
		UE_LOG(LogDeviceComponent, Warning, TEXT("[%s]'s [%s] received an invalid receiver actor for registration"), *GetNameSafe(GetOwner()), *GetNameSafe(this));
	}
}

void USenderComponent::SendGenericCalls(AActor* InstigatorActor)
{
	// Set flag
	bIsProcessingCalls = true;
	
	// Send Calls to all devices connected to this one
	for (UReceiverComponent* comp : LinkedDeviceReceiverComponents)
	{
		if (comp->CanReceiveCalls())
		{
			comp->OnCallReceived.Broadcast(InstigatorActor);
		}
	}

	// Clear flag
	bIsProcessingCalls = false;
}

TArray<UReceiverComponent*> USenderComponent::GetLinkedDevices()
{
	return LinkedDeviceReceiverComponents;
}

void USenderComponent::ClearLinkedDevices(AActor* InstigatorActor)
{
	if (!LinkedDeviceReceiverComponents.IsEmpty())
	{
		for (int i = LinkedDeviceReceiverComponents.Num() - 1; i >= 0; i--)
		{
			UReceiverComponent* comp = LinkedDeviceReceiverComponents[i];

			// Invalid component skip
			if (!IsValid(comp))
			{
				UE_LOG(LogDeviceComponent, Warning, TEXT("[%s]'s [%s] device failed to clear linked devices"), *GetNameSafe(GetOwner()), *GetNameSafe(this));
				LinkedDeviceReceiverComponents.RemoveAt(i);
				return;
			}

			// Send Unregister message to the owner of the components, and remove the component from the list
			if (AActor* OwnerOfComp = comp->GetOwner())
			{
				if (OwnerOfComp->Implements<URegistrationInterface>())
				{
					IRegistrationInterface::Execute_UnRegisterCallSent(OwnerOfComp, InstigatorActor);
				}
			}

			// Remove device from array
			LinkedDeviceReceiverComponents.RemoveAt(i);
		}
	}
	else
	{
		UE_LOG(LogDeviceComponent, Warning, TEXT("[%s]'s [%s] device failed to clear linked devices"), *GetNameSafe(GetOwner()), *GetNameSafe(this));
	}
}

void USenderComponent::ClearSingleLinkedDevice(UReceiverComponent* DeviceReceiverComponent)
{
	if (LinkedDeviceReceiverComponents.IsEmpty())
	{
		UE_LOG(LogDeviceComponent, Warning, TEXT("[%s]'s [%s] device failed to clear linked devices"), *GetNameSafe(GetOwner()), *GetNameSafe(this));
		return;
	}

	// Return if processes are still occurring - add to queue instead
	if (bIsProcessingCalls)
	{
		// Add to queue if not already there
		if (!PendingUnregisterQueue.Contains(DeviceReceiverComponent))
		{
			PendingUnregisterQueue.Add(DeviceReceiverComponent);
			
			// Start the queue processing timer if it's not already running
			if (!GetWorld()->GetTimerManager().IsTimerActive(UnregisterQueueTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(UnregisterQueueTimerHandle, this, &USenderComponent::ProcessUnregisterQueue, 0.1f, true);
			}
		}
		return;
	}

	// Process immediately if not busy
	ProcessSingleUnregister(DeviceReceiverComponent);
}

void USenderComponent::ProcessUnregisterQueue()
{
	// If still processing, wait for next tick
	if (bIsProcessingCalls)
	{
		return;
	}

	// Process all pending unregisters
	for (int32 i = PendingUnregisterQueue.Num() - 1; i >= 0; i--)
	{
		UReceiverComponent* DeviceToUnregister = PendingUnregisterQueue[i];
		
		if (IsValid(DeviceToUnregister))
		{
			ProcessSingleUnregister(DeviceToUnregister);
		}
		
		PendingUnregisterQueue.RemoveAt(i);
	}

	// Clear the timer since queue is empty
	GetWorld()->GetTimerManager().ClearTimer(UnregisterQueueTimerHandle);
}

void USenderComponent::ProcessSingleUnregister(UReceiverComponent* DeviceReceiverComponent)
{
	// Send unsubscribe message and remove from array
	if (LinkedDeviceReceiverComponents.Contains(DeviceReceiverComponent))
	{
		if (AActor* OwnerOfComp = DeviceReceiverComponent->GetOwner())
		{
			if (OwnerOfComp->Implements<URegistrationInterface>())
			{
				// Send the Sender as the instigator instead of whoever instigated the call. This is because we want to allow the receivers that where connected to get the ability to perform actions on the owner of the sender component.
				IRegistrationInterface::Execute_UnRegisterCallSent(OwnerOfComp, OwnerOfComp);
			}
		}
		
		LinkedDeviceReceiverComponents.Remove(DeviceReceiverComponent);
	}
	else
	{
		UE_LOG(LogDeviceComponent, Warning, TEXT("[%s]'s [%s] device failed to clear linked devices"), *GetNameSafe(GetOwner()), *GetNameSafe(this));
	}
}
