// Fill out your copyright notice in the Description page of Project Settings.


#include "LOEGameModeBase.h"
#include "Player/LOECharacter.h"

ALOEGameModeBase::ALOEGameModeBase()
{
	DefaultPawnClass = ALOECharacter::StaticClass();
}
