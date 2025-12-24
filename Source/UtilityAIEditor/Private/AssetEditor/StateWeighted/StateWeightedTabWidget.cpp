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
	/*TSharedPtr<ISinglePropertyView> SinglePropertyView */BaseScoreWidget = propertyModule.CreateSingleProperty(
		EditedAsset.Get(),
		GET_MEMBER_NAME_CHECKED(UUtilityAIStateWeighted, BaseScore),
		initParams
		);

	/*BaseScoreWidget = SinglePropertyView;*/

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
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SSpacer)
		]
	];
}

