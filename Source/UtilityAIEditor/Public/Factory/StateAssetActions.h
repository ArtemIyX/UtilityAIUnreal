// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "UtilityAIState.h"


class UTILITYAIEDITOR_API FStateAssetActions : public FAssetTypeActions_Base
{

public:
	virtual UClass* GetSupportedClass() const override { return UUtilityAIState::StaticClass(); };
	virtual FText GetName() const override { return INVTEXT("State"); };
	virtual FColor GetTypeColor() const override { return FColor::Cyan; };

	virtual uint32 GetCategories() override
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			return FAssetToolsModule::GetModule().Get().FindAdvancedAssetCategory(FName("UtilityAI"));
		}

		return EAssetTypeCategories::Misc;
	};
};