// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LOEInteractionInterface.generated.h"

class ALOECharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULOEInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIFEOREND_API ILOEInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InteractWithMe(ALOECharacter* Character) = 0;
};
