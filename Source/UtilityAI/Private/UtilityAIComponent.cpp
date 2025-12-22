// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIComponent.h"

#include "UtilityAIContext.h"
#include "UtilityAIContextCollector.h"
#include "UtilityAIProcessor.h"
#include "UtilityAIState.h"

UUtilityAIComponent::UUtilityAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ContextClass = UUtilityAIContext::StaticClass();
	ContextCollectorClass = UUtilityAIContextCollector::StaticClass();
	DefaultProcessors = { UUtilityAIProcessor::StaticClass() };
}

void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetNetMode() == NM_Standalone
		|| GetWorld()->GetNetMode() == NM_DedicatedServer
		|| GetOwner()->HasAuthority())
	{
		// Initialize context 
		if (ContextClass)
		{
			Context = NewObject<UUtilityAIContext>(this, ContextClass);
		}

		if (ContextCollectorClass)
		{
			ContextCollector = NewObject<UUtilityAIContextCollector>(this, ContextCollectorClass);
		}

		// Initialize processors
		for (TSubclassOf<UUtilityAIProcessor> el : DefaultProcessors)
		{
			if (el)
			{
				UUtilityAIProcessor* processorObject = NewObject<UUtilityAIProcessor>(this, el);
				AddProcessor(processorObject);
			}
		}

		// Initialize considerations
		for (FConsiderationSetup& el : DefaultConsiderations)
		{
			if (el.ConsiderationClass)
			{
				UUtilityAIConsideration* consObject = NewObject<UUtilityAIConsideration>(this, el.ConsiderationClass);
				AddConsideration(consObject, el.DefaultParamValues);
			}
		}
	}
}

void UUtilityAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GetWorld()->GetNetMode() == NM_Standalone
		|| GetWorld()->GetNetMode() == NM_DedicatedServer
		|| GetOwner()->HasAuthority())
	{
		if (Context)
		{
			Context->ConditionalBeginDestroy();
			Context = nullptr;
		}

		if (ContextCollector)
		{
			ContextCollector->ConditionalBeginDestroy();
			ContextCollector = nullptr;
		}

		const int32 processorsNum = Processors.Num();
		for (int32 i = 0; i < processorsNum; ++i)
		{
			if (Processors[i])
			{
				Processors[i]->ConditionalBeginDestroy();
				Processors[i] = nullptr;
			}
		}
		Processors.Empty();

		const int32 consNum = Considerations.Num();
		for (int32 i = 0; i < consNum; ++i)
		{
			if (Considerations[i])
			{
				Considerations[i]->ConditionalBeginDestroy();
				Considerations[i] = nullptr;
			}
		}
		Considerations.Empty();
		ConsiderationsMap.Empty();
	}
}

void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetWorld()->GetNetMode() == NM_Standalone
		|| GetWorld()->GetNetMode() == NM_DedicatedServer
		|| GetOwner()->HasAuthority())
	{
		UpdateContext();
	}
}

float UUtilityAIComponent::GetConsiderationScoreById(const FGameplayTag& InId) const
{
	if (UUtilityAIConsideration* consideration = GetConsiderationById(InId))
	{
		return consideration->GetScore(GetOwner(), this, GetContext());
	}
	return 0.0f;
}

void UUtilityAIComponent::AddProcessor(UUtilityAIProcessor* InNewProcessor)
{
	if (InNewProcessor && !Processors.Contains(InNewProcessor))
	{
		InNewProcessor->Setup(this);
		Processors.Add(InNewProcessor);
		InNewProcessor->InitDefaultStates();
	}
}

void UUtilityAIComponent::RemoveProcessor(UUtilityAIProcessor* InProcessorToRemove)
{
	Processors.Remove(InProcessorToRemove);
}

UUtilityAIProcessor* UUtilityAIComponent::GetProcessorByType(FGameplayTag InType) const
{
	for (const TObjectPtr<UUtilityAIProcessor>& proc : Processors)
	{
		if (proc && proc->GetProcessorType() == InType)
		{
			return proc.Get();
		}
	}
	return nullptr;
}

bool UUtilityAIComponent::GetBestStateForProcessor(FGameplayTag InProcessorType, UUtilityAIState*& OutState, float& OutScore) const
{
	OutState = nullptr;
	OutScore = 0.0f;

	UUtilityAIProcessor* processor = GetProcessorByType(InProcessorType);
	if (processor == nullptr)
		return false;

	if (Context == nullptr)
		return false;

	float bestScore = 0.0f;
	UUtilityAIState* returnState = processor->GetBestState(Context, bestScore);
	OutState = returnState;
	OutScore = bestScore;
	return IsValid(returnState);
}

/*
void UUtilityAIComponent::ExecuteBestForProcessorType(int32 Type)
{
	UUtilityAIProcessor* processor = GetProcessorByType(Type);
	if (processor && Context)
	{
		float bestScore;
		UUtilityAIState* bestState = processor->GetBestState(Context, bestScore);
		if (bestState && bestScore > 0.0f)
		{
			bestState->Execute(GetOwner(), this, Context);
		}
	}
}
*/

UUtilityAIConsideration* UUtilityAIComponent::GetConsiderationById(const FGameplayTag& InId) const
{
	if (ConsiderationsMap.Contains(InId))
	{
		return ConsiderationsMap[InId].Get();
	}
	return nullptr;
	/*if (GetOwner()->HasAuthority())
	{
		
	}
	else
	{
		for (const TObjectPtr<UUtilityAIConsideration>& consideration : Considerations)
		{
			if (consideration && consideration->GetId() == InId)
			{
				return consideration.Get();
			}
		}
		return nullptr;
	}*/
}

void UUtilityAIComponent::AddConsideration(UUtilityAIConsideration* InNewConsideration,
	TMap<FConsiderationParamKey, FConsiderationParamValue>& InParams)
{
	if (InNewConsideration && !Considerations.Contains(InNewConsideration))
	{
		// Add to storage
		Considerations.Add(InNewConsideration);
		ConsiderationsMap.Add(InNewConsideration->GetId(), InNewConsideration);

		// Add params
		InNewConsideration->InitDefaultParams();

		// Set custom values
		for (TPair<FConsiderationParamKey, FConsiderationParamValue> pair : InParams)
		{
			InNewConsideration->SetParam(pair.Key.Key, pair.Value);
		}
	}
}

void UUtilityAIComponent::RemoveConsideration(UUtilityAIConsideration* InConsiderationToRemove)
{
	if (InConsiderationToRemove)
	{
		const FGameplayTag id = InConsiderationToRemove->GetId();
		if (ConsiderationsMap.Contains(id))
		{
			ConsiderationsMap.Remove(id);
		}
	}
	Considerations.Remove(InConsiderationToRemove);
}

void UUtilityAIComponent::RemoveConsiderationById(const FGameplayTag& InId)
{
	if (UUtilityAIConsideration* consideration = GetConsiderationById(InId))
	{
		RemoveConsideration(consideration);
	}
}

void UUtilityAIComponent::UpdateContext()
{
	if (!GetOwner() || !Context || !ContextCollector)
	{
		return;
	}
	ContextCollector->FillContext(this, Context);
}