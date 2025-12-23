#include "UtilityAIEditor.h"

#include "ConvObjThumbnailRenderer.h"
#include "UtilityAIConvertObject.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FUtilityAIEditorModule"

void FUtilityAIEditorModule::StartupModule()
{
	if (GEditor)
	{
		UThumbnailManager::Get().RegisterCustomRenderer(UUtilityAIConvertObjectBase::StaticClass(),
			UConvObjThumbnailRenderer::StaticClass());
	}
}

void FUtilityAIEditorModule::ShutdownModule()
{
	if (GEditor && FSlateApplication::IsInitialized())
	{
		UThumbnailManager::Get().UnregisterCustomRenderer(UUtilityAIConvertObjectBase::StaticClass());
	}
	
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAIEditorModule, UtilityAIEditor)