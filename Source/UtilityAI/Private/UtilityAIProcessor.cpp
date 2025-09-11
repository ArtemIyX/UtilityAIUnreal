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

UUtilityAIState* UUtilityAIProcessor::GetStateByType(int32 InType) const
{
	for (UUtilityAIState* State : States)
	{
		if (State && State->GetStateType() == InType)
		{
			return State;
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

void UUtilityAIProcessor::InitDefaultStates()
{
	for (TSubclassOf<UUtilityAIState> el : DefaultStates)
	{
		if (el)
		{
			UUtilityAIState* stateObject = NewObject<UUtilityAIState>(el);
			AddState(stateObject);
		}
	}
}
