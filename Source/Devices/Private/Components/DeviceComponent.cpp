#include "Components/DeviceComponent.h"
#include "Data/EDeviceTypeEnum.h"

EDeviceType UDeviceComponent::GetDeviceType() const
{
	return DeviceType;
}

EDeviceID UDeviceComponent::GetDeviceID() const
{
	return DeviceID;
}
