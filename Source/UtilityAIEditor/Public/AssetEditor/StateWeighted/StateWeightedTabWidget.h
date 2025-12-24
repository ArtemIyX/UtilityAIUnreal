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
	TSharedPtr<SWidget> BaseScoreWidget;
};
