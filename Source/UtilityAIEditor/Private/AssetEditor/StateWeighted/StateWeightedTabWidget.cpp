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

void SStateWeightedTabWidget::Construct(const FArguments& InArgs)
{
	EditedAsset = InArgs._EditedAsset;
	InitializeDebugWeightNames();

	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FPropertyRowGeneratorArgs args;
	args.bAllowEditingClassDefaultObjects = true;
	args.bShouldShowHiddenProperties = true;
	args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Show;
	args.bAllowMultipleTopLevelObjects = true;

	PropertyRowGenerator = propertyModule.CreatePropertyRowGenerator(args);
	PropertyRowGenerator->SetObjects({ EditedAsset.Get() });

	const TArray<TSharedRef<IDetailTreeNode>>& RootNodes = PropertyRowGenerator->GetRootTreeNodes();
	for (const TSharedRef<IDetailTreeNode>& Node : RootNodes)
	{
		TArray<TSharedRef<IDetailTreeNode>> children;
		Node->GetChildren(children);
		for (const TSharedRef<IDetailTreeNode>& child : children)
		{
			TSharedPtr<IPropertyHandle> PropertyHandle = child->CreatePropertyHandle();
			if (PropertyHandle.IsValid() && PropertyHandle->GetProperty()->GetFName() == GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, Sum))
			{
				SumArrayHandle = PropertyHandle->AsArray();
				break;
			}
		}
	}

	FSinglePropertyParams initParams;
	initParams.NamePlacement = EPropertyNamePlacement::Left;
	BaseScoreWidget = propertyModule.CreateSingleProperty(
		EditedAsset.Get(),
		GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, BaseScore),
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
	int32 Index = *Item;

	if (!SumArrayHandle.IsValid())
	{
		return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
			[
				SNew(STextBlock).Text(FText::FromString("Invalid SumArrayHandle"))
			];
	}

	uint32 n;
	SumArrayHandle->GetNumElements(n);
	if (n == 0 || static_cast<uint32>(Index) >= n)
	{
		return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
			[
				SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("Invalid Index Handle %d"), Index)))
			];
	}

	TSharedRef<IPropertyHandle> ElementHandle = SumArrayHandle->GetElement(Index);

	// Get handles for each property
	TSharedPtr<IPropertyHandle> WeightNameHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, WeightName));
	TSharedPtr<IPropertyHandle> ConsiderationHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, Consideration));
	TSharedPtr<IPropertyHandle> FloatConverterHandle = ElementHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWeightedInitParams, FloatConverter));

	return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
		[
			SNew(SHorizontalBox)

			// WeightName - Now with ComboBox
			+ SHorizontalBox::Slot()
			.FillWidth(0.3f)
			.Padding(5, 2)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Weight Name"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					WeightNameHandle.IsValid()
					? SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&DebugWeightNames)
					.OnGenerateWidget(this, &SStateWeightedTabWidget::OnGenerateWeightNameWidget)
					.OnSelectionChanged(this, &SStateWeightedTabWidget::OnWeightNameSelected, Index, WeightNameHandle)
					[
						SNew(STextBlock)
						.Text(this, &SStateWeightedTabWidget::GetCurrentWeightNameText, Index, WeightNameHandle)
					]
					: SNullWidget::NullWidget
				]
			]

			// Consideration
			+ SHorizontalBox::Slot()
			.FillWidth(0.3f)
			.Padding(5, 2)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Consideration"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					ConsiderationHandle.IsValid()
					? ConsiderationHandle->CreatePropertyValueWidget(false)
					: SNullWidget::NullWidget
				]
			]

			// FloatConverter - needs special handling for instanced objects
			+ SHorizontalBox::Slot()
			.FillWidth(0.3f)
			.Padding(5, 2)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Float Converter"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					FloatConverterHandle.IsValid()
					? SNew(SObjectPropertyEntryBox)
					                               .PropertyHandle(FloatConverterHandle)
                            /*.AllowedClass(::StaticClass()) // Replace with your base class*/
					                               .DisplayUseSelected(true)
					                               .DisplayBrowse(true)
					                               .EnableContentPicker(true)
					                               .DisplayCompactSize(true)
					: SNullWidget::NullWidget
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
				.OnClicked(this, &SStateWeightedTabWidget::OnRemoveItem, Index)
			]
		];
}

FReply SStateWeightedTabWidget::OnAddItem()
{
	if (EditedAsset.IsValid())
	{
		EditedAsset->Sum.Add(FWeightedInitParams());
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
	DebugWeightNames.Empty();
	DebugWeightNames.Add(MakeShared<FString>("a"));
	DebugWeightNames.Add(MakeShared<FString>("b"));
	DebugWeightNames.Add(MakeShared<FString>("c"));
	DebugWeightNames.Add(MakeShared<FString>("d"));
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