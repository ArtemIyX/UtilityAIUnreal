// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIConvertObject.generated.h"

/**
 * @class UUtilityAIConvertObject
 * @brief Base class for converting a normalized input value (0.0 to 1.0) to an output value using a customizable response curve.
 *
 * This class serves as a foundation for utility AI conversion objects that remap a float value in the range [0.0, 1.0]
 * (often representing a normalized score, consideration weight, or alpha) to another float value according to a defined
 * curve. Derived classes can override the evaluation behavior or use the built-in FRuntimeFloatCurve for common
 * non-linear mappings such as sinusoidal, cubic, ease-in/out, etc.
 *
 *
 * Typical use cases in Utility AI systems:
 * - Converting a linear consideration score into a non-linear utility value.
 * - Applying response curves for AI decision weighting (e.g., diminishing returns, thresholds, S-curves).
 * - Creating reusable curve-based remapping assets in the editor.
 *
 * @see FRuntimeFloatCurve
 */
UCLASS(BlueprintType, Blueprintable)
class UTILITYAI_API UUtilityAIConvertObject : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 * @param ObjectInitializer The object initializer (defaults to the global one).
	 */
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

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	TSoftObjectPtr<UTexture2D> DisplayIcon;*/

public:
	
	
	/**
	 * @brief Evaluates the conversion curve (or custom logic) for the given input alpha.
	 *
	 * This function is intended to be overridden in derived classes for custom evaluation logic.
	 * The default implementation (provided in the .cpp or a base Blueprint) typically samples the
	 * stored Curve at InAlpha clamped to [0.0, 1.0].
	 *
	 * @param InAlpha Normalized input value, expected to be in the range [0.0, 1.0].
	 * @return The converted/output value according to the curve or custom logic.
	 *
	 * @note This is a BlueprintNativeEvent, so Blueprint classes can implement custom logic
	 *       while C++ derived classes can override the virtual _Implementation function.
	 * @note Marked as BlueprintPure for easy use in Blueprint graphs without execution pins.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure)
	float Evaluate(float InAlpha) const;
};