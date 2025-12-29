// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIStateWeighted.h"

#include "UtilityAIComponent.h"
#include "UtilityAIWeight.h"

UUtilityAIStateWeighted::UUtilityAIStateWeighted(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WeightTemplate = nullptr;
}

float UUtilityAIStateWeighted::GetScore_Implementation(const UUtilityAIContext* Context) const
{
	float result = BaseScore;

	if (!RuntimeData.AIComponentPtr.IsValid())
		return result;

	if (!RuntimeData.ProcessorPtr.IsValid())
		return result;

	for (const FWeightedInitParams& el : Sum)
	{
		UUtilityAIConsideration* consideration = RuntimeData.AIComponentPtr->GetConsiderationById(el.Consideration);
		if (IsValid(consideration))
		{
			float score = consideration->GetScore(
				RuntimeData.AIComponentPtr->GetOwner(),
				RuntimeData.AIComponentPtr.Get(),
				RuntimeData.AIComponentPtr->GetContext());

			if (el.FloatConverter != nullptr)
			{
				score = el.FloatConverter->Evaluate(score);
			}
			result += score;
		}
	}

	return result;
}