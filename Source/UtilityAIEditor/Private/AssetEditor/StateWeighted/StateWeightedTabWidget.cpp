// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "AssetEditor/StateWeighted/StateWeightedTabWidget.h"

#include "DetailWidgetRow.h"
#include "IDetailTreeNode.h"
#include "IPropertyRowGenerator.h"
#include "ISinglePropertyView.h"
#include "PropertyCustomizationHelpers.h"
#include "UtilityAIStateWeighted.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Input/Reply.h"
#include "SGameplayTagCombo.h"
#include "UtilityAIWeight.h"

FReply SStateWeightedTabWidget::OnBaseScoreChanged(float NewValue)
{
	if (EditedAsset.IsValid())
	{
		UUtilityAIStateWeighted* Asset = EditedAsset.Get();

		if (!FMath::IsNearlyEqual(Asset->BaseScore, NewValue))
		{
			// Proper undo/redo support
			Asset->Modify();
			Asset->BaseScore = NewValue;

		}
	}
	return FReply::Handled();
}

float SStateWeightedTabWidget::GetBaseScore() const
{
	if (EditedAsset.IsValid())
	{
		return EditedAsset->BaseScore;
	}
	return 0.0f;
}


TSharedPtr<IPropertyHandle> SStateWeightedTabWidget::FindProperty(const TArray<TSharedRef<IDetailTreeNode>>& InRootNodes, const FName& InPropertyName)
{
	for (const TSharedRef<IDetailTreeNode>& node : InRootNodes)
	{
		TArray<TSharedRef<IDetailTreeNode>> children;
		node->GetChildren(children);
		for (const TSharedRef<IDetailTreeNode>& child : children)
		{
			TSharedPtr<IPropertyHandle> propertyHandle = child->CreatePropertyHandle();
			if (propertyHandle.IsValid() && propertyHandle->GetProperty()->GetFName() == /*GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, Sum)*/ InPropertyName)
			{
				return propertyHandle;
			}
		}
	}
	return {};
}

void SStateWeightedTabWidget::Construct(const FArguments& InArgs)
{
	EditedAsset = InArgs._EditedAsset;
	//InitializeDebugWeightNames();

	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FPropertyRowGeneratorArgs args;
	args.bAllowEditingClassDefaultObjects = true;
	args.bShouldShowHiddenProperties = true;
	args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Show;
	args.bAllowMultipleTopLevelObjects = true;

	PropertyRowGenerator = propertyModule.CreatePropertyRowGenerator(args);
	PropertyRowGenerator->SetObjects({ EditedAsset.Get() });

	const TArray<TSharedRef<IDetailTreeNode>>& rootNodes = PropertyRowGenerator->GetRootTreeNodes();

	SumArrayHandle = FindProperty(rootNodes, GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, Sum))->AsArray();
	WeightTemplateObjectHandle = FindProperty(rootNodes, GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, WeightTemplate));

	InitializeWeightComboItems();

	FSinglePropertyParams initParams;
	initParams.NamePlacement = EPropertyNamePlacement::Left;
	BaseScoreWidget = propertyModule.CreateSingleProperty(
		EditedAsset.Get(),
		GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, BaseScore),
		initParams
		);

	WeightTemplateClassWidget = propertyModule.CreateSingleProperty(
		EditedAsset.Get(),
		GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, WeightTemplate),
		initParams
		);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(10)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("State Configuration"))
			.Justification(ETextJustify::Center)
		]
		+ SVerticalBox::Slot()
		.Padding(20, 10)
		.AutoHeight()
		[
			BaseScoreWidget.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.Padding(20, 10)
		.AutoHeight()
		[
			SNew(SObjectPropertyEntryBox)
			.PropertyHandle(WeightTemplateObjectHandle)
			.OnObjectChanged(this, &SStateWeightedTabWidget::WeightTemplateObjectChanged)
			.AllowClear(false)
			.AllowCreate(true)
			.AllowedClass(UUtilityAIWeight::StaticClass())
			/*WeightTemplateClassWidget.ToSharedRef()*/
			/*SNew(SClassPropertyEntryBox)
			.SelectedClass(EditedAsset->WeightTemplate)
			.AllowedClasses({ UUtilityAIWeight::StaticClass() })
			.OnSetClass_Lambda([this](const UClass* InClass) {
				OnWeightTemplateClassChanged(const_cast<UClass*>(InClass));
			})*/
		]
		+ SVerticalBox::Slot()
		.Padding(20, 10)
		.AutoHeight()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Sum Array:"))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					SNew(SSpacer)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(FText::FromString("Add Item"))
					.OnClicked(this, &SStateWeightedTabWidget::OnAddItem)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(SumListWidget, SListView<TSharedPtr<int32>>)
				.ListItemsSource(&SumItems)
				.OnGenerateRow(this, &SStateWeightedTabWidget::OnGenerateRowForList)
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SSpacer)
		]
	];

	RefreshSumList();
}

void SStateWeightedTabWidget::WeightTemplateObjectChanged(const FAssetData& InAssetData)
{
	UObject* asset = InAssetData.GetAsset();
	if (UUtilityAIWeight* weight = Cast<UUtilityAIWeight>(asset))
	{
		RefreshSumList();
	}
}

void SStateWeightedTabWidget::RefreshSumList()
{
	SumItems.Empty();

	if (EditedAsset.IsValid())
	{
		for (int32 i = 0; i < EditedAsset->Sum.Num(); ++i)
		{
			SumItems.Add(MakeShared<int32>(i));
		}
	}

	if (SumListWidget.IsValid())
	{
		SumListWidget->RequestListRefresh();
	}
}

TSharedRef<ITableRow> SStateWeightedTabWidget::OnGenerateRowForList(TSharedPtr<int32> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	int32 index = *Item;

	if (!SumArrayHandle.IsValid())
	{
		return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
			[
				SNew(STextBlock).Text(FText::FromString("Invalid SumArrayHandle"))
			];
	}

	uint32 n;
	SumArrayHandle->GetNumElements(n);
	if (n == 0 || static_cast<uint32>(index) >= n)
	{
		return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
			[
				SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("Invalid Index Handle %d"), index)))
			];
	}

	TSharedRef<IPropertyHandle> elementHandle = SumArrayHandle->GetElement(index);

	// Get handles for each property
	TSharedPtr<IPropertyHandle> weightNameHandle = elementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, WeightName));
	TSharedPtr<IPropertyHandle> considerationHandle = elementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, Consideration));
	TSharedPtr<IPropertyHandle> floatConverterHandle = elementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, FloatConverter));
	TSharedPtr<IPropertyHandle> floatConverterClassHandle = elementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, FloatConverterClass));

	UObject* classValueObject;
	floatConverterClassHandle->GetValue(classValueObject);
	UClass* classValue = Cast<UClass>(classValueObject);

	return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
		[
			SNew(SHorizontalBox)

			// Column 1: Weight Name and Consideration (stacked vertically)
			+ SHorizontalBox::Slot()
			  .FillWidth(0.3f) // Adjusted width to accommodate both fields
			  .Padding(5, 2)
			[
				SNew(SVerticalBox)

				// Weight Name subsection
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 5)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Weight Name"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						weightNameHandle.IsValid()
						? SNew(SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&WeightNames)
						.OnGenerateWidget(this, &SStateWeightedTabWidget::OnGenerateWeightNameWidget)
						.OnSelectionChanged(this, &SStateWeightedTabWidget::OnWeightNameSelected, index, weightNameHandle)
						[
							SNew(STextBlock)
							.Text(this, &SStateWeightedTabWidget::GetCurrentWeightNameText, index, weightNameHandle)
						]
						: SNullWidget::NullWidget
					]
				]

				// Consideration subsection
				+ SVerticalBox::Slot()
				.FillHeight(0.33f) // ~1/3 of the column
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 0, 0, 5)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Consideration"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						considerationHandle.IsValid()
						? SNew(SGameplayTagCombo)
						.PropertyHandle(considerationHandle)
						: SNullWidget::NullWidget
					]
				]
			]

			// Column 2: FloatConverter Class and Details (stacked vertically)
			+ SHorizontalBox::Slot()
			  .FillWidth(0.6f) // Takes most of the remaining space
			  .Padding(5, 2)
			[
				SNew(SVerticalBox)

				// Class selector row
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(SClassPropertyEntryBox)
					.SelectedClass(classValue)
					.AllowedClasses({ UUtilityAIConvertObjectBase::StaticClass() })
					.OnSetClass_Lambda([this, index](const UClass* InClass) {
						OnFloatConverterClassChanged(const_cast<UClass*>(InClass), index);
					})
				]

				// Details widget row
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					EditedAsset.IsValid() ? CreateFloatDetailsWidget(index) : SNullWidget::NullWidget
				]
			]

			// Remove button
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5, 2)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.Text(FText::FromString("X"))
				.ButtonStyle(FAppStyle::Get(), "SimpleButton")
				.OnClicked(this, &SStateWeightedTabWidget::OnRemoveItem, index)
			]
		];
}


FReply SStateWeightedTabWidget::OnAddItem()
{
	if (EditedAsset.IsValid())
	{
		FWeightedInitParams data = FWeightedInitParams();
		UUtilityAIConvertObjectBase* floatConverter = NewObject<UUtilityAIConvertObjectBase>(EditedAsset.Get(),
			UUtilityAIConvertObjectBase::StaticClass(), NAME_None, RF_Transactional);
		if (floatConverter)
		{
			data.FloatConverter = floatConverter;
		}

		EditedAsset->Sum.Add(data);
		EditedAsset->MarkPackageDirty();
		RefreshSumList();
	}
	return FReply::Handled();
}

FReply SStateWeightedTabWidget::OnRemoveItem(int32 Index)
{
	if (EditedAsset.IsValid() && EditedAsset->Sum.IsValidIndex(Index))
	{
		EditedAsset->Sum.RemoveAt(Index);
		EditedAsset->MarkPackageDirty();
		RefreshSumList();
	}
	return FReply::Handled();
}

void SStateWeightedTabWidget::InitializeDebugWeightNames()
{
	WeightNames.Empty();
	WeightNames.Add(MakeShared<FString>("a"));
	WeightNames.Add(MakeShared<FString>("b"));
	WeightNames.Add(MakeShared<FString>("c"));
	WeightNames.Add(MakeShared<FString>("d"));
}

void SStateWeightedTabWidget::InitializeWeightComboItems()
{
	WeightNames.Empty();
	if (!EditedAsset.IsValid())
		return;

	UUtilityAIWeight* weightTemplate = EditedAsset->WeightTemplate;
	if (!weightTemplate)
	{
		WeightNames.Add(MakeShared<FString>("Select WeightTemplate"));
	}
	else
	{
		if (weightTemplate)
		{
			TArray<FString> keys;
			weightTemplate->WeightMap.GetKeys(keys);
			const int32 n = keys.Num();
			WeightNames.Reserve(n);
			for (int32 i = 0; i < n; i++)
			{
				WeightNames.Add(MakeShared<FString>(keys[i]));
			}
		}
	}
}

void SStateWeightedTabWidget::OnWeightNameSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo, int32 Index, TSharedPtr<IPropertyHandle> WeightNameHandle)
{
	if (NewSelection.IsValid() && WeightNameHandle.IsValid())
	{
		SelectedWeightNames.Add(Index, NewSelection);
		WeightNameHandle->SetValue(*NewSelection);
	}
}

TSharedRef<SWidget> SStateWeightedTabWidget::OnGenerateWeightNameWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

FText SStateWeightedTabWidget::GetCurrentWeightNameText(int32 Index, TSharedPtr<IPropertyHandle> WeightNameHandle) const
{
	if (SelectedWeightNames.Contains(Index))
	{
		return FText::FromString(*SelectedWeightNames[Index]);
	}

	// Try to get value from property handle
	if (WeightNameHandle.IsValid())
	{
		FString CurrentValue;
		if (WeightNameHandle->GetValue(CurrentValue) == FPropertyAccess::Success)
		{
			return FText::FromString(CurrentValue);
		}
	}

	return FText::FromString("Select...");
}

void SStateWeightedTabWidget::OnFloatConverterClassChanged(UClass* InClass, int32 InIndex)
{
	if (!EditedAsset.IsValid())
	{
		return;
	}
	UUtilityAIStateWeighted* editedAsset = EditedAsset.Get();
	if (!editedAsset->Sum.IsValidIndex(InIndex))
	{
		return;
	}
	FWeightedInitParams& arrayElement = editedAsset->Sum[InIndex];
	arrayElement.FloatConverterClass = InClass;

	if (arrayElement.FloatConverter)
	{
		arrayElement.FloatConverter->ConditionalBeginDestroy();
	}
	arrayElement.FloatConverter = nullptr;

	if (InClass != nullptr)
	{
		UUtilityAIConvertObjectBase* floatConverter = NewObject<UUtilityAIConvertObjectBase>(editedAsset, InClass, NAME_None, RF_Transactional);
		if (floatConverter)
		{
			arrayElement.FloatConverter = floatConverter;
		}
	}

	EditedAsset.Get()->MarkPackageDirty();
	RefreshSumList();
}


/*
void SStateWeightedTabWidget::OnWeightTemplateClassChanged(UClass* InClass)
{

	RefreshSumList();
}
*/


TSharedRef<SWidget> SStateWeightedTabWidget::CreateFloatClassDebugWidget(int32 InIndex)
{
	UClass* selectedClass = EditedAsset.Get()->Sum[InIndex].FloatConverterClass;
	if (selectedClass == nullptr)
	{
		return SNew(STextBlock).Text(FText::FromString(TEXT("Select a class")));
	}

	return SNew(STextBlock).Text(FText::FromString(
			FString::Printf(TEXT("%s -> %s"), *GetNameSafe(selectedClass), *GetNameSafe(EditedAsset.Get()->Sum[InIndex].FloatConverter))
			));
}

TSharedRef<SWidget> SStateWeightedTabWidget::CreateFloatDetailsWidget(int32 InIndex)
{
	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs args;
	{
		args.bAllowSearch = false;
		args.bHideSelectionTip = true;
		args.bLockable = false;
		args.bSearchInitialKeyFocus = true;
		args.bUpdatesFromSelection = false;
		args.NotifyHook = nullptr;
		args.bShowOptions = true;
		args.bShowModifiedPropertiesOption = false;
		args.bShowScrollBar = false;
	}

	if (!EditedAsset.Get()->Sum.IsValidIndex(InIndex))
	{
		return SNullWidget::NullWidget;
	}
	TSharedRef<IDetailsView> detailsView = propertyModule.CreateDetailView(args);
	detailsView->SetObject(EditedAsset.Get()->Sum[InIndex].FloatConverter);

	return detailsView;
}