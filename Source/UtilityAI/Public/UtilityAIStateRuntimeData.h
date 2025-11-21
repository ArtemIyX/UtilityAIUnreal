// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"

#include "UtilityAIStateRuntimeData.generated.h"

class UUtilityAIComponent;
class UUtilityAIProcessor;

USTRUCT(Blueprintable, BlueprintType)
struct UTILITYAI_API FUtilityAIStateRuntimeData
{
	GENERATED_BODY()

public:
	FUtilityAIStateRuntimeData() {}

	explicit FUtilityAIStateRuntimeData(const TWeakObjectPtr<UUtilityAIProcessor>& InProcessor,
		const TWeakObjectPtr<UUtilityAIComponent>& InAiComponent);

	explicit FUtilityAIStateRuntimeData(UUtilityAIProcessor* InProcessor,
		UUtilityAIComponent* InAiComponent);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UUtilityAIProcessor> ProcessorPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UUtilityAIComponent> AIComponentPtr;
};
