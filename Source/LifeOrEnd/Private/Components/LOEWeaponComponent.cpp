// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LOEWeaponComponent.h"
#include "Items/LOEBaseItem.h"
#include "Weapons/LOEBaseWeapon.h"
#include "GameFramework/Character.h"

ULOEWeaponComponent::ULOEWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void ULOEWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void ULOEWeaponComponent::SpawnWeapon(const ULOEBaseItem* ItemWeapon)
{
	if (!ItemWeapon || !GetWorld()) return;
	CurrentWeapon = GetWorld()->SpawnActor<ALOEBaseWeapon>(ItemWeapon->ItemActor);
	if (!CurrentWeapon) return;
	AttachWeaponToSocket(CurrentWeapon);
}

void ULOEWeaponComponent::AttachWeaponToSocket(ALOEBaseWeapon* Weapon)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || !Weapon) return;
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Character->GetMesh(), AttachmentRule, WeaponAttachPointName);
	Weapon->SetOwner(Character);
}

void ULOEWeaponComponent::Fire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->Fire();
}


