// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAIConvertObjectBase.h"
#include "UtilityAIConvertObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIConvertObject : public UUtilityAIConvertObjectBase
{
	GENERATED_BODY()

public:
	UUtilityAIConvertObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	/**
		 * @brief The runtime curve used to remap the input alpha value.
		 *
		 * This curve maps input values from 0.0 to 1.0 on the X-axis to output values on the Y-axis.
		 * It can be edited in the Details panel of derived Blueprint classes or asset instances.
		 * Common curves include:
		 * - Linear (default identity mapping)
		 * - Ease In / Ease Out
		 * - Sinusoidal
		 * - Cubic / Quadratic
		 * - Custom hand-tuned curves
		 *
		 * @note Visible only in defaults to prevent accidental runtime modification.
		 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	FRuntimeFloatCurve Curve;

public:
	virtual float Evaluate_Implementation(float InAlpha) const override;
};