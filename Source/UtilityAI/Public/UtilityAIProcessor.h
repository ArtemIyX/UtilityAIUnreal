// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIProcessor.generated.h"

class UUtilityAIContext;
class UUtilityAIState;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIProcessor : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIProcessor();

protected:
	// The processor type - set in constructor or defaults
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Processor")
	int32 ProcessorType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Processor")
	TArray<TSubclassOf<UUtilityAIState>> DefaultStates;
	
	// List of states configured for this processor
	UPROPERTY(BlueprintReadOnly, Category = "AI Processor")
	TArray<TObjectPtr<UUtilityAIState>> States;

protected:
	// Find the state with the highest score
	virtual UUtilityAIState* FindHighestScoringState(const UUtilityAIContext* Context, float& OutScore) const;

public:

	UFUNCTION(BlueprintCallable, Category="AI Processor")
	virtual void InitDefaultStates();
	
	/**
	 * @brief Getter for array of possible States
	 * @see States
	 */
	UFUNCTION(BlueprintCallable, Category="AI Processor")
	void GetCurrentStates(TArray<UUtilityAIState*>& OutStates) const;

	/**
	 * @brief Getter for ProcessorType
	 * @see ProcessorType
	 * @return ProcessorType
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AI Processor")
	FORCEINLINE int32 GetProcessorType() const { return ProcessorType; }

	// Add a state to the processor
	UFUNCTION(BlueprintCallable, Category = "AI Processor")
	void AddState(UUtilityAIState* InNewState);

	// Remove a state by pointer
	UFUNCTION(BlueprintCallable, Category = "AI Processor")
	void RemoveState(UUtilityAIState* InStateToRemove);

	// Get best state based on context
	UFUNCTION(BlueprintCallable, Category = "AI Processor")
	UUtilityAIState* GetBestState(const UUtilityAIContext* InContext, float& OutScore) const;

	// Get state by type
	UFUNCTION(BlueprintCallable, Category = "AI Processor")
	UUtilityAIState* GetStateByType(int32 InType) const;
};
