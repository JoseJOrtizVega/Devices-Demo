#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RegistrationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URegistrationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface responsible for sending special registration calls to the owners of [ReceiverComponents] and [SenderComponents]
 */
class DEVICES_API IRegistrationInterface
{
	GENERATED_BODY()

public:
	/**
	 * Function responsible for sending a registration call to the owner of this interface
	 * @param InstigatorActor Whoever initiated the registration event
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Device Registration Interface")
	void RegisterCallSent(AActor* InstigatorActor);

	/**
	 * Function responsible for sending an unregistration call to the owner of this interface
	 * @param InstigatorActor Whoever initiated the registration event
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Device Registration Interface")
	void UnRegisterCallSent(AActor* InstigatorActor);
};
