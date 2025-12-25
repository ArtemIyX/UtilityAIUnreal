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
		: FloatConverter(nullptr)
		, FloatConverterClass(UUtilityAIConvertObjectBase::StaticClass()) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeightName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Consideration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUtilityAIConvertObjectBase> FloatConverterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UUtilityAIConvertObjectBase* FloatConverter;

public:

};

UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIStateWeighted : public UUtilityAIState
{
	GENERATED_BODY()

public:
	UUtilityAIStateWeighted(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateWeighted")
	float BaseScore{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateWeighted")
	TArray<FWeightedInitParams> Sum;

public:
	virtual float GetScore_Implementation(const UUtilityAIContext* Context) const override;
};