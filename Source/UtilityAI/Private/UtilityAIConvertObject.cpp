// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UtilityAIConvertObject.h"

/**
 * Default implementation samples the Curve property.
 * InAlpha is automatically clamped to [0.0, 1.0].
 */
float UUtilityAIConvertObject::Evaluate_Implementation(float InAlpha) const
{
	float clamped = FMath::Clamp(InAlpha, 0.0f, 1.0f);
	return Curve.GetRichCurveConst()->Eval(clamped);
}

UUtilityAIConvertObject::UUtilityAIConvertObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FRichCurve* RichCurve = Curve.GetRichCurve(); // Non-const reference for editing

	RichCurve->AddKey(0.0f, 0.0f);
	RichCurve->AddKey(1.0f, 1.0f);
}