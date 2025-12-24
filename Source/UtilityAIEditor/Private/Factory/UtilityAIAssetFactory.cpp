// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Factory/UtilityAIAssetFactory.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "UtilityAIState.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"


UUtilityAIAssetFactory::UUtilityAIAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UObject::StaticClass();
	TargetObjectClass = UObject::StaticClass();
}

bool UUtilityAIAssetFactory::ConfigureProperties()
{
	//FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	FClassViewerInitializationOptions pickOptions;
	pickOptions.Mode = EClassViewerMode::ClassPicker;
	pickOptions.DisplayMode = EClassViewerDisplayMode::TreeView; 
	pickOptions.bShowNoneOption = false;
	pickOptions.bExpandRootNodes = true;

	// THIS IS THE KEY: Restrict to subclasses of your base class
	pickOptions.ExtraPickerCommonClasses.Add(TargetObjectClass);
	
	// Optional: Set display mode (tree view is default and best)
	pickOptions.bIsBlueprintBaseOnly = false;
	pickOptions.bIsPlaceableOnly = false;

	TSharedPtr<FUtilityAiClassViewFilter> filter = MakeShareable(new FUtilityAiClassViewFilter());
	filter->AllowedChildrenOfClasses.Add(TargetObjectClass);
	pickOptions.ClassFilters.Add(filter.ToSharedRef());

	// Show the modal dialog
	UClass* selectedClass;
	const bool bSelected = SClassPickerDialog::PickClass(FText::FromString(
		FString::Printf(TEXT("Pick %s"), SupportedClass ? *SupportedClass->GetFName().ToString() : TEXT("InvalidClass"))),
		pickOptions,
		selectedClass,
		TargetObjectClass);

	SelectedByUserClass = selectedClass;
	return bSelected;
}

UObject* UUtilityAIAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UBlueprint* blueprintObject = FKismetEditorUtilities::CreateBlueprint(SelectedByUserClass, InParent, InName, BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(), CallingContext);
	return blueprintObject;

}

bool UUtilityAIAssetFactory::CanCreateNew() const
{
	return true;
}