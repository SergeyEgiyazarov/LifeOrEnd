// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LOECharacter.h"
#include "Items/LOEBaseItem.h"
#include "Weapons/LOEBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/LOECharacterMovementComponent.h"
#include "Components/LOEWeaponComponent.h"
#include "Components/LOEHealthComponent.h"
#include "InteractiveObjects/LOEInteractionInterface.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerCharacterLog, All, All);

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

	HealthComponent = CreateDefaultSubobject<ULOEHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<ULOEWeaponComponent>(TEXT("WeaponComponent"));

	CrouchTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimelineComponent"));

	//Default Movement State Player 
	CurrentMovementState = EMovementState::Idle;
}

// Called when the game starts or when spawned
void ALOECharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddUObject(this, &ALOECharacter::OnDeath);
	HealthComponent->OnHealthChange.AddUObject(this, &ALOECharacter::OnHealthChange);
	LandedDelegate.AddDynamic(this, &ALOECharacter::OnGroundLanded);

	/*Bind crouch animation*/
	UpdateCharacterHeightTrack.BindDynamic(this, &ALOECharacter::UpdateCharacterHeight);
	if (CrouchHeightCurve)
	{
		SetupCrouchCurve();
		CrouchTimelineComp->AddInterpFloat(CrouchHeightCurve, UpdateCharacterHeightTrack);
	}
	/**/
}

// Called every frame
void ALOECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractTrace();

}

void ALOECharacter::OnHealthChange(float Health)
{
	UE_LOG(PlayerCharacterLog, Display, TEXT("Health changed: %f"), Health);
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

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALOECharacter::InteractWithObject);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULOEWeaponComponent::Fire);
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

void ALOECharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	UE_LOG(PlayerCharacterLog, Display, TEXT("Fall Velocity: %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(PlayerCharacterLog, Display, TEXT("Take damage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ALOECharacter::OnDeath()
{
	UE_LOG(PlayerCharacterLog, Display, TEXT("Player: %s is dead"), *GetName());

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(3.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALOECharacter::SpawnWeaponToSocket(const ULOEBaseItem* ItemWeapon)
{
	WeaponComponent->SpawnWeapon(ItemWeapon);
	/*if(!ItemWeapon) return;
	//const ALOEBaseWeapon* Weapon = Cast<ALOEBaseWeapon>(ItemWeapon->ItemActor);
	const auto Weapon = ItemWeapon->ItemActor;
	if (Weapon)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ALOEBaseWeapon>(Weapon);
		const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRule, "GripPoint");
	}*/
}

//Line trace for check interact object
void ALOECharacter::InteractTrace()
{
	if(!Controller) return;
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FVector StartTrace;
	FRotator RotatorTrace;
	GetController()->GetPlayerViewPoint(StartTrace, RotatorTrace);
	FVector EndTrace = StartTrace + RotatorTrace.Vector() * 200.0f;
	
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 1.0f);
	
	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5.0f, 5.0f, 5.0f), FColor::Red, false, 1.5f);
		LookAtActor = Cast<ILOEInteractionInterface>(Hit.GetActor());
		if (LookAtActor)
		{
			isLookAt = true;
		}
	}
	else
	{
		isLookAt = false;
	}
	
}

void ALOECharacter::InteractWithObject()
{
	if (LookAtActor)
	{
		LookAtActor->InteractWithMe(this);
	}
}

