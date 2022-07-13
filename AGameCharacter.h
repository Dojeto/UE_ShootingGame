// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AGameCharacter.generated.h"

UCLASS()
class GAMECOURSE_API AAGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnyWhere,BluePrintReadOnly, Category = Camera,meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnyWhere,BluePrintReadOnly, Category = Camera,meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	FORCEINLINE USpringArmComponent*GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent*GetFollowCamera() const { return FollowCamera; }

	// called for forward
	void MoveForward(float value);

	//called or roght
	void MoveRight (float value );
};