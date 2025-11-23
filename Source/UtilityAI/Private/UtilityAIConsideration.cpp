// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIConsideration.h"

UUtilityAIConsideration::UUtilityAIConsideration(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

float UUtilityAIConsideration::GetScore_Implementation(AActor* Owner, const UUtilityAIComponent* InSystem, const UUtilityAIContext* InContext) const
{
	return 0.0f;
}