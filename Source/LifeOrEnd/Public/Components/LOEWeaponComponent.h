// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LOEWeaponComponent.generated.h"

class ALOEBaseWeapon;
class ULOEBaseItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIFEOREND_API ULOEWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULOEWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	ALOEBaseWeapon* CurrentWeapon = nullptr;

	void SpawnWeapon(const ULOEBaseItem* ItemWeapon);

private:
	void AttachWeaponToSocket(ALOEBaseWeapon* Weapon);
};
