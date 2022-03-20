// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LOECharacter.h"

#include "Components/LOECharacterMovementComponent.h"

// Sets default values
ALOECharacter::ALOECharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<ULOECharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bWantsRunning = false;
}

// Called when the game starts or when spawned
void ALOECharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALOECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALOECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALOECharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALOECharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ALOECharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ALOECharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALOECharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALOECharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ALOECharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ALOECharacter::StopRun);
}

void ALOECharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ALOECharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}

void ALOECharacter::StartRun()
{
	bWantsRunning = true;
}

void ALOECharacter::StopRun()
{
	bWantsRunning = false;
}

bool ALOECharacter::IsRunning() const
{
	if (!GetCharacterMovement()) return false;

	return bWantsRunning && !GetVelocity().IsZero();
}

