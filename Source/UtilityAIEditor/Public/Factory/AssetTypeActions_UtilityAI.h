// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "UtilityAIState.h"


class UTILITYAIEDITOR_API FAssetTypeActions_UtilityAI : public FAssetTypeActions_Base
{

public:
	FAssetTypeActions_UtilityAI();

protected:
	TSubclassOf<UObject> SupportedClass;
	FText Name;

public:
	virtual UClass* GetSupportedClass() const override { return SupportedClass; };
	virtual FText GetName() const override { return Name; };
	virtual FColor GetTypeColor() const override { return FColor::FromHex("#641896"); }
	virtual uint32 GetCategories() override;
};