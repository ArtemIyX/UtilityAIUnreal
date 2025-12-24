// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAIConvertObjectBase.h"
#include "UtilityAIState.h"
#include "UtilityAIStateWeighted.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct UTILITYAI_API FWeightedInitParams
{
	GENERATED_BODY()

public:
	FWeightedInitParams()
		: FloatConverter(nullptr) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeightName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Consideration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UUtilityAIConvertObjectBase* FloatConverter;

public:

};

UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIStateWeighted : public UUtilityAIState
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseScore{ 0.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeightedInitParams> Sum;

public:
	UUtilityAIStateWeighted(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual float GetScore_Implementation(const UUtilityAIContext* Context) const override;
};