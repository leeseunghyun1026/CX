#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXCoinItem.generated.h"

class USceneComponent;
class UPrimitiveComponent;
class USphereComponent;
class UStaticMeshComponent;   // 추가
class UParticleSystem;
class USoundBase;

UCLASS()
class CX_API ACXCoinItem : public AActor
{
	GENERATED_BODY()

public:
	ACXCoinItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)   // 수정: UnReliable → Reliable
		void MulticastPickupEffects();

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	TObjectPtr<USceneComponent> SceneRoot;   // 수정: Root → SceneRoot

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	TObjectPtr<USphereComponent> OverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin")   // 수정: VisibleAnywhere → EditAnywhere
		TObjectPtr<UParticleSystem> PickupParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin")   // 수정: VisibleAnywhere → EditAnywhere
		TObjectPtr<USoundBase> PickupSound;
};