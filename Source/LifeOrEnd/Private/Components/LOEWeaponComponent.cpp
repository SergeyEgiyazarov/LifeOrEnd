// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LOEWeaponComponent.h"

#include "Items/LOEBaseItem.h"
#include "Weapons/LOEBaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
ULOEWeaponComponent::ULOEWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULOEWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULOEWeaponComponent::SpawnWeapon(const ULOEBaseItem* ItemWeapon)
{
	if (!ItemWeapon) return;
	const auto Weapon = GetWorld()->SpawnActor<ALOEBaseWeapon>(ItemWeapon->ItemActor);
	if (!Weapon) return;
	AttachWeaponToSocket(Weapon);
}

void ULOEWeaponComponent::AttachWeaponToSocket(ALOEBaseWeapon* Weapon)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || !Weapon) return;
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Character->GetMesh(), AttachmentRule, "GripPoint");
}


