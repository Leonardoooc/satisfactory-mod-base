#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableGenerator.h"
#include "FGPowerInfoComponent.h"
#include "MyFGBuildableGenerator.generated.h"

/**
 * Solar generator that produces power based on time of day
 */
UCLASS()
class DOCMOD_API AMyFGBuildableGenerator : public AFGBuildableGenerator
{
	GENERATED_BODY()

public:
	AMyFGBuildableGenerator();

	// Time settings (in seconds from midnight)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar")
	int mDayTimeStart = 21600; // 6:00 AM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar")
	int mDayTimeEnd = 64800; // 6:00 PM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar")
	int mDayTimeMaxProd = 28800; // 08:00 AM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar")
	int mDayTimeStartDecay = 59400; // 4:30 PM

	// Max power production in MW
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar")
	float mMaxSolarProduction = 50.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	/** Calculate power production based on current time of day */
	float CalculateSolarProduction(int currentTimeSeconds) const;

	/** Update the power info component with current production */
	void UpdatePowerProduction(float production);

	/** Timer for delayed updates */
	float mUpdateTimer = 0.0f;

	/** Update interval in seconds */
	const float mUpdateInterval = 1.0f;
};
