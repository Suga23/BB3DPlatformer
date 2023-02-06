// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

// Sets default values
AItem::AItem() :
	ItemName(FString("Default")),
	ItemCount(0),
	ItemState(EItemState::EIS_Pickup),
	ItemType(EItemType::EIT_MAX)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Visibility,
		ECollisionResponse::ECR_Block);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	SetItemProperties(ItemState);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->IncrementOverlappedItemCount(-1);
		}
	}
}

void AItem::SetItemProperties(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_Pickup:
		ItemMesh->SetSimulatePhysics(false); // ��������� ������, ��� Item Mesh.
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);	// ��������� �������.
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ���������� ��������.
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ��������� ������� AreaSphere.
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// ��������� ������� CollisionBox.
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel( // ���������� ������� �� ��������� �� ����������, ����� ��� ����� ����������� ����������� �����.
			ECollisionChannel::ECC_Visibility,
			ECollisionResponse::ECR_Block); // ������ �������� - ������������(������������� ��� ������� � ��������� ���������).

		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ����� �������� ��������, ��� ���������� ������� � ������, ����� ����������� ��������, ������� ����� ������������.
		break;

	case EItemState::EIS_Equipped:
		ItemMesh->SetSimulatePhysics(false); // ��������� ������, ��� Item Mesh.
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);	// ��������� �������.
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ���������� ��������.
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ��������� ������� AreaSphere.
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ��������� ������� CollisionBox.
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ����� �������� ��������, ��� ���������� ������� � ������, ����� ����������� ��������, ������� ����� ������������.
		break;

	case EItemState::EIS_Falling:
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_WorldStatic,
			ECollisionResponse::ECR_Block);
		// AreaSphere properties
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Set CollisionBox properties
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_PickedUp:
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AItem::PlayPickupSound(bool bForcePlaySound)
{
	if (Character)
	{
		if (bForcePlaySound)
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
		}
		else if (Character->ShouldPlayEquipSound())
		{
			Character->StartPickupSoundTimer();
			if (PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
		}
	}
}

void AItem::PlayEquipSound(bool bForcePlaySound)
{
	if (Character)
	{
		if (bForcePlaySound)
		{
			if (UsingSound)
			{
				UGameplayStatics::PlaySound2D(this, UsingSound);
			}
		}
		else if (Character->ShouldPlayEquipSound())
		{
			Character->StartUsingSoundTimer();
			if (UsingSound)
			{
				UGameplayStatics::PlaySound2D(this, UsingSound);
			}
		}
	}
}

void AItem::FinishPickupItem(APlayerCharacter* Char, bool bForcePlaySound)
{
	Character = Char;

	if (Character)
	{
		Character->TakeItem(this);

		PlayPickupSound(bForcePlaySound);
	}
}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}