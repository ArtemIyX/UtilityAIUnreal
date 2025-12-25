// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UUtilityAIStateWeighted;

class SStateWeightedTabWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStateWeightedTabWidget) {}
		// Correct: no leading underscore in the argument name
		SLATE_ARGUMENT(TWeakObjectPtr<UUtilityAIStateWeighted>, EditedAsset)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
protected:
	// Called when the float value changes in the spinner
	FReply OnBaseScoreChanged(float NewValue);

	// Helper to get current BaseScore safely
	float GetBaseScore() const;
public:
	TWeakObjectPtr<UUtilityAIStateWeighted> EditedAsset;


private:


	
	// Debug weight names for combo box
	TArray<TSharedPtr<FString>> DebugWeightNames;
    
	// Store selected weight name per row (key = Index)
	TMap<int32, TSharedPtr<FString>> SelectedWeightNames;
	
	TSharedPtr<SWidget> BaseScoreWidget;
	TSharedPtr<IPropertyHandleArray> SumArrayHandle;
	TSharedPtr<IPropertyRowGenerator> PropertyRowGenerator;

	TSharedPtr<SListView<TSharedPtr<int32>>> SumListWidget;
	TArray<TSharedPtr<int32>> SumItems;
    
	void RefreshSumList();

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<int32> Item, const TSharedRef<STableViewBase>& OwnerTable);
	FReply OnAddItem();
	FReply OnRemoveItem(int32 Index);
	void InitializeDebugWeightNames();
	void OnWeightNameSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo, int32 Index, TSharedPtr<IPropertyHandle> WeightNameHandle);
	TSharedRef<SWidget> OnGenerateWeightNameWidget(TSharedPtr<FString> InItem);
	TSharedRef<SWidget> CreateFloatClassDebugWidget(int32 InIndex);
	TSharedRef<SWidget> CreateFloatDetailsWidget(int32 InIndex);
	FText GetCurrentWeightNameText(int32 Index, TSharedPtr<IPropertyHandle> WeightNameHandle) const;

	void OnFloatConverterClassChanged(UClass* InClass, int32 InIndex);
};
