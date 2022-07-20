// Fill out your copyright notice in the Description page of Project Settings.


#include "AGameCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

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


	//Don't Roatate your character only camera will rotate
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,54.0f,0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl=0.2f;
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
	PlayerInputComponent->BindAction("FireButton",IE_Pressed,this,&AAGameCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Reaload",IE_Pressed,this,&AAGameCharacter::Reaload);
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

void AAGameCharacter :: FireWeapon()
{
	Ammo = Ammo - 1;

	if(Ammo >0)
	{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this,FireSound);
	}

	const USkeletalMeshSocket * BarrelSocket = GetMesh()->GetSocketByName("Muzzel");

	if(BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if(MuzzeleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),MuzzeleFlash,SocketTransform);
		}

		FHitResult FireHit;

		const FVector Start{SocketTransform.GetLocation()};

		const FQuat Roatation{SocketTransform.GetRotation()};

		const FVector RoatationAxix {Roatation.GetAxisX()};

		const FVector End {Start + RoatationAxix * 5000.0f};

		FVector BeamEndPoint { End };

		GetWorld()->LineTraceSingleByChannel(FireHit,Start,End , ECollisionChannel::ECC_Visibility);

		if(FireHit.bBlockingHit)
		{
			if(Impact)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Impact,FireHit.Location);
			}
			// ** For DeBug Line ** //

			// DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,2.f);
			// DrawDebugPoint(GetWorld(),FireHit.Location,5.f,FColor::Red,false,2.f);

			BeamEndPoint = FireHit.Location;

			if (SmokeParticle)
			{
				UParticleSystemComponent * Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SmokeParticle,SocketTransform);
				if(Beam)
				{
					Beam->SetVectorParameter(FName("Target"),BeamEndPoint);
				}
			}
		}
	}

	UAnimInstance * AnimeInstance = GetMesh()->GetAnimInstance();
	if(AnimeInstance && HipFireMontage)
	{
		AnimeInstance->Montage_Play(HipFireMontage);
		AnimeInstance->Montage_JumpToSection(FName("Start"));
	}
	}
}

void AAGameCharacter::Reaload()
{
	if(Ammo<=10)
	{
	UAnimInstance* RealoadAnimeInstance = GetMesh()->GetAnimInstance();
	RealoadAnimeInstance->Montage_Play(AmmoReaload);
	RealoadAnimeInstance->Montage_JumpToSection(FName("Start"));
	Ammo = 10.0f;
	}
}