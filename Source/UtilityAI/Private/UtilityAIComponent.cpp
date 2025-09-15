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
	DefaultProcessors = {UUtilityAIProcessor::StaticClass()};
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

		int32 processorsNum = Processors.Num();
		for (int32 i = 0; i < processorsNum; ++i)
		{
			if (Processors[i])
			{
				Processors[i]->ConditionalBeginDestroy();
				Processors[i] = nullptr;
			}
		}
		Processors.Empty();
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

void UUtilityAIComponent::AddProcessor(UUtilityAIProcessor* NewProcessor)
{
	if (NewProcessor && !Processors.Contains(NewProcessor))
	{
		Processors.Add(NewProcessor);
		NewProcessor->InitDefaultStates();
	}
}

void UUtilityAIComponent::RemoveProcessor(UUtilityAIProcessor* ProcessorToRemove)
{
	Processors.Remove(ProcessorToRemove);
}



UUtilityAIProcessor* UUtilityAIComponent::GetProcessorByType(int32 Type) const
{
	for (UUtilityAIProcessor* proc : Processors)
	{
		if (proc && proc->GetProcessorType() == Type)
		{
			return proc;
		}
	}
	return nullptr;
}

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

void UUtilityAIComponent::UpdateContext()
{
	if (!GetOwner() || !Context || !ContextCollector)
	{
		return;
	}
	ContextCollector->FillContext(this, Context);
}
