// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIConsideration.h"

UUtilityAIConsideration::UUtilityAIConsideration(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}


bool UUtilityAIConsideration::AddParam(const FConsiderationParamKey& InKey, const FConsiderationParamValue& InValue)
{
	if (RuntimeParams.Contains(InKey))
	{
		return false;
	}
	RuntimeParams.Add(InKey, InValue);
	return true;
}

void UUtilityAIConsideration::InitDefaultParams()
{
	for (TPair<FConsiderationParamKey, FConsiderationParamValue>& pair : DefaultParams)
	{
		AddParam(pair.Key, pair.Value);
	}
}

bool UUtilityAIConsideration::SetParam(FGameplayTag InKey, const FConsiderationParamValue& InValue)
{
	const FConsiderationParamKey paramKey(InKey);
	if (!RuntimeParams.Contains(paramKey))
	{
		return AddParam(paramKey, InValue);
	}
	RuntimeParams[paramKey] = InValue;
	return true;
}

bool UUtilityAIConsideration::TryGetParamValue(FGameplayTag InKey, FConsiderationParamValue& OutValue)
{
	const FConsiderationParamKey paramKey(InKey);
	if (!RuntimeParams.Contains(paramKey))
	{
		OutValue = FConsiderationParamValue();
		return false;
	}
	OutValue = RuntimeParams[paramKey];
	return true;
}

bool UUtilityAIConsideration::ContainsParam(FGameplayTag InKey) const
{
	return RuntimeParams.Contains(FConsiderationParamKey(InKey));
}

float UUtilityAIConsideration::GetScore_Implementation(AActor* Owner, const UUtilityAIComponent* InSystem, const UUtilityAIContext* InContext) const
{
	return 0.0f;
}