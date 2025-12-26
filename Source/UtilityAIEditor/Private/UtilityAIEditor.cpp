#include "UtilityAIEditor.h"

#include "AssetToolsModule.h"
#include "UtilityAIConvertObjectBase.h"
#include "Factory/UtilityAIFactories.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Thumbnail/ConvObjThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FUtilityAIEditorModule"


TSharedPtr<FSlateStyleSet> FUtilityAIEditorModule::StyleSet;

void FUtilityAIEditorModule::StartupModule()
{
	if (GEditor)
	{
		if (!StyleSet.IsValid())
		{
			StyleSet = Create();
			FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
		}

		UThumbnailManager::Get().RegisterCustomRenderer(UUtilityAIConvertObjectBase::StaticClass(),
			UConvObjThumbnailRenderer::StaticClass());

		IAssetTools& assetTools = IAssetTools::Get();
		assetTools.RegisterAdvancedAssetCategory(FName(TEXT("UtilityAI")), FText::FromString(TEXT("Utility AI")));
		RegisterAssetActions<FAssetTypeActions_Context>(assetTools);
		RegisterAssetActions<FAssetTypeActions_ContextCollector>(assetTools);
		RegisterAssetActions<FAssetTypeActions_Processor>(assetTools);
		RegisterAssetActions<FAssetTypeActions_ProcessorState>(assetTools);
		RegisterAssetActions<FAssetTypeActions_ProcessorStateWeighted>(assetTools);
		RegisterAssetActions<FAssetTypeActions_ProcessorConsideration>(assetTools);
		RegisterAssetActions<FAssetTypeActions_Converter>(assetTools);
		RegisterAssetActions<FAssetTypeActions_WeightObject>(assetTools);
	}
}

void FUtilityAIEditorModule::ShutdownModule()
{
	if (GEditor && FSlateApplication::IsInitialized())
	{
		if (StyleSet.IsValid())
		{
			FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
			StyleSet.Reset();
		}
		UThumbnailManager::Get().UnregisterCustomRenderer(UUtilityAIConvertObjectBase::StaticClass());
		
	}

}

void FUtilityAIEditorModule::UnregisterAssetActions()
{
	IAssetTools& assetTools = IAssetTools::Get();
	const int32 n = AssetActions.Num();
	for (int32 i = 0; i < n; ++i)
	{
		assetTools.UnregisterAssetTypeActions(AssetActions[i].ToSharedRef());
	}
	AssetActions.Empty();
}

TSharedRef<FSlateStyleSet> FUtilityAIEditorModule::Create()
{
	TSharedRef<FSlateStyleSet> style = MakeShareable(new FSlateStyleSet("UtilityAIStyle"));

	// Get plugin base directory
	FString contentDir = IPluginManager::Get().FindPlugin(TEXT("UtilityAI"))->GetBaseDir() / TEXT("Resources");

	style->SetContentRoot(contentDir);

	// Define the thumbnail brush (128x128 size)
	const FVector2D icon16(16.0f, 16.0f);

	const FVector2D icon32(32.0f, 32.0f);

	// Smaller icon for lists/context menus (16x16)
	const FVector2D icon128(128.0f, 128.0f);

	style->Set("ClassThumbnail.UtilityAIConsideration",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIConsideration128.png")), icon128));

	style->Set("ClassIcon.UtilityAIConsideration",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIConsideration16.png")), icon16));

	style->Set("ClassThumbnail.UtilityAIState",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIState128.png")), icon128));

	style->Set("ClassIcon.UtilityAIState",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIState16.png")), icon16));

	style->Set("ClassThumbnail.UtilityAIProcessor",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIProcessor128.png")), icon128));

	style->Set("ClassIcon.UtilityAIProcessor",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIProcessor16.png")), icon16));

	style->Set("ClassThumbnail.UtilityAIComponent",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIComponent128.png")), icon128));

	style->Set("ClassIcon.UtilityAIComponent",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIComponent32.png")), icon32));

	style->Set("ClassThumbnail.UtilityAIContextCollector",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIContextCollector128.png")), icon128));

	style->Set("ClassIcon.UtilityAIContextCollector",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIContextCollector16.png")), icon16));

	style->Set("ClassThumbnail.UtilityAIContext",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIContext128.png")), icon128));

	style->Set("ClassIcon.UtilityAIContext",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIContext16.png")), icon16));

	style->Set("ClassIcon.UtilityAIConvertObjectBase",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIConvertObject16.png")), icon16));

	style->Set("ClassThumbnail.UtilityAIWeight",
	new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIWeight128.png")), icon128));

	style->Set("ClassIcon.UtilityAIWeight",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIWeight16.png")), icon16));

	return style;
}

const ISlateStyle& FUtilityAIEditorModule::Get()
{
	return *StyleSet.Get();
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAIEditorModule, UtilityAIEditor)