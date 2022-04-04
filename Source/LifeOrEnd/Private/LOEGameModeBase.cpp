// Fill out your copyright notice in the Description page of Project Settings.


#include "LOEGameModeBase.h"

#include "LOEPlayerController.h"
#include "UI/LOEGameHUD.h"
#include "Player/LOECharacter.h"

ALOEGameModeBase::ALOEGameModeBase()
{
	DefaultPawnClass = ALOECharacter::StaticClass();
	PlayerControllerClass = ALOEPlayerController::StaticClass();
	HUDClass = ALOEGameHUD::StaticClass();
}
