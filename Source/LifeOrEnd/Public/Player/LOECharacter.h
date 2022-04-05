// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "LOECharacter.generated.h"

class UCameraComponent;
class ALOEBaseWeapon;
class ULOEBaseItem;
class ULOEWeaponComponent;
class ULOEHealthComponent;
class ILOEInteractionInterface;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle = 0,
	Running,
	Crouch
};

UCLASS()
class LIFEOREND_API ALOECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALOECharacter(const FObjectInitializer& ObjInit);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULOEHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULOEWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState CurrentMovementState;
	
	//TimelineComponent to animate crouch
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement|Crouch")
	UTimelineComponent* CrouchTimelineComp;

	FOnTimelineFloat UpdateCharacterHeightTrack;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity = FVector2D(700.0f, 1000.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch", meta = (ClampMin = 20.0f, ClampMax = 100.0f))
	float CrouchHeight = 55.0f;
	//Time animation for crouch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch", meta = (ClampMin = 0.1f, ClampMax = 3.0f))
	float CrouchAnimationTime = 0.5f;
	//Curve float for crouch animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch")
	UCurveFloat* CrouchHeightCurve;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	EMovementState GetMovementState() const;

	//Spawn weapon and attach to socket
	UFUNCTION(BlueprintCallable)
	void SpawnWeaponToSocket(const ULOEBaseItem* ItemWeapon);

	UPROPERTY(BlueprintReadWrite)
	bool isLookAt = false;
private:

	ILOEInteractionInterface* LookAtActor;
	
	//Movement functions
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	void StartCrouch();
	void StopCrouch();
	void SetupCrouchCurve();
	UFUNCTION()
	void UpdateCharacterHeight(float Height);
	
	void OnDeath();
	void OnHealthChange(float Health);
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	//Line trace for check interact object
	void InteractTrace();
	void InteractWithObject();

};
