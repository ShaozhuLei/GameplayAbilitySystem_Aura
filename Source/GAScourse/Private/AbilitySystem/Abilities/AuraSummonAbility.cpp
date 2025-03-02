// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocation()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread/NumMinions;
	
	const FVector LeftSpread = Forward.RotateAngleAxis(-SpawnSpread/2, FVector::UpVector);
	TArray<FVector> SpawnLocations;

	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenLocation + FVector(0.f, 0.f, 400.f), ChosenLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);

		if (Hit.bBlockingHit)
		{
			ChosenLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenLocation);
	}
	return SpawnLocations;
}
