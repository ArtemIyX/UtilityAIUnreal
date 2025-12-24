// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#include "UtilityAIConvertObjectBase.h"

UUtilityAIConvertObjectBase::UUtilityAIConvertObjectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

/**
 * Default implementation samples the Curve property.
 * InAlpha is automatically clamped to [0.0, 1.0].
 */
float UUtilityAIConvertObjectBase::Evaluate_Implementation(float InAlpha) const
{
	return 1.0f - InAlpha;
}

