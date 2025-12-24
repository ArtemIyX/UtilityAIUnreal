// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "AssetEditor/StateWeighted/StateWeightedTabWidget.h"

#include "DetailWidgetRow.h"
#include "ISinglePropertyView.h"
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

	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

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
			/*.Font(FSlateFontInfo(FName("Roboto"), 24))*/
			                .Justification(ETextJustify::Center)
		]
		+ SVerticalBox::Slot()
		.Padding(20, 10)
		.AutoHeight()
		[
			// Embed the single property widget here (it handles its own layout)
			BaseScoreWidget.ToSharedRef()
		]
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

	return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.Padding(5)
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("Item %d"), Index)))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5)
			[
				SNew(SButton)
				.Text(FText::FromString("Remove"))
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