// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "LOECharacter.generated.h"

UCLASS()
class LIFEOREND_API ALOECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALOECharacter(const FObjectInitializer& ObjInit);
	
protected:
	//TimelineComponent to animate crouch
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement|Crouch")
	UTimelineComponent* CrouchTimelineComp;

	FOnTimelineFloat UpdateCharacterHeightTrack;
	
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

	bool IsRunning() const;

private:
	bool bWantsRunning;

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

};
