// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIProcessor.generated.h"

class UUtilityAIContext;
class UUtilityAIState;

/**
 * @brief Manages a collection of states and determines the best one to execute
 */
UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIProcessor : public UObject
{
    GENERATED_BODY()

public:
    /** @brief Default constructor for UUtilityAIProcessor */
    UUtilityAIProcessor();

protected:
    /**
     * @brief Type identifier for the processor
     * @note Must be set in C++
     */
    UPROPERTY(BlueprintReadOnly, Category = "AI Processor")
    int32 ProcessorType;

    /** @brief Array of default state classes */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Processor")
    TArray<TSubclassOf<UUtilityAIState>> DefaultStates;
    
    /** @brief List of active state instances configured for this processor */
    UPROPERTY(BlueprintReadOnly, Category = "AI Processor")
    TArray<TObjectPtr<UUtilityAIState>> States;

protected:
    /**
     * @brief Finds the state with the highest score
     * @param Context Current data context
     * @param OutScore Output parameter for the highest score
     * @return Pointer to the highest scoring state object, or nullptr if none found
     */
    virtual UUtilityAIState* FindHighestScoringState(const UUtilityAIContext* Context, float& OutScore) const;

public:
    virtual void BeginDestroy() override;
    /**
     * @brief Initializes the default states for the processor
     */
    UFUNCTION(BlueprintCallable, Category="AI Processor")
    virtual void InitDefaultStates();
    
    /**
     * @brief Retrieves the current array of states
     * @param OutStates Output array to store the current states
     */
    UFUNCTION(BlueprintCallable, Category="AI Processor")
    void GetCurrentStates(TArray<UUtilityAIState*>& OutStates) const;

    /**
     * @brief Gets the processor type
     * @return The processor type identifier
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="AI Processor")
    FORCEINLINE int32 GetProcessorType() const { return ProcessorType; }

    /**
     * @brief Adds a state to the processor
     * @param InNewState The state to add
     */
    UFUNCTION(BlueprintCallable, Category = "AI Processor")
    void AddState(UUtilityAIState* InNewState);

    /**
     * @brief Removes a state from the processor
     * @param InStateToRemove The state to remove
     */
    UFUNCTION(BlueprintCallable, Category = "AI Processor")
    void RemoveState(UUtilityAIState* InStateToRemove);

    /**
     * @brief Gets the best state based on the provided context
     * @param InContext The context to evaluate states against
     * @param OutScore Output parameter for the score of the best state
     * @return Pointer to the best state, or nullptr if none found
     */
    UFUNCTION(BlueprintCallable, Category = "AI Processor")
    UUtilityAIState* GetBestState(const UUtilityAIContext* InContext, float& OutScore) const;

    UFUNCTION(BlueprintCallable, Category="AI Processor")
    void GetScoreMap(const UUtilityAIContext* InContext, TMap<UUtilityAIState*, float>& OutScoreMap,
         TMap<UUtilityAIState*, float>& OutScoreMapNormalized);
    
    /**
     * @brief Gets a state by its type
     * @param InType The type identifier of the state
     * @return Pointer to the state, or nullptr if not found
     */
    UFUNCTION(BlueprintCallable, Category = "AI Processor")
    UUtilityAIState* GetStateByType(int32 InType) const;
};