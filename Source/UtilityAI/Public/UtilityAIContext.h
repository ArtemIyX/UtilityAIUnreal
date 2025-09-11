// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIContext.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIContext : public UObject
{
	GENERATED_BODY()
	
public:
	UUtilityAIContext();
};
