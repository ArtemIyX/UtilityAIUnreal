// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "AssetEditor/StateWeighted/StateWeightedAssetEditorApp.h"

#include "UtilityAIStateWeighted.h"
#include "AssetEditor/StateWeighted/StateWeightedAppMode.h"

void FStateWeightedAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);

}

void FStateWeightedAssetEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost> InitToolkitHost, UObject* InObject)
{
	TArray<UObject*> objectToEdit;
	objectToEdit.Add(InObject);

	WorkingAsset = Cast<UUtilityAIStateWeighted>(InObject);
	InitAssetEditor(Mode, InitToolkitHost, FName(TEXT("StateWeightedAssetEditorApp")),
		FTabManager::FLayout::NullLayout,
		true,
		true,
		objectToEdit);

	AddApplicationMode(TEXT("StateWeightedAppMode"), MakeShareable(new FStateWeightedAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("StateWeightedAppMode"));
}

void FStateWeightedAssetEditorApp::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingStarted(Toolkit);

}

void FStateWeightedAssetEditorApp::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingFinished(Toolkit);

}