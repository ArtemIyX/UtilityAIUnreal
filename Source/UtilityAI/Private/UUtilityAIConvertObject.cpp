// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "UUtilityAIConvertObject.h"

UUUtilityAIConvertObject::UUUtilityAIConvertObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FRichCurve* RichCurve = Curve.GetRichCurve(); // Non-const reference for editing

	RichCurve->AddKey(0.0f, 0.0f);
	RichCurve->AddKey(1.0f, 1.0f);
}

float UUUtilityAIConvertObject::Evaluate_Implementation(float InAlpha) const
{
	float clamped = FMath::Clamp(InAlpha, 0.0f, 1.0f);
	return Curve.GetRichCurveConst()->Eval(clamped);
}