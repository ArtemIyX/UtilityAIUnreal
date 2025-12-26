// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIWeight.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIWeight : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Utility AI Weight")
	TMap<FString, float> WeightMap;

};
