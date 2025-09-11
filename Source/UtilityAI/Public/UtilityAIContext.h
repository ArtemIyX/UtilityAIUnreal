// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIContext.generated.h"

/**
 * @class UUtilityAIContext
 * @brief Base context class that holds data used by states for scoring and execution.
 * @note Inherit from this to create custom context data for your AI.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIContext : public UObject
{
	GENERATED_BODY()
	
public:
	UUtilityAIContext();
};
