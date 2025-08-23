#include "Data/EDeviceTypeEnum.h"
#include "Components/DeviceComponent.h"

EDeviceType UDeviceComponent::GetDeviceType() const
{
	return DeviceType;
}

EDeviceID UDeviceComponent::GetDeviceID() const
{
	return DeviceID;
}
