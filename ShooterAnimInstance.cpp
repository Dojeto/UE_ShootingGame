// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "AGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    if(ShooterCharacter == nullptr)
    {
        ShooterCharacter = Cast<AAGameCharacter>(TryGetPawnOwner());
    }   
    if(ShooterCharacter)
    {
        FVector Velocity {ShooterCharacter->GetVelocity()};
        Velocity.Z = 0;
        speed  = Velocity.Size();

        // character in the air 
        bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

        //is character is accelerating

        if(ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
        {
            bISAccelerating = true;
        }
        else
        {
            bISAccelerating = false;
        }

    }
}

void UShooterAnimInstance:: NativeInitializeAnimation()
{
    ShooterCharacter = Cast<AAGameCharacter>(TryGetPawnOwner());

}