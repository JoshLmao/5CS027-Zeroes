// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "UObject\ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Heroes\HeroBase.h"
#include "Heroes/HeroState.h"
#include "UI\PlayerUIWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BlueprintClass(TEXT("/Game/UI/PlayerUI_BP"));
	if (BlueprintClass.Class)
		HUDWidgetClass = BlueprintClass.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
			CurrentWidget->AddToViewport();
	}

	AHeroBase* player = Cast<AHeroBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (player)
	{
		AHeroState* state = Cast<AHeroState>(player->GetPlayerState());
		if (state)
		{
			HeroState = state;

			UPlayerUIWidget* widget = Cast<UPlayerUIWidget>(CurrentWidget);
			widget->PlayerState = HeroState;
		}
	}
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if(HeroState)
	{
		Health = HeroState->GetHealth();
	}
}