// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "LOEBaseItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class LIFEOREND_API ULOEBaseItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULOEBaseItem(){}
	
	//Type of this item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FSlateBrush ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<AActor> ItemActor;
	
};
