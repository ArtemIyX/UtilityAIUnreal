// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FStateWeightedAppMode : public FApplicationMode
{
public:
	FStateWeightedAppMode(TSharedPtr<class FStateWeightedAssetEditorApp> InApp);
public:
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class FStateWeightedAssetEditorApp> AppWeakPtr;
	FWorkflowAllowedTabSet Tabs;
};
