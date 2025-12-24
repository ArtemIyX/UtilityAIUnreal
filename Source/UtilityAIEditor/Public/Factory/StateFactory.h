// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_UtilityAI.h"
#include "UtilityAIAssetFactory.h"
#include "StateFactory.generated.h"

UCLASS()
class UTILITYAIEDITOR_API UStateFactory : public UUtilityAIAssetFactory
{
	GENERATED_BODY()

public:
	UStateFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		SupportedClass = UUtilityAIState::StaticClass();
		TargetObjectClass = UUtilityAIState::StaticClass();
	}
};

class UTILITYAIEDITOR_API FAssetTypeActions_State : public FAssetTypeActions_UtilityAI
{
public:
	FAssetTypeActions_State()
	{
		SupportedClass = UUtilityAIState::StaticClass();
		Name = FText::FromString(TEXT("Processor State"));
	}
};