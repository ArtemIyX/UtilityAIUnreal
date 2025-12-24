// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Factory/StateAssetFactory.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "UtilityAIState.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"


UStateAssetFactory::UStateAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UUtilityAIState::StaticClass();
	TargetObjectClass = UUtilityAIState::StaticClass();
}

void UStateAssetFactory::OnClassPicked(UClass* InClass)
{
	SelectedByUserClass = InClass;
}

bool UStateAssetFactory::ConfigureProperties()
{
	//FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	FClassViewerInitializationOptions pickOptions;
	pickOptions.Mode = EClassViewerMode::ClassPicker;
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

UObject* UStateAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UBlueprint* blueprintObject = FKismetEditorUtilities::CreateBlueprint(TargetObjectClass, InParent, InName, BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(), CallingContext);
	return blueprintObject;

}

bool UStateAssetFactory::CanCreateNew() const
{
	return true;
}