// Fill out your copyright notice in the Description page of Project Settings.


#include "Jugador.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AJugador::AJugador()
{
	MaxSpeed = 1000.f;
	AccelerationRate = 500.f;
	DecelerationRate = 1000.f;
	CurrentSpeed = 0.f;

	
	PaquetesRestantes = 10;
	EntregasRestantes = 5;

	NitroSpeed = 1800.f;
	bIsNitroActive = false;

	NitroTimeElapsed = 0;
	NitroDuration = 5;
	QuedaNitro = true;
}

// Called when the game starts or when spawned
void AJugador::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AJugador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FString NitroTimeText = FString::Printf(TEXT("Nitro Time Elapsed: %.2f"), NitroTimeElapsed);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *NitroTimeText);
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * CurrentSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	if (bIsNitroActive == true)
	{
		NitroTimeElapsed += DeltaTime;
		
		if (NitroTimeElapsed >= NitroDuration)
		{
			QuedaNitro = false;
			bIsNitroActive = false;
		}
	}
	
}

// Called to bind functionality to input
void AJugador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AJugador::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AJugador::AddControllerYawInput);

	PlayerInputComponent->BindAction("Nitro", IE_Pressed, this, FInputActionHandlerSignature::TMethodPtr<AJugador>(&AJugador::ActivateNitro));
	PlayerInputComponent->BindAction("Nitro", IE_Released, this, FInputActionHandlerSignature::TMethodPtr<AJugador>(&AJugador::DeactivateNitro));
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AJugador::OnOverlapBegin);
}

void AJugador::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	ACharacter::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJugador, CurrentSpeed);
	DOREPLIFETIME(AJugador, NitroSpeed);
	DOREPLIFETIME(AJugador, PaquetesRestantes);
	DOREPLIFETIME(AJugador, EntregasRestantes);
	DOREPLIFETIME(AJugador, NitroTimeElapsed);
	DOREPLIFETIME(AJugador, bIsNitroActive)
	DOREPLIFETIME(AJugador, QuedaNitro)
}

void AJugador::ActivateNitro(float Value)
{
	if(!IsLocallyControlled()) return;

	ServerNitroSpeed(Value);
	
}

void AJugador::DeactivateNitro(float Value)
{
	if(!IsLocallyControlled()) return;

	DesactivateNitro(Value);
	
}

void AJugador::MoveForward(float Value)
{
	if(!IsLocallyControlled()) return;

	ServerMoveForward(Value);
}

void AJugador::DesactivateNitro_Implementation(float Value)
{
	bIsNitroActive = false;
}

void AJugador::ServerNitroSpeed_Implementation(float Value)
{
	if(QuedaNitro == true)
	{
		bIsNitroActive = true;
	}
	if(QuedaNitro == false)
	{
		bIsNitroActive = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("FuncionNitro"));
}

void AJugador::ServerMoveForward_Implementation(float Value)
{
	if (Value != 0.f)
 	{
 		// Verificar si el nitro está activo
 		if (bIsNitroActive)
 		{
 			CurrentSpeed = FMath::Clamp(CurrentSpeed + (Value * NitroSpeed * GetWorld()->GetDeltaSeconds()), 0.f, NitroSpeed);
 		}
 		else
 		{
 			CurrentSpeed = FMath::Clamp(CurrentSpeed + (Value * AccelerationRate * GetWorld()->GetDeltaSeconds()), 0.f, MaxSpeed);
 		}
 	}
 	else
 	{
 		CurrentSpeed = FMath::Clamp(CurrentSpeed - (DecelerationRate * GetWorld()->GetDeltaSeconds()), 0.f, MaxSpeed);
 	}
}

// Collision function called when player collides with other actors
void AJugador::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Entrega"))
	{
		EntregarPaquete();
		UE_LOG(LogTemp, Warning, TEXT("El jugador ha entregado una pizza"));
	}

	if (OtherActor && OtherActor->ActorHasTag("Coche"))
	{
		UE_LOG(LogTemp, Warning, TEXT("El jugador ha chocado con un enemigoooooooo"));
		GolpeCoche();
	}

	if (OtherActor && OtherActor->ActorHasTag("Pizzeria"))
	{
		UE_LOG(LogTemp, Warning, TEXT("El jugador ha recogido mas comida"));
		RecogerComida();
	}

	if (OtherActor && OtherActor->ActorHasTag("Gasolina"))
	{
		UE_LOG(LogTemp, Warning, TEXT("El jugador llena el depósito"));
		RellenarGasoline();
	}
}


void AJugador::EntregarPaquete()
{
	// Verificar si hay paquetes restantes
	if (PaquetesRestantes > 0)
	{
		PaquetesRestantes--;
		UE_LOG(LogTemp, Warning, TEXT("Paquetes restantes: %d"), PaquetesRestantes);
		

		EntregasRestantes--;
		if (EntregasRestantes == 0)
		{
			// Cambiar a la escena MainMenu,NivelCompletado.
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
		}
	}
	else
	{
		// Si no hay paquetes restantes
		UE_LOG(LogTemp, Warning, TEXT("No hay paquetes restantes para entregar"));
	}
}


void AJugador::GolpeCoche()
{
	PaquetesRestantes -= 2;
	PaquetesRestantes = FMath::Max(PaquetesRestantes, 0);
	UE_LOG(LogTemp, Warning, TEXT("Paquetes restantes: %d"), PaquetesRestantes);
	Jump();
	
}


void AJugador::RecogerComida()
{
	PaquetesRestantes = 10;
	UE_LOG(LogTemp, Warning, TEXT("Paquetes restantes: %d"), PaquetesRestantes);
}

void AJugador::RellenarGasoline()
{
	UE_LOG(LogTemp, Warning, TEXT("El jugador tiene nitro"));
	ServerRellenarGasoline(this);
}

void AJugador::ServerRellenarGasoline_Implementation(AJugador* apPlayer)
{
	apPlayer->NitroTimeElapsed = 0;
	apPlayer->QuedaNitro = true;
	apPlayer->bIsNitroActive = false;
}

