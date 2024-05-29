// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Jugador.generated.h"

UCLASS()
class DELIVERYEATS_API AJugador : public ACharacter
{
	GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AJugador();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void MoveForward(float Value);

    UFUNCTION(Server, Reliable)
    void ServerMoveForward(float Value);

    UFUNCTION(Server, Reliable)
    void ServerNitroSpeed(float Value);

    UFUNCTION(Server, Reliable)
    void DesactivateNitro(float Value);
    
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MaxSpeed;
    
    UPROPERTY(EditAnywhere, Category = "Movement")
    float AccelerationRate;
    
    UPROPERTY(EditAnywhere, Category = "Movement")
    float DecelerationRate;
    
    void EntregarPaquete();
    
    void GolpeCoche();
    
    void RecogerComida();

    void ActivateNitro(float Value);

    void DeactivateNitro(float Value);

    void RellenarGasoline();

    UFUNCTION(Server, Reliable)
    void ServerRellenarGasoline(AJugador* apPlayer);

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Delibers", Replicated)
    int32 EntregasRestantes;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Delibers", Replicated)
    int32 PaquetesRestantes;
    

    UPROPERTY(EditAnywhere,Replicated)
    bool bIsNitroActive;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Nitro", Replicated)
    float NitroTimeElapsed;
    
    float NitroDuration;
    UPROPERTY(EditAnywhere,Replicated)
    bool QuedaNitro;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
    float CurrentSpeed;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
    float NitroSpeed;
protected:
    virtual void BeginPlay() override;


private:
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};