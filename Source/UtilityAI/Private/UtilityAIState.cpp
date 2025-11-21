// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIState.h"

UUtilityAIState::UUtilityAIState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StateType(0) {}

void UUtilityAIState::Setup(const FUtilityAIStateRuntimeData& InParams)
{
	this->RuntimeData = InParams;
}


float UUtilityAIState::GetScore_Implementation(const UUtilityAIContext* Context) const
{
	return 0.0f;
}