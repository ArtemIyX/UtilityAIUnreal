// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIStateRuntimeData.h"

FUtilityAIStateRuntimeData::FUtilityAIStateRuntimeData(const TWeakObjectPtr<UUtilityAIProcessor>& InProcessor, const TWeakObjectPtr<UUtilityAIComponent>& InAiComponent)
	: ProcessorPtr(InProcessor)
	, AIComponentPtr(InAiComponent) {}

FUtilityAIStateRuntimeData::FUtilityAIStateRuntimeData(UUtilityAIProcessor* InProcessor, UUtilityAIComponent* InAiComponent)
	: ProcessorPtr(InProcessor)
	, AIComponentPtr(InAiComponent) {}