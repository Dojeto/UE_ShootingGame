// Fill out your copyright notice in the Description page of Project Settings.


#include "AGameCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AAGameCharacter::AAGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating cameraboom (spring arm)

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength= 300.0f ;
	CameraBoom -> bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


}

// Called when the game starts or when spawned
void AAGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp,Warning,TEXT("Begin Play Called ..."));

	int myInt { 64 };

	UE_LOG(LogTemp,Error,TEXT("Here is myInt : %d"),myInt);

	FString myStr { "Hello" };

	//Fstring is a structure (struct)

	UE_LOG(LogTemp,Warning ,TEXT("lmao : %s"),*myStr);

	// here * is not a pointer it is a Oprator overloading it's convert f string into character arrey (string)

	UE_LOG(LogTemp ,Warning,TEXT("GetName Test : %s"),*GetName());

}

// Called every frame
void AAGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&AAGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AAGameCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookRateRight",this,&AAGameCharacter::LookRateRight);
	PlayerInputComponent->BindAxis("LookRateUp",this,&AAGameCharacter::LookRateUp);
	PlayerInputComponent->BindAxis("MouseUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MouseTurn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);
}

void AAGameCharacter :: MoveForward(float value)
{

	//** This Way also Work **

	// if((Controller!=nullptr) && (value != 0) )
	// {
	// 	const FRotator TestRoatation{ Controller->GetControlRotation() };

	// 	const FRotator JkYawRotation { 0,TestRoatation.Yaw,0 } ;	

	// 	const FVector Direction { FRotationMatrix{JkYawRotation}.GetUnitAxes(EAxis::X) };	

	// 	AddMovementInput(Direction,value);
	// }

	AddMovementInput(GetActorForwardVector()* value);
}

void AAGameCharacter :: MoveRight(float value)
{
	AddMovementInput(GetActorRightVector()* value);
}

void AAGameCharacter:: LookRateRight(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate *GetWorld()->DeltaTimeSeconds);
}

void AAGameCharacter:: LookRateUp(float Rate)
{
	AddControllerPitchInput(Rate * BaseUpRate *GetWorld()->DeltaTimeSeconds);
}