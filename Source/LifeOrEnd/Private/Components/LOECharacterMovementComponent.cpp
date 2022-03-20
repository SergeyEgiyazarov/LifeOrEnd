// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LOECharacterMovementComponent.h"
#include "Player/LOECharacter.h"

float ULOECharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const ALOECharacter* LOECharacterOwner = Cast<ALOECharacter>(GetOwner());
	if (LOECharacterOwner)
	{
		if(LOECharacterOwner->IsRunning())
		{
			MaxSpeed *= 1.8f;
		}
	}

	return MaxSpeed;
}
