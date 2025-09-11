// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIContextCollector.generated.h"

class UUtilityAIContext;
class UUtilityAIComponent;
/**
 * @class UUtilityAIContextCollector
 * @brief Responsible for gathering and populating context data before state evaluation.
 * @note Implement FillContext() to provide relevant data to your AI states.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIContextCollector : public UObject
{
	GENERATED_BODY()

public:
	/**
	* @brief Fills the AI context with relevant data
	* @param InSystem The Utility AI component requesting context data
	* @param InContext The context object to populate
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AI Context Collector")
	void FillContext(UUtilityAIComponent* InSystem, UUtilityAIContext* InContext);
};
