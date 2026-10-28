// Copyright Epic Games, Inc. All Rights Reserved.


#include "CXUI.h"
#include "Components/TextBlock.h"
#include "PlayerState/CXPlayerState.h"
#include "GameState/CXGameState.h"

void UCXUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerStateInfo)
	{
		if (ACXPlayerState* PS = Cast<ACXPlayerState>(GetOwningPlayerState()))
		{
			PlayerStateInfo->SetText(FText::FromString(PS->GetPlayerStateInfo()));
		}
	}

	UpdateTimerText();   // 추가
	UpdateResultText();
}

// 추가
void UCXUI::UpdateTimerText()
{
	if (TimerText == nullptr)
	{
		return;
	}

	UWorld* World = GetWorld();
	ACXGameState* CXGameState = World ? World->GetGameState<ACXGameState>() : nullptr;
	if (CXGameState == nullptr)
	{
		TimerText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	const int32 Time = CXGameState->GetRemainingTime();

	switch (CXGameState->GetCurrentGameState())
	{
	case EGameState::Wating:
		TimerText->SetText(FText::FromString(FString::Printf(TEXT("곧 시작: %d"), Time)));
		TimerText->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;

	case EGameState::Playing:
		TimerText->SetText(FText::FromString(FString::Printf(TEXT("남은 시간: %d"), Time)));
		TimerText->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;

	default:   // Finished: 결과 텍스트가 대신 뜨니까 숨김
		TimerText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}

void UCXUI::UpdateResultText()
{
	if (ResultText == nullptr)
	{
		return;
	}

	UWorld* World = GetWorld();
	ACXGameState* CXGameState = World ? World->GetGameState<ACXGameState>() : nullptr;

	// 게임이 끝나기 전에는 숨기기
	if (CXGameState == nullptr || CXGameState->GetCurrentGameState() != EGameState::Finished)
	{
		ResultText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 이 UI의 주인 = 이 화면을 보고 있는 로컬 플레이어
	ACXPlayerState* MyPlayerState = Cast<ACXPlayerState>(GetOwningPlayerState());
	if (MyPlayerState == nullptr)
	{
		return;
	}

	const bool bIsWinner = CXGameState->Winners.Contains(MyPlayerState);
	ResultText->SetText(FText::FromString(bIsWinner ? TEXT("승리!") : TEXT("패배...")));
	ResultText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCXUI::OnNewChatMessage(const FString& InSender, const FString& InMessage)
{
	UI_Chat->OnNewChatMessage(InSender, InMessage);
}

void UCXUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCXUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}