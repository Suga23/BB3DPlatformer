// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.h"
#include "MeleeWeapon.h"
#include "Projectile.h"
#include "WeaponAttackInterface.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "DefendSkillComponent.h"
#include "AttackSkillComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() :
	bCanAttack(true),
	bAttackButtonPressed(false),

	// Movement
	BaseMovementSpeed(600.f),
	CrouchMovementSpeed(300.f),
	StandingCapsuleHalfHeight(88.f),
	CrouchingCapsuleHalfHeight(55.f),
	bCrouching(false),
	BaseGroundFriction(2.f),
	CrouchingGroundFriction(100.f), // „тобы убедитьс€, что персонаж не будет скользить.
	Health(100.f),
	MaxHealth(100.f),
	MagicShield(0.f),

	CombatState(ECombatState::ECS_Unoccupied)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	SideCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	SideCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideCameraComponent->bUsePawnControlRotation = false;

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::KeepRelative, // ќтносительное расположение.
		true); // ѕрикрепление к телу(к руке).

	ProjectileShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Shoot Point"));
	ProjectileShootPoint->SetupAttachment(RootComponent);

	ProjectileShootPoint->AttachToComponent(
		GetMesh(),
		AttachmentRules,
		FName(TEXT("spine_03")));

	DefendSkill = CreateDefaultSubobject<UDefendSkillComponent>(TEXT("DefendSkillComponent"));
	DefendSkill->SetIsReplicated(true);

	AttackSkill = CreateDefaultSubobject<UAttackSkillComponent>(TEXT("AttackSkillComponent"));
	AttackSkill->SetIsReplicated(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	EquipWeapon(EquipDefaultWeapon());
	EquippedWeapon->SetCharacter(this);

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EIS_Melee)
	{
		auto EquippedWea = Cast<AMeleeWeapon>(EquippedWeapon);
		EquippedWea->GetMeleeWeaponCollision()->OnComponentBeginOverlap.AddDynamic(
		this,
		&APlayerCharacter::OnMeleeWeaponOverlap);
	}

	PlayerCharController = Cast<APlayerController>(APawn::GetController());
}

void APlayerCharacter::CharacterMove(float Value)
{
	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
}

void APlayerCharacter::StartJumping()
{
	if (bCrouching)
	{
		bCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
	else
	{
		ACharacter::Jump();
	}
}

void APlayerCharacter::StartAttack()
{
	if (EquippedWeapon == nullptr) return;
	GetWeaponTypeProperties(EquippedWeapon->GetWeaponType(), EquippedWeapon);
	StartCooldownTimer();
}

void APlayerCharacter::AttackButtonPressed()
{
	/*if (bFirstSkillButtonPressed)
	{
		UseFirstSkillMontage();
		bFirstSkillButtonPressed = false;
	}
	else
	{*/
		bAttackButtonPressed = true;
		StartAttack();
	//}
}

void APlayerCharacter::AttackButtonReleased()
{
	bAttackButtonPressed = false;
}

void APlayerCharacter::FirstSkillButtonPressed()
{
	UseFirstSkillMontage();
	//bFirstSkillButtonPressed = true;
}

void APlayerCharacter::SecondSkillButtonPressed()
{
	UseSecondSkillMontage();
}

AWeapon* APlayerCharacter::EquipDefaultWeapon()
{
	if (EquippedWeaponClass)
	{
		return GetWorld()->SpawnActor<AWeapon>(EquippedWeaponClass);
	}
	return nullptr;
}

void APlayerCharacter::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(
			FName("RightHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
}

void APlayerCharacter::GetWeaponTypeProperties(EWeaponType WeaponT, AWeapon* EquippedW)
{
	IWeaponAttackInterface* WeaponAttackInterface = Cast<IWeaponAttackInterface>(EquippedW);

	switch (WeaponT)
	{
	case EWeaponType::EIS_Melee:
		/*TODO: создание сферы, чей диапазон будет равен переменной AttackRange.
		ƒобавить CollisionBoxes.*/
		
		if (WeaponAttackInterface)
		{
			WeaponAttackInterface->WeaponAttack_Implementation(this, EquippedW, this, PlayerCharController);
		}
		break;

	case EWeaponType::EIS_Range:
		if (WeaponAttackInterface)
		{
			WeaponAttackInterface->WeaponAttack_Implementation(this, EquippedW, this, PlayerCharController);
		}
		break;
	}
}

void APlayerCharacter::StartCooldownTimer()
{
	if (EquippedWeapon == nullptr) return;
	CombatState = ECombatState::ECS_FireTimerInProgress;
	bCanAttack = false;

	GetWorldTimerManager().SetTimer(
		AttackTimer,
		this,
		&APlayerCharacter::ResetAttack,
		EquippedWeapon->GetCooldownTimer());
}

void APlayerCharacter::ResetAttack()
{
	if (CombatState == ECombatState::ECS_Stunned) return;
	CombatState = ECombatState::ECS_Unoccupied;
	
	if (EquippedWeapon == nullptr) return;
	
	/* ¬озможно будут добавлены скиллы, которые стрел€ют очеред€ми, 
	после чего уход€ в долгий откат, но при первом же выстреле запускаетс€ общий откат навык,
	перезар€жающий всю пачку. */
	if (bAttackButtonPressed)
	{
		StartAttack();
	}
	/* else {
		ƒобавить перезар€дку навыка, если будут добавл€тьс€ 
		навыки стрел€ющие очеред€ми или нанос€щие серии ударов.

		GlobalCooldownSkill();
	}*/
}

void APlayerCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bTrackItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bTrackItems = true;
	}
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlayerCharController)
	{
		RotateCharacter();
		RotateShootPoint();
		CapsuleHalfHeight(DeltaTime);
	}
}


void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*if (Skill)
	{
		Skill->PlayerCharacter = this;
		Skill->PlayerCharController = UGameplayStatics::GetPlayerController(this, 0);
		Skill->SetInitialDamage(15);
	}*/

	if (DefendSkill)
	{
		DefendSkill->PlayerCharacter = this;
		DefendSkill->PlayerCharController = UGameplayStatics::GetPlayerController(this, 0);
		DefendSkill->SetInitialDamage(15);
	}

	if (AttackSkill)
	{
		AttackSkill->PlayerCharacter = this;
		AttackSkill->PlayerCharController = UGameplayStatics::GetPlayerController(this, 0);
	}
}

void APlayerCharacter::RotateCharacter()
{
	FVector2D MousePosition;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetMousePosition(MousePosition);
	}

	FRotator Wec;

	if (MousePosition.X <= 600.f)
	{
		Wec = FRotator{0.f, -90.f, 0.f};
		GetMesh()->SetWorldRotation(Wec);
	}
	else if (MousePosition.X >= 600.f)
	{
		Wec = FRotator{ 0.f, 90.f, 0.f };
		GetMesh()->SetWorldRotation(Wec);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &APlayerCharacter::AttackButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &APlayerCharacter::AttackButtonReleased);

	PlayerInputComponent->BindAxis("CharacterMove", this, &APlayerCharacter::CharacterMove);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("FirstSkill", IE_Pressed, this, &APlayerCharacter::FirstSkillButtonPressed);
	PlayerInputComponent->BindAction("SecondSkill", IE_Pressed, this, &APlayerCharacter::SecondSkillButtonPressed);
}

void APlayerCharacter::TakeItem(AItem* Item)
{

}

void APlayerCharacter::ResetPickupSoundTimer()
{
	bPlayPickupSound = true;
}

void APlayerCharacter::ResetUsingSoundTimer()
{
	bPlayUsingSound = true;
}

void APlayerCharacter::StartPickupSoundTimer()
{
	bPlayPickupSound = false;
	GetWorldTimerManager().SetTimer(
		PickupSoundTimer,
		this,
		&APlayerCharacter::ResetPickupSoundTimer,
		ResetPickupSoundTime);
}

void APlayerCharacter::StartUsingSoundTimer()
{
	bPlayUsingSound = false;
	GetWorldTimerManager().SetTimer(
		PickupSoundTimer,
		this,
		&APlayerCharacter::ResetUsingSoundTimer,
		ResetUsingSoundTime);
}

/*APlayerController* APlayerCharacter::GetPlayerCharacterController()
{
	return UGameplayStatics::GetPlayerController(this, 0);
}*/

AController* APlayerCharacter::GetController()
{
	return APawn::GetController();
}

// TODO: Test Attack Hit Montage.
void APlayerCharacter::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MeleeAttackMontage);
		AnimInstance->Montage_JumpToSection(FName("AttackL"));
	}

	StartCooldownTimer();
}

void APlayerCharacter::DoMeleeDamage(AActor* Target)
{
	if (Target == nullptr) return;

	UGameplayStatics::ApplyDamage(
		Target,
		EquippedWeapon->GetAttackDamage(),
		GetController(),
		this,
		UDamageType::StaticClass());

	/*if (Target->MeleeImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Target->MeleeImpactSound,
			GetActorLocation());
	}*/
}

void APlayerCharacter::ActivateMeleeWeapon()
{
	auto EquippedW = Cast<AMeleeWeapon>(EquippedWeapon);
	EquippedW->GetMeleeWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::DeactivateMeleeWeapon()
{
	auto EquippedW = Cast<AMeleeWeapon>(EquippedWeapon);
	EquippedW->GetMeleeWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::OnMeleeWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto Char = Cast<APlayerCharacter>(OtherActor);
	//AActor* OtherActor;
	if (OtherActor)
	{
		DoMeleeDamage(OtherActor);
		//TODO: SpawnBlood/SpawnSparks
		//TODO: StunCharacter
	}
}

void APlayerCharacter::RotateShootPoint()
{
	FVector MousePosition;
	FVector ShootDirection;

	FVector GetProjShootPoint = ProjectileShootPoint->GetComponentLocation();
	const float a = 0.f;

	if (bCrouching)
	{
		GetProjShootPoint.Z -= 45.f;
	}

	bool MouseToWorldP = PlayerCharController->DeprojectMousePositionToWorld(
		MousePosition,
		ShootDirection);

	/*UE_LOG(LogTemp, Warning, TEXT("X = %f"), MousePosition.X);
	UE_LOG(LogTemp, Warning, TEXT("Y = %f"), MousePosition.Y);
	UE_LOG(LogTemp, Warning, TEXT("Z = %f"), MousePosition.Z);*/

	ShootDirection.Y = a;
	MousePosition.Y = a;

	FVector EndLoc{ GetProjShootPoint + ShootDirection * 1500.f };

	FVector ToObj = EndLoc - GetProjShootPoint;
	FRotator RotateObj = FRotator{ ToObj.Rotation().Pitch, ToObj.Rotation().Yaw, ToObj.Rotation().Roll };

	ProjectileShootPoint->SetWorldRotation(RotateObj);
}

void APlayerCharacter::CrouchButtonPressed()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bCrouching = !bCrouching;
	}
	if (bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
		GetCharacterMovement()->GroundFriction = CrouchingGroundFriction;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	}
}

void APlayerCharacter::CapsuleHalfHeight(float DeltaTime)
{
	float TargetCapsuleHalfHeight{};
	if (bCrouching)
	{
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
	}
	else
	{
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	}

	const float InterpHalfHeight{ FMath::FInterpTo(
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), // ѕолучить значение капсулы.
		TargetCapsuleHalfHeight, // ¬ысота.
		DeltaTime,
		20.f) }; // —корость интерпол€ции.

	const float DeltaCapsuleHalfHeight{ InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() };

	const FVector MeshOffset{ 0.f, 0.f, -DeltaCapsuleHalfHeight };
	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (MagicShield > 0)
	{
		MagicShield -= DamageAmount;
	}
	else 
	{
		Health -= DamageAmount;
	} 
		
	if (Health <= 0.f)
	{
		Health = 0.f;
		Die();
	}

	return DamageAmount;
}

void APlayerCharacter::Die()
{
	bDead = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		APlayerController* PlCntr = UGameplayStatics::GetPlayerController(this, 0);

		if (PlCntr)
		{
			DisableInput(PlCntr);
		}
	}
}

void APlayerCharacter::FinishDeath()
{
	GetMesh()->bPauseAnims = true;
}

void APlayerCharacter::Stun()
{
	if (Health <= 0.f) return;

	CombatState = ECombatState::ECS_Stunned;

	/*
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
	}
	*/
}

void APlayerCharacter::EndStun()
{
	CombatState = ECombatState::ECS_Unoccupied;

	/*
	if (bAimingButtonPressed)
	{
		Aim();
	}
	*/
}

void APlayerCharacter::UseFirstSkillMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(EquippedWeapon->GetFirstSkillMontage());
	}
}

void APlayerCharacter::UseSecondSkillMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(EquippedWeapon->GetSecondSkillMontage());
	}
}
