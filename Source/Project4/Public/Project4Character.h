// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project4Character.generated.h"

UCLASS(config = Game)
class AProject4Character : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AProject4Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/* next 3 are variables handling delta, min/max camera zoom values */
	UPROPERTY(EditAnywhere, Category = Camera)
		float CameraZoomGranularity;

	UPROPERTY(EditAnywhere, Category = Camera)
		float CameraZoomMin;

	UPROPERTY(EditAnywhere, Category = Camera)
		float CameraZoomMax;

	UPROPERTY(EditAnywhere, Category = Camera)
		float CameraSensitivity;

protected:


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	void CameraZoom(float Value);

	// Binds to on left click pressed
	void AddInputToCameraRotation();
	void StartMovePlayerCamera();
	void StopMovePlayerCamera();

	// binds to right click pressed
	void SetPlayerRotationToCamera();
	void StartPlayerRotationToCamera();
	void StopPlayerRotationToCamera();


private:
	// private variables until i get comfortable with UPROPERTY
	class UInputComponent* InputComponent;

	bool doInputRotateCamera;
	bool doRotatePlayerAndCamera;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


public:
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

