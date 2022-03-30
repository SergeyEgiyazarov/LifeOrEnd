// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObjects/Pickups/LOEBasePickup.h"
#include "LOECharacter.h"
//#include "Items/LOEBaseItem.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ALOEBasePickup::ALOEBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = _RootComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CollisionComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALOEBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALOEBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALOEBasePickup::InteractWithMe(ALOECharacter* Character)
{
	if (Character)
	{
		Character->SpawnWeaponToSocket(ItemData);
	}
	Destroy();
}

