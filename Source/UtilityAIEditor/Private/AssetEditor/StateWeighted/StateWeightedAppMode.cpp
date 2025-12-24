// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "AssetEditor/StateWeighted/StateWeightedAppMode.h"

#include "AssetEditor/StateWeighted/StateWeightedAssetEditorApp.h"
#include "AssetEditor/StateWeighted/StateWeightedTabFactory.h"

FStateWeightedAppMode::FStateWeightedAppMode(TSharedPtr<FStateWeightedAssetEditorApp> InApp)
	: FApplicationMode(TEXT("StateWeightedAppMode"))
{
	this->AppWeakPtr = InApp.ToWeakPtr();
	Tabs.RegisterFactory(MakeShareable(new FStateWeightedTabFactory(InApp)));

	TabLayout = FTabManager::NewLayout(FName(TEXT("StateWeightedAppModeLayout")))
		->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			                             ->Split(
				                             FTabManager::NewStack()->AddTab(FName(TEXT("StateWeightedTab")), ETabState::OpenedTab)
				                             ));
}

void FStateWeightedAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FStateWeightedAssetEditorApp> appPtr = AppWeakPtr.Pin();
	appPtr->PushTabFactories(Tabs);

	// Call super
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FStateWeightedAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();

}

void FStateWeightedAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();

}