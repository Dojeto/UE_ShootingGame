// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMECOURSE_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:

		UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties(float DeltaTime);


		virtual void NativeInitializeAnimation() override;
	
	private:
		UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement,meta=(AllowPrivateAccess="true"))
		class AAGameCharacter* ShooterCharacter;
	

		//speed 
		UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement,meta=(AllowPrivateAccess="true"))
		float speed;
		UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement,meta=(AllowPrivateAccess="true"))
		bool bIsInAir;
		UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Movement,meta=(AllowPrivateAccess="true"))
		bool bISAccelerating;
};
