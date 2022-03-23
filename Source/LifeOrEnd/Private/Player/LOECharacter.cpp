// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LOECharacter.h"

#include "LOEBaseItem.h"
#include "Weapons/LOEBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/LOECharacterMovementComponent.h"

// Sets default values
ALOECharacter::ALOECharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<ULOECharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;
	
	GetMesh()->SetupAttachment(CameraComponent);
	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->bOwnerNoSee = false;
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;

	CrouchTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimelineComponent"));

	CurrentMovementState = EMovementState::Idle;
}

// Called when the game starts or when spawned
void ALOECharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateCharacterHeightTrack.BindDynamic(this, &ALOECharacter::UpdateCharacterHeight);
	
	if (CrouchHeightCurve)
	{
		SetupCrouchCurve();
		CrouchTimelineComp->AddInterpFloat(CrouchHeightCurve, UpdateCharacterHeightTrack);
	}
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
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ALOECharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ALOECharacter::StopCrouch);
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
	if (CurrentMovementState == EMovementState::Crouch)
	{
		StopCrouch();
	}
	CurrentMovementState = EMovementState::Running;
}

void ALOECharacter::StopRun()
{
	CurrentMovementState  = EMovementState::Idle;
}

EMovementState ALOECharacter::GetMovementState() const
{
	if (!GetCharacterMovement() && GetVelocity().IsZero()) return EMovementState::Idle;
	return CurrentMovementState;
}

void ALOECharacter::SetupCrouchCurve()
{
	const float CurrentHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FRichCurveKey FirstKey = FRichCurveKey(0.0f, CurrentHeight);
	const FRichCurveKey SecondKey = FRichCurveKey(CrouchAnimationTime, CrouchHeight);
	const TArray<FRichCurveKey> RichCurveKeys = {FirstKey, SecondKey};
	CrouchHeightCurve->FloatCurve.SetKeys(RichCurveKeys);
}

void ALOECharacter::StartCrouch()
{
	if(CurrentMovementState != EMovementState::Idle) return;
	CurrentMovementState = EMovementState::Crouch;
	CrouchTimelineComp->Play();
}

void ALOECharacter::StopCrouch()
{
	if(CurrentMovementState != EMovementState::Crouch) return;
	CurrentMovementState = EMovementState::Idle;
	CrouchTimelineComp->Reverse();
}

void ALOECharacter::UpdateCharacterHeight(float Height)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(Height);
}

void ALOECharacter::SpawnWeaponToSocket(const ULOEBaseItem* ItemWeapon)
{
	if(!ItemWeapon) return;
	//const ALOEBaseWeapon* Weapon = Cast<ALOEBaseWeapon>(ItemWeapon->ItemActor);
	const auto Weapon = ItemWeapon->ItemActor;
	if (Weapon)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ALOEBaseWeapon>(Weapon);
		const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRule, "GripPoint");
	}
}
