// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LOECharacterMovementComponent.h"
#include "Player/LOECharacter.h"

float ULOECharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const ALOECharacter* LCharacterOwner = Cast<ALOECharacter>(GetOwner());
	if (LCharacterOwner)
	{
		const auto State = LCharacterOwner->GetMovementState();

		switch (State)
		{
		case EMovementState::Running:
			MaxSpeed *= 1.8f;
			break;
		case EMovementState::Crouch:
			MaxSpeed *= 0.5f;
			break;
		default:
			break;
		}
	}

	return MaxSpeed;
}
