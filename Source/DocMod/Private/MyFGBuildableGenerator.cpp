#include "MyFGBuildableGenerator.h"
#include "FGTimeSubsystem.h"

AMyFGBuildableGenerator::AMyFGBuildableGenerator()
{
	// Enable Tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	// Set the base power production - this will be overridden dynamically
	mPowerProduction = 50.0f;
}

void AMyFGBuildableGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void AMyFGBuildableGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only update every 3 seconds
	mUpdateTimer += DeltaTime;
	if (mUpdateTimer < mUpdateInterval)
	{
		return;
	}
	mUpdateTimer = 0.0f;

	AFGTimeOfDaySubsystem* TimeSubsystem = AFGTimeOfDaySubsystem::Get(GetWorld());
	if (!TimeSubsystem)
	{
		return;
	}

	// Get current time in seconds (0-86400)
	int32 CurrentTimeSeconds = TimeSubsystem->GetDaySeconds();
	float Production = CalculateSolarProduction(CurrentTimeSeconds);
	UpdatePowerProduction(Production);
}

float AMyFGBuildableGenerator::CalculateSolarProduction(int currentTimeSeconds) const
{
	// Night time - no production
	if (currentTimeSeconds < mDayTimeStart || currentTimeSeconds > mDayTimeEnd)
	{
		return 0.0f;
	}

	float Production = 0.0f;

	// Morning ramp up: from DayTimeStart to DayTimeMaxProd
	if (currentTimeSeconds >= mDayTimeStart && currentTimeSeconds <= mDayTimeMaxProd)
	{
		float Progress = static_cast<float>(currentTimeSeconds - mDayTimeStart) / 
		                 static_cast<float>(mDayTimeMaxProd - mDayTimeStart);
		Production = mMaxSolarProduction * Progress;
	}
	// Peak production: from DayTimeMaxProd to DayTimeStartDecay
	else if (currentTimeSeconds > mDayTimeMaxProd && currentTimeSeconds < mDayTimeStartDecay)
	{
		Production = mMaxSolarProduction;
	}
	// Evening decay: from DayTimeStartDecay to DayTimeEnd
	else if (currentTimeSeconds >= mDayTimeStartDecay && currentTimeSeconds <= mDayTimeEnd)
	{
		float Progress = static_cast<float>(mDayTimeEnd - currentTimeSeconds) / 
		                 static_cast<float>(mDayTimeEnd - mDayTimeStartDecay);
		Production = mMaxSolarProduction * Progress;
	}

	return FMath::Clamp(Production, 0.0f, mMaxSolarProduction);
}

void AMyFGBuildableGenerator::UpdatePowerProduction(float production)
{
	UFGPowerInfoComponent* PowerInfo = GetPowerInfo();
	if (PowerInfo)
	{
		PowerInfo->SetDynamicProductionCapacity(production);
		PowerInfo->SetBaseProduction(production);
	}
}
