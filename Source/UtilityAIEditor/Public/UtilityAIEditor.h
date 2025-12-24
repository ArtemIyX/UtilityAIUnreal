#pragma once

#include "CoreMinimal.h"
#include "IAssetTools.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"


class FUtilityAIEditorModule : public IModuleInterface
{

protected:
	TArray<TSharedPtr<IAssetTypeActions>> AssetActions;

protected:
	template<typename T>
	void RegisterAssetActions(IAssetTools& InAssetTools)
	{
		TSharedPtr<IAssetTypeActions> stateActions = MakeShareable(new T());
		AssetActions.Add(stateActions);
		InAssetTools.RegisterAssetTypeActions(stateActions.ToSharedRef());
	}

	void UnregisterAssetActions();

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> StyleSet;
	static const ISlateStyle& Get();
};