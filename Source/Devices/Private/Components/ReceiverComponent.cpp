#include "Components/ReceiverComponent.h"

#include "Components/SenderComponent.h"
#include "Data/EDeviceTypeEnum.h"

UReceiverComponent* UReceiverComponent::GetDeviceReceiverComponent(AActor* TargetActor)
{
	return TargetActor->FindComponentByClass<UReceiverComponent>();
}

UReceiverComponent::UReceiverComponent()
{
	// Set this device to be a receiver
	DeviceType = EDeviceType::Receiver;

	// Set this device to be registered by default
	bShouldBeRegistered = true;
	bCanReceiveCalls = bShouldBeRegistered;
}

void UReceiverComponent::SetLinkedSenderDevice(USenderComponent* DeviceSenderComponent)
{
	LinkedDeviceSenderComponent = DeviceSenderComponent;
}

void UReceiverComponent::UnRegisterFromCalls()
{
	if (IsValid(LinkedDeviceSenderComponent))
	{
		bShouldBeRegistered = false;
		bCanReceiveCalls = false;
		LinkedDeviceSenderComponent->ClearSingleLinkedDevice(this);
		LinkedDeviceSenderComponent = nullptr;
	}
	else
	{
		UE_LOG(LogDeviceComponent, Warning, TEXT("[%s]'s [%s] device failed to unsubscribe from calls"), *GetNameSafe(GetOwner()), *GetNameSafe(this));
	}
}

EDeviceSpecialID UReceiverComponent::GetDeviceSpecialID() const
{
	return DeviceSpecialID;
}

bool UReceiverComponent::ShouldBeRegistered() const
{
	return bShouldBeRegistered;
}

bool UReceiverComponent::CanReceiveCalls() const
{
	return bCanReceiveCalls;
}

void UReceiverComponent::FlipDeviceState(EReceiverState& ReceiverState)
{
	bShouldBeRegistered = !bShouldBeRegistered;
	bCanReceiveCalls = bShouldBeRegistered;

	ReceiverState = bShouldBeRegistered ? EReceiverState::CanReceiveCalls : EReceiverState::CanNotReceiveCalls;
}
