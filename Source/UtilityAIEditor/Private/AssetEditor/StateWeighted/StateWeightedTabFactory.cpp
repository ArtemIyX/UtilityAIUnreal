// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "AssetEditor/StateWeighted/StateWeightedTabFactory.h"

#include "AssetEditor/StateWeighted/StateWeightedAssetEditorApp.h"
#include "AssetEditor/StateWeighted/StateWeightedTabWidget.h"

FStateWeightedTabFactory::FStateWeightedTabFactory(TSharedPtr<FStateWeightedAssetEditorApp> InApp)
	: FWorkflowTabFactory(FName(TEXT("StateWeightedTab")),
		StaticCastSharedPtr<FAssetEditorToolkit, FStateWeightedAssetEditorApp>(InApp))
{
	AppWeakPtr = InApp.ToWeakPtr();
	TabLabel = FText::FromString((TEXT("Score (Weighted)")));
	ViewMenuDescription = FText::FromString(TEXT("Show Utility AI Score Object properties"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Utility AI Score Object"));
}

TSharedRef<SWidget> FStateWeightedTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FStateWeightedAssetEditorApp> appPtr = AppWeakPtr.Pin();
	TWeakObjectPtr<UUtilityAIStateWeighted> workingAsset = appPtr->GetWorkingAsset();

	return SNew(SStateWeightedTabWidget).EditedAsset(workingAsset);
}

FText FStateWeightedTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Score (Weighted)"));
}