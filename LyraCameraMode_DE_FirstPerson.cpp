// Copyright ScaleSculptor / Spyrm.


#include "LyraCameraMode_DE_FirstPerson.h"
#include "Curves/CurveVector.h"
#include "Engine/Canvas.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CameraBlockingVolume.h"
#include "Camera/LyraCameraAssistInterface.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"

namespace LyraCameraMode_DE_FirstPerson_Statics
{
	static const FName NAME_IgnoreCameraCollision = TEXT("IgnoreCameraCollision");
}

ULyraCameraMode_DE_FirstPerson::ULyraCameraMode_DE_FirstPerson()
{
	TargetOffsetCurve = nullptr;
}

void ULyraCameraMode_DE_FirstPerson::UpdateView(float DeltaTime)
{
	UpdateForTarget(DeltaTime);
	UpdateCrouchOffset(DeltaTime);

	FVector PivotLocation = GetPivotLocation() + CurrentCrouchOffset;
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void ULyraCameraMode_DE_FirstPerson::UpdateForTarget(float DeltaTime)
{

	if (const ACharacter* TargetCharacter = Cast<ACharacter>(GetTargetActor()))
	{
		if (TargetCharacter->bIsCrouched)
		{
			const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			const float CrouchedHeightAdjustment = TargetCharacterCDO->CrouchedEyeHeight - TargetCharacterCDO->BaseEyeHeight;

			SetTargetCrouchOffset(FVector(0.f, 0.f, CrouchedHeightAdjustment));

			return;
		}
	}

	SetTargetCrouchOffset(FVector::ZeroVector);
}

void ULyraCameraMode_DE_FirstPerson::DrawDebug(UCanvas* Canvas) const
{
	Super::DrawDebug(Canvas);

#if ENABLE_DRAW_DEBUG
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
	for (int i = 0; i < DebugActorsHitDuringCameraPenetration.Num(); i++)
	{
		DisplayDebugManager.DrawString(
			FString::Printf(TEXT("HitActorDuringPenetration[%d]: %s")
				, i
				, *DebugActorsHitDuringCameraPenetration[i]->GetName()));
	}

	LastDrawDebugTime = GetWorld()->GetTimeSeconds();
#endif
}

void ULyraCameraMode_DE_FirstPerson::UpdatePreventPenetration(float DeltaTime)
{
}

void ULyraCameraMode_DE_FirstPerson::PreventCameraPenetration(class AActor const& ViewTarget, FVector const& SafeLoc, FVector& CameraLoc, float const& DeltaTime, float& DistBlockedPct, bool bSingleRayOnly)
{
}

void ULyraCameraMode_DE_FirstPerson::SetTargetCrouchOffset(FVector NewTargetOffset)
{
}


void ULyraCameraMode_DE_FirstPerson::UpdateCrouchOffset(float DeltaTime)
{
}

FVector ULyraCameraMode_DE_FirstPerson::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// Get mesh socket location for head bone
		if (const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn))
		{
			return TargetCharacter->GetMesh()->GetSocketLocation(CameraAttachSocket);
		}

		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();

}
