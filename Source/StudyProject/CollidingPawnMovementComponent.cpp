// Fill out your copyright notice in the Description page of Project Settings.

#include "CollidingPawnMovementComponent.h"
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/Pawn.h>
#include "CollidingPawnMovementComponent.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}
