// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"


class UUtilityAIStateWeighted;

class FStateWeightedAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
protected:
	TWeakObjectPtr<UUtilityAIStateWeighted> WorkingAsset = nullptr;

public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost> InitToolkitHost, UObject* InObject);

public:
	virtual FName GetToolkitFName() const override { return FName(TEXT("StateWeightedAssetEditorApp")); } // Unique name to identify this editor
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("Weighted State Editor")); }

	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("StateWeightedAssetEditorApp"); } // Dont care
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.2f, 0.15f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return FString(TEXT("https://github.com/ArtemIyX/UtilityAIUnreal")); }

	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;

public:
	TWeakObjectPtr<UUtilityAIStateWeighted> GetWorkingAsset() const { return WorkingAsset; }

};