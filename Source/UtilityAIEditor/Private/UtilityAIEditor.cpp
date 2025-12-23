#include "UtilityAIEditor.h"

#include "ConvObjThumbnailRenderer.h"
#include "UtilityAIConvertObject.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
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
	}
	UThumbnailManager::Get().UnregisterCustomRenderer(UUtilityAIConvertObjectBase::StaticClass());
}

TSharedRef<FSlateStyleSet> FUtilityAIEditorModule::Create()
{
	TSharedRef<FSlateStyleSet> style = MakeShareable(new FSlateStyleSet("UtilityAIStyle"));

	// Get plugin base directory
	FString contentDir = IPluginManager::Get().FindPlugin(TEXT("UtilityAI"))->GetBaseDir() / TEXT("Resources");

	style->SetContentRoot(contentDir);

	// Define the thumbnail brush (128x128 size)
	const FVector2D Icon128(128.0f, 128.0f);
	style->Set("ClassThumbnail.UtilityAIConsideration",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIConsideration128.png")), Icon128));

	style->Set("ClassThumbnail.UtilityAIConsideration.Background", FLinearColor(0.1f, 0.5f, 0.8f, 1.0f));

	// Smaller icon for lists/context menus (16x16)
	const FVector2D Icon16(16.0f, 16.0f);
	style->Set("ClassIcon.UtilityAIConsideration",
		new FSlateImageBrush(style->RootToContentDir(TEXT("UtilityAIConsideration16.png")), Icon16));

	return style;
}

const ISlateStyle& FUtilityAIEditorModule::Get()
{
	return *StyleSet.Get();
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAIEditorModule, UtilityAIEditor)