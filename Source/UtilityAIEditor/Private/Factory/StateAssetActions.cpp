// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Factory/AssetTypeActions_UtilityAI.h"

FAssetTypeActions_UtilityAI::FAssetTypeActions_UtilityAI()
{
	SupportedClass = UObject::StaticClass();
	Name = FText::FromString("Utility AI Object");
}

uint32 FAssetTypeActions_UtilityAI::GetCategories()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		return FAssetToolsModule::GetModule().Get().FindAdvancedAssetCategory(FName("UtilityAI"));
	}

	return EAssetTypeCategories::Misc;
}