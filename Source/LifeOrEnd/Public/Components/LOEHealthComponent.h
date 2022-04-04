// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LOEHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIFEOREND_API ULOEHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULOEHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const {return Health;}

	bool IsDead() const {return Health <= 0.0f;}

	FOnDeath OnDeath;
	FOnHealthChange OnHealthChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0" , ClampMax = "1000.0"))
	float MaxHealth = 100.0f;
private:
	UPROPERTY()
	float Health = 0.0f;
		
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
