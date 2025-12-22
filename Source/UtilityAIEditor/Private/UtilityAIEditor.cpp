#include "UtilityAIEditor.h"

#include "ConvObjThumbnailRenderer.h"
#include "UtilityAIConvertObject.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FUtilityAIEditorModule"

void FUtilityAIEditorModule::StartupModule()
{
	UThumbnailManager::Get().RegisterCustomRenderer(UUtilityAIConvertObject::StaticClass(),
		UConvObjThumbnailRenderer::StaticClass());
}

void FUtilityAIEditorModule::ShutdownModule()
{
	UThumbnailManager::Get().UnregisterCustomRenderer(UUtilityAIConvertObject::StaticClass());
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAIEditorModule, UtilityAIEditor)