// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CXItemSpawner.h"
#include "Components/BillboardComponent.h"   // 수정: Component → Components, Bilboard → Billboard

// Sets default values
ACXItemSpawner::ACXItemSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BilboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Bilboard"));   // 수정: UBillboardComponent
	SetRootComponent(BilboardComponent);   // 추가
}

void ACXItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnItem();
	}
}

void ACXItemSpawner::SpawnItem()
{
	if (!HasAuthority() || IsValid(ItemClass) == false)
	{
		return;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	SpawnedItem = World->SpawnActorDeferred<AActor>(ItemClass, GetActorTransform(), this,
		nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	SpawnedItem->OnDestroyed.AddDynamic(this, &ACXItemSpawner::OnItemDestroyed);
	SpawnedItem->FinishSpawning(GetActorTransform());
}

void ACXItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	if (!HasAuthority() || IsValid(ItemClass) == false)
	{
		return;
	}

	SpawnedItem = nullptr;

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACXItemSpawner::SpawnItem, SpawnInterval, false);
}