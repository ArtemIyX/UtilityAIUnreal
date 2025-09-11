// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIState.generated.h"


class UUtilityAIComponent;
class UUtilityAIContext;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, Blueprintable)
class UTILITYAI_API UUtilityAIState : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIState();

protected:
	// The state type - set in constructor or defaults
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI State")
	int32 StateType = 0;

public:
	// Virtual function to compute score based on context (0.0f to 1.0f typically)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI State")
	float GetScore(const UUtilityAIContext* Context) const;

	// Virtual execution function - override in child classes to perform actions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI State")
	void Execute(AActor* Owner, UUtilityAIComponent* InSystem, const UUtilityAIContext* InContext);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI State")
	FORCEINLINE int32 GetStateType() const { return StateType; }
};
