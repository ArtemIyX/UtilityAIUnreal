// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIProcessor.h"
#include "UtilityAIState.h"


UUtilityAIProcessor::UUtilityAIProcessor(): ProcessorType(0)
{
}


void UUtilityAIProcessor::GetCurrentStates(TArray<UUtilityAIState*>& OutStates) const
{
	OutStates.Empty();
	OutStates.Reserve(States.Num());
	for (TObjectPtr<UUtilityAIState> el : States)
	{
		if (el)
		{
			OutStates.Add(el.Get());
		}
	}
}

void UUtilityAIProcessor::AddState(UUtilityAIState* InNewState)
{
	if (InNewState && !States.Contains(InNewState))
	{
		States.Add(InNewState);
	}
}

void UUtilityAIProcessor::RemoveState(UUtilityAIState* InStateToRemove)
{
	States.Remove(InStateToRemove);
}

UUtilityAIState* UUtilityAIProcessor::GetBestState(const UUtilityAIContext* InContext, float& OutScore) const
{
	return FindHighestScoringState(InContext, OutScore);
}

void UUtilityAIProcessor::GetScoreMap(const UUtilityAIContext* InContext, TMap<UUtilityAIState*, float>& OutScoreMap,
                                               TMap<UUtilityAIState*, float>& OutScoreMapNormalized)
{
	OutScoreMap.Empty();
	OutScoreMapNormalized.Empty();

	if (!InContext)
		return;

	// 1. Collect raw scores
	for (const TObjectPtr<UUtilityAIState>& el : States)
	{
		if (!el)
			continue;

		const float rawScore = el->GetScore(InContext);
		OutScoreMap.Add(el.Get(), rawScore);
		OutScoreMapNormalized.Add(el.Get(), rawScore);   // temporary copy
	}

	// 2. Early-out if nothing to normalise
	if (OutScoreMapNormalized.Num() == 0)
		return;

	// 3. Find max raw score
	float maxScore = 0.f;
	for (const TPair<UUtilityAIState*, float>& p : OutScoreMapNormalized)
	{
		maxScore = FMath::Max(maxScore, p.Value);
	}

	// 4. Normalise in-place (only if non-zero)
	if (maxScore > SMALL_NUMBER)
	{
		for (TPair<UUtilityAIState*, float>& p : OutScoreMapNormalized)
		{
			p.Value /= maxScore;
		}
	}
}

UUtilityAIState* UUtilityAIProcessor::GetStateByType(int32 InType) const
{
	for (const TObjectPtr<UUtilityAIState>& el : States)
	{
		if (el && el->GetStateType() == InType)
		{
			return el.Get();
		}
	}
	return nullptr;
}

UUtilityAIState* UUtilityAIProcessor::FindHighestScoringState(const UUtilityAIContext* Context, float& OutScore) const
{
	OutScore = -1.0f;
	UUtilityAIState* BestState = nullptr;

	for (UUtilityAIState* State : States)
	{
		if (State)
		{
			float Score = State->GetScore(Context);
			if (Score > OutScore)
			{
				OutScore = Score;
				BestState = State;
			}
		}
	}

	return BestState;
}

void UUtilityAIProcessor::BeginDestroy()
{
	const int32 n = States.Num();
	if (n > 0)
	{
		for (int32 i = 0; i < n; ++i)
		{
			if (States[i])
			{
				States[i]->ConditionalBeginDestroy();
				States[i] = nullptr;
			}
		}
	}
	States.Empty();
	UObject::BeginDestroy();
}

void UUtilityAIProcessor::InitDefaultStates()
{
	for (TSubclassOf<UUtilityAIState> el : DefaultStates)
	{
		if (el)
		{
			UUtilityAIState* stateObject = NewObject<UUtilityAIState>(this, el);
			AddState(stateObject);
		}
	}
}
