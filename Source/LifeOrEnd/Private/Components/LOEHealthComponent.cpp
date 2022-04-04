// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LOEHealthComponent.h"

// Sets default values for this component's properties
ULOEHealthComponent::ULOEHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULOEHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChange.Broadcast(Health);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &ULOEHealthComponent::OnTakeAnyDamage);
	}
}

void ULOEHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage < 0.0f || IsDead()) return;
	
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChange.Broadcast(Health);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

