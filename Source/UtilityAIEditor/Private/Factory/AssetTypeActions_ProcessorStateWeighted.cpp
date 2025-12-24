#include "AssetEditor/StateWeighted/StateWeightedAssetEditorApp.h"
#include "Factory/UtilityAIFactories.h"

UObject* UProcessorStateWeightedFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UUtilityAIStateWeighted>(InParent, SelectedByUserClass, InName, Flags);
}

void FAssetTypeActions_ProcessorStateWeighted::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//FAssetTypeActions_UtilityAI::OpenAssetEditor(InObjects, EditWithinLevelEditor);

	EToolkitMode::Type mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* obj : InObjects)
	{
		UUtilityAIStateWeighted* stateAsset = Cast<UUtilityAIStateWeighted>(obj);
		if (stateAsset)
		{
			TSharedRef<FStateWeightedAssetEditorApp> editor(new FStateWeightedAssetEditorApp());
			editor->InitEditor(mode, EditWithinLevelEditor, stateAsset);
		}
	}
}