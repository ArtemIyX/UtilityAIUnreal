#pragma once

#include "CoreMinimal.h"
#include "UtilityAIAssetFactory.h"

#include "AssetTypeActions_UtilityAI.h"
#include "UtilityAIConsideration.h"
#include "UtilityAIContext.h"
#include "UtilityAIContextCollector.h"
#include "UtilityAIProcessor.h"
#include "UtilityAIState.h"
#include "UtilityAIConvertObjectBase.h"
#include "UtilityAIFactories.generated.h"

namespace UtilityAI
{
	namespace Classes
	{
		inline TSubclassOf<UObject> Context = UUtilityAIContext::StaticClass();
		inline TSubclassOf<UObject> ContextCollector = UUtilityAIContextCollector::StaticClass();
		inline TSubclassOf<UObject> Processor = UUtilityAIProcessor::StaticClass();
		inline TSubclassOf<UObject> State = UUtilityAIState::StaticClass();
		inline TSubclassOf<UObject> Consideration = UUtilityAIConsideration::StaticClass();
		inline TSubclassOf<UObject> Converter = UUtilityAIConvertObjectBase::StaticClass();
	}

}

// Context ---------------------------------------------------------------------

UCLASS()
class UTILITYAIEDITOR_API UContextFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UContextFactory(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SupportedClass = UtilityAI::Classes::Context;
		TargetObjectClass = UtilityAI::Classes::Context;
	}
};

class FAssetTypeActions_Context : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_Context()
	{
		SupportedClass = UtilityAI::Classes::Context;
		Name = FText::FromString("Context");
	}
};

// Context Collector ---------------------------------------------------------------------

UCLASS()
class UTILITYAIEDITOR_API UContextCollectorFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UContextCollectorFactory(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SupportedClass = UtilityAI::Classes::ContextCollector;
		TargetObjectClass = UtilityAI::Classes::ContextCollector;
	}
};

class FAssetTypeActions_ContextCollector : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_ContextCollector()
	{
		SupportedClass = UtilityAI::Classes::ContextCollector;
		Name = FText::FromString("Context Collector");
	}
};

// Processor ---------------------------------------------------------------------

UCLASS()
class UTILITYAIEDITOR_API UProcessorFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UProcessorFactory(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SupportedClass = UtilityAI::Classes::Processor;
		TargetObjectClass = UtilityAI::Classes::Processor;
	}
};

class FAssetTypeActions_Processor : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_Processor()
	{
		SupportedClass = UtilityAI::Classes::Processor;
		Name = FText::FromString("Processor");
	}
};

// State ---------------------------------------------------------------------

UCLASS()
class UTILITYAIEDITOR_API UProcessorStateFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UProcessorStateFactory(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SupportedClass = UtilityAI::Classes::State;
		TargetObjectClass = UtilityAI::Classes::State;
	}
};

class FAssetTypeActions_ProcessorState : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_ProcessorState()
	{
		SupportedClass = UtilityAI::Classes::State;
		Name = FText::FromString("Processor State");
	}
};

// Consideration ---------------------------------------------------------------------

UCLASS()
class UTILITYAIEDITOR_API UProcessorConsiderationFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UProcessorConsiderationFactory(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SupportedClass = UtilityAI::Classes::Consideration;
		TargetObjectClass = UtilityAI::Classes::Consideration;
	}
};

class FAssetTypeActions_ProcessorConsideration : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_ProcessorConsideration()
	{
		SupportedClass = UtilityAI::Classes::Consideration;
		Name = FText::FromString("Processor Consideration");
	}
};

// Converter ---------------------------------------------------------------------

UCLASS()
class UTILITYAIEDITOR_API UFloatConverterFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UFloatConverterFactory(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		SupportedClass = UtilityAI::Classes::Converter;
		TargetObjectClass = UtilityAI::Classes::Converter;
	}
};

class FAssetTypeActions_Converter : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_Converter()
	{
		SupportedClass = UtilityAI::Classes::Converter;
		Name = FText::FromString("Float Converter");
	}
};