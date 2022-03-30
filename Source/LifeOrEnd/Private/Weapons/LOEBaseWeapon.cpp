// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LOEBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

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
