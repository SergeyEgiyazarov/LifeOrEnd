// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LOEBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponLog, All, All);

// Sets default values
ALOEBaseWeapon::ALOEBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->bCastDynamicShadow = false;
	Mesh->CastShadow = false;
}

// Called when the game starts or when spawned
void ALOEBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALOEBaseWeapon::Fire()
{
	UE_LOG(WeaponLog, Display, TEXT("I can fire!"));

	MakeShot();
}

void ALOEBaseWeapon::MakeShot()
{
	if (!GetWorld()) return;

	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	const FTransform SocketTransform = Mesh->GetSocketTransform(MuzzleSocketName);
	const FVector StartTrace = ViewLocation;
	const FVector DirectionTrace = ViewRotation.Vector();
	const FVector EndTrace = StartTrace + DirectionTrace * 1500.0f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, CollisionParams);

	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(),SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint, 10.0f, 24, FColor::Green, false, 2.0f);
		//Only Test, delete this
		if (HitResult.Actor->ActorHasTag("Enemy"))
		{
			HitResult.Actor->Destroy();
		}
		//...
	}
	else
	{
		DrawDebugLine(GetWorld(),SocketTransform.GetLocation(), EndTrace, FColor::Green, false, 2.0f, 0, 1.0f);
	}
}

















