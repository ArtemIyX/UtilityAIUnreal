// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAIState.h" // For templated GetStateExactClass<T>
#include "UObject/Object.h"
#include "UtilityAIProcessor.generated.h"

class UUtilityAIComponent;
class UUtilityAIContext;

/**
 * @brief Manages a collection of states and determines the best one to execute
 */
UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityAIProcessor : public UObject
{
	GENERATED_BODY()

public:
	/** @brief Default constructor for UUtilityAIProcessor */
	UUtilityAIProcessor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

protected:
	/** @brief List of active state instances configured for this processor */
	UPROPERTY(BlueprintReadOnly, Category = "AI Processor|Runtime")
	TArray<TObjectPtr<UUtilityAIState>> States;

	UPROPERTY(BlueprintReadOnly, Category = "AI Processor|Runtime")
	TWeakObjectPtr<UUtilityAIComponent> AIComponentPtr;

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

	virtual void Setup(UUtilityAIComponent* InOwnerComponent);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AI Processor|Get")
	FORCEINLINE UUtilityAIComponent* GetUtilityAIComponent() const { return AIComponentPtr.Get(); }

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

	/**
	 * @brief Retrieves a state of type T from the collection of states.
	 * 
	 * This templated function iterates through the States collection and returns a pointer to the first state
	 * whose class matches the specified type T. If no matching state is found, it returns nullptr.
	 * 
	 * @tparam T The type of the state to retrieve, expected to be a subclass of UUtilityAIState.
	 * @return T* A pointer to the state of type T, or nullptr if no matching state is found.
	 */
	template <typename T>
	T* GetStateExactClass() const
	{
		for (const TObjectPtr<UUtilityAIState>& el : States)
		{
			if (el->GetClass() == T::StaticClass())
			{
				return Cast<T>(el.Get());
			}
		}
		return nullptr;
	}

	/**
	 * @brief Retrieves a state of type T from the collection of states based on a specific state type identifier.
	 * 
	 * This templated function iterates through the States collection and returns a pointer to the first state
	 * whose state type matches the provided InType value. If no matching state is found, it returns nullptr.
	 * 
	 * @tparam T The type of the state to retrieve, expected to be a subclass of UUtilityAIState.
	 * @tparam E The type of the state identifier (e.g., an enum or integer type).
	 * @param InType The state type identifier to match against.
	 * @return T* A pointer to the state of type T with the matching state type, or nullptr if no matching state is found.
	 */
	template <typename T, typename E>
	T* GetState(E InType) const
	{
		for (const TObjectPtr<UUtilityAIState>& el : States)
		{
			if (el->GetStateType() == static_cast<int32>(InType))
			{
				return Cast<T>(el.Get());
			}
		}
		return nullptr;
	}
};