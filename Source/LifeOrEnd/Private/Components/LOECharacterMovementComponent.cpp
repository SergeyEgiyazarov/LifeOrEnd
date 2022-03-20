// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LOECharacterMovementComponent.h"
#include "Player/LOECharacter.h"

float ULOECharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const ALOECharacter* LCharacterOwner = Cast<ALOECharacter>(GetOwner());
	if (LCharacterOwner)
	{
		if(LCharacterOwner->IsRunning())
		{
			MaxSpeed *= 1.8f;
		}
	}

	return MaxSpeed;
}
