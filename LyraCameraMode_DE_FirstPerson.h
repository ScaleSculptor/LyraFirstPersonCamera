// Copyright ScaleSculptor / Spyrm.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCameraMode.h"
#include "Curves/CurveFloat.h"
#include "Camera/LyraPenetrationAvoidanceFeeler.h"
#include "DrawDebugHelpers.h"
#include "LyraCameraMode_DE_FirstPerson.generated.h"

class UCurveVector;

/**
 * ALyraCameraMode_DE_FirstPerson
 * 
 * Camera designed to be attached to a head/eyes socket.
 */
UCLASS(Abstract, Blueprintable)
class DRAGONSEGGRUNTIME_API ULyraCameraMode_DE_FirstPerson : public ULyraCameraMode
{
	GENERATED_BODY()
	
public:

	ULyraCameraMode_DE_FirstPerson();

protected:

	virtual FVector GetPivotLocation() const override;

	virtual void UpdateView(float DeltaTime) override;

	void UpdateForTarget(float DeltaTime);
	void UpdatePreventPenetration(float DeltaTime);
	void PreventCameraPenetration(class AActor const& ViewTarget, FVector const& SafeLoc, FVector& CameraLoc, float const& DeltaTime, float& DistBlockedPct, bool bSingleRayOnly);

	virtual void DrawDebug(UCanvas* Canvas) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "First Person")
		FName CameraAttachSocket = "Socket.Camera";

	UPROPERTY(EditDefaultsOnly, Category = "First Person", Meta = (EditCondition = "!bUseRuntimeFloatCurves"))
		const UCurveVector* TargetOffsetCurve;

	UPROPERTY(EditDefaultsOnly, Category = "First Person")
		bool bUseRuntimeFloatCurves;

	UPROPERTY(EditDefaultsOnly, Category = "First Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
		FRuntimeFloatCurve TargetOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "First Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
		FRuntimeFloatCurve TargetOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "First Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
		FRuntimeFloatCurve TargetOffsetZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "First Person")
		float CrouchOffsetBlendMultiplier = 5.0f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		float PenetrationBlendInTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		float PenetrationBlendOutTime = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		bool bPreventPenetration = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		bool bDoPredictiveAvoidance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		float CollisionPushOutDistance = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		float ReportPenetrationPercent = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		TArray<FLyraPenetrationAvoidanceFeeler> PenetrationAvoidanceFeelers;

	UPROPERTY(Transient)
		float AimLineToDesiredPosBlockedPct;

	UPROPERTY(Transient)
		TArray<TObjectPtr<const AActor>> DebugActorsHitDuringCameraPenetration;

#if ENABLE_DRAW_DEBUG
	mutable float LastDrawDebugTime = -MAX_FLT;
#endif

protected:

	void SetTargetCrouchOffset(FVector NewTargetOffset);
	void UpdateCrouchOffset(float DeltaTime);

	FVector InitialCrouchOffset = FVector::ZeroVector;
	FVector TargetCrouchOffset = FVector::ZeroVector;
	float CrouchOffsetBlendPct = 1.0f;
	FVector CurrentCrouchOffset = FVector::ZeroVector;

};