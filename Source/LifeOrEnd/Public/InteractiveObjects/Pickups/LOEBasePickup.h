// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjects/LOEInteractionInterface.h"
#include "LOEBasePickup.generated.h"

class ULOEBaseItem;
class UCapsuleComponent;

UCLASS()
class LIFEOREND_API ALOEBasePickup : public AActor, public ILOEInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALOEBasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	ULOEBaseItem* ItemData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractWithMe(ALOECharacter* Character) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* _RootComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComponent;
};
