// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityAIConsideration.h"
#include "UtilityAIProcessor.h"
#include "UtilityAIComponent.generated.h"


class UUtilityAIContextCollector;
class UUtilityAIContext;

/**
 * @brief Actor component that orchestrates the Utility AI system
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** @brief Default constructor for UUtilityAIComponent */
	UUtilityAIComponent();

protected:
	/** @brief Class type for the AI context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NoClear, Category = "AI Component")
	TSubclassOf<UUtilityAIContext> ContextClass;

	/** @brief Class type for the context collector */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NoClear, Category = "AI Component")
	TSubclassOf<UUtilityAIContextCollector> ContextCollectorClass;

	/** @brief Array of default processor classes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Component")
	TArray<TSubclassOf<UUtilityAIProcessor>> DefaultProcessors;

	/** @brief Array of default Consideration classes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Component")
	TArray<TSubclassOf<UUtilityAIConsideration>> DefaultConsiderations;

	/** @brief Shared context filled on tick */
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TObjectPtr<UUtilityAIContext> Context;

	/** @brief Context collector instance */
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TObjectPtr<UUtilityAIContextCollector> ContextCollector;

	/** @brief List of active processor instances */
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TArray<TObjectPtr<UUtilityAIProcessor>> Processors;

	/** @brief List of active consideration instances */
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TArray<TObjectPtr<UUtilityAIConsideration>> Considerations;

	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UUtilityAIConsideration>> ConsiderationsMap;

protected:
	/**
	 * @brief Updates the context with actor data
	 * @note Can be overridden or extended in derived classes
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
	virtual void UpdateContext();

protected:
	/**
	 * @brief Called when the game starts
	 * @note Initializes the component
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Called when the component is destroyed or the game ends
	 * @param EndPlayReason Reason for ending play
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/**
	 * @brief Called every frame to update the component
	 * @param DeltaTime Time since last frame
	 * @param TickType Type of tick
	 * @param ThisTickFunction Tick function for this component
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Retrieves the first processor in the list that is of the specified type T (or derived from it).
	 *
	 * Iterates through all processors stored in the Processors array and checks each one using Unreal's
	 * reflection system. If a processor matches the requested class (via IsA(T::StaticClass())), it is
	 * safely cast to T* and returned.
	 *
	 * @tparam T The exact processor class type to search for. Must be a subclass of UUtilityAIProcessor
	 *           (or the class itself) and must expose a valid StaticClass() method.
	 *
	 * @return Pointer to the found processor cast to T*, or nullptr if no processor of type T
	 *         (or a derived type) is present in the list.
	 *
	 * @note This is a linear O(n) search that returns the **first** matching processor only.
	 *       If multiple processors of the same type can exist, only the first one encountered will be returned.
	 *
	 * @warning Relies on Unreal Engine's UObject reflection system. T must be a UObject-derived class
	 *          registered with the engine's reflection system.
	 *
	 * Example usage:
	 * @code
	 * UMyCustomProcessor* CustomProc = GetProcessorByClass<UMyCustomProcessor>();
	 * if (CustomProc)
	 * {
	 *     CustomProc->DoSomething();
	 * }
	 * @endcode
	 *
	 * @see UUtilityAIProcessor, Cast<T>()
	 */
	template <class T>
	T* GetProcessorByClass() const
	{
		for (const TObjectPtr<UUtilityAIProcessor>& el : Processors)
		{
			if (el && el->IsA(T::StaticClass()))
			{
				return Cast<T>(el.Get());
			}
		}
		return nullptr;
	}

	/**
	 * @brief Retrieves the first consideration in the list that is of the specified type T (or derived from it).
	 *
	 * Iterates through all considerations stored in the Considerations array and performs a dynamic cast
	 * check using RTTI. If a consideration matches the requested class (checked via IsA(T::StaticClass())),
	 * it is cast to T* and returned.
	 *
	 * @tparam T The exact consideration class type to look for. Must be a subclass of UUtilityAIConsideration
	 *           (or the class itself) and must have a valid StaticClass() method.
	 *
	 * @return Pointer to the found consideration cast to T*, or nullptr if no consideration of type T
	 *         (or a derived type) exists in the list.
	 *
	 * @note This function performs a linear search (O(n)) and returns the first matching element only.
	 *       If multiple considerations of the same type can exist, only the first one encountered is returned.
	 *
	 * @warning The function relies on Unreal Engine's reflection system (UClass::IsA). T must be a UObject-derived
	 *          class known to the reflection system.
	 *
	 * Example usage:
	 * @code
	 * AMyAttackConsideration* AttackConsideration = GetConsiderationByClass<AMyAttackConsideration>();
	 * if (AttackConsideration)
	 * {
	 *     // Use the consideration safely
	 * }
	 * @endcode
	 */
	template <class T>
	T* GetConsiderationByClass() const
	{
		for (const TObjectPtr<UUtilityAIConsideration>& el : Considerations)
		{
			if (el && el->IsA(T::StaticClass()))
			{
				return Cast<T>(el.Get());
			}
		}
		return nullptr;
	}

	template <class T>
	float GetConsiderationScore() const
	{
		for (const TObjectPtr<UUtilityAIConsideration>& el : Considerations)
		{
			if (el && el->IsA(T::StaticClass()))
			{
				const UUtilityAIComponent* thisPtr = this;
				return el->GetScore(GetOwner(), thisPtr, GetContext());
			}
		}
		return 0.0f;
	}

	float GetConsiderationScoreById(const FName& InId) const;

	/**
	 * @brief Gets the current AI context
	 * @return Pointer to the current context
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category="AI Component")
	FORCEINLINE UUtilityAIContext* GetContext() const { return Context.Get(); }

	/**
	 * @brief Adds a processor to the component
	 * @param InNewProcessor Processor to add
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Processors")
	void AddProcessor(UUtilityAIProcessor* InNewProcessor);

	/**
	 * @brief Removes a processor from the component
	 * @param InProcessorToRemove Processor to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Processors")
	void RemoveProcessor(UUtilityAIProcessor* InProcessorToRemove);

	/**
	 * @brief Gets a processor by its type
	 * @param Type Type identifier of the processor
	 * @return Pointer to the processor, or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Processors")
	UUtilityAIProcessor* GetProcessorByType(int32 Type) const;

	/*/**
	 * @brief Executes the best action for a specific processor type
	 * @param Type Type identifier of the processor
	 #1#
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Processors")
	void ExecuteBestForProcessorType(int32 Type);*/

	/**
	 * @brief Retrieves a consideration from the list by its unique ID (FName).
	 *
	 * Performs a linear search through the Considerations array and returns the first consideration
	 * whose GetId() matches the provided InId.
	 *
	 * @param InId The unique identifier (FName) of the consideration to find.
	 *
	 * @return Pointer to the UUtilityAIConsideration with the matching ID, or nullptr if no
	 *         consideration with that ID exists in the list.
	 *
	 * @note This is an O(1) lookup.
	 * @note Comparison is performed with FName's fast, case-insensitive, interned string logic.
	 * @note Returns the raw UUtilityAIConsideration pointer (not cast to a derived type).
	 *
	 * Example usage:
	 * @code
	 * UUtilityAIConsideration* HealthConsideration = GetConsiderationById(FName("LowHealth"));
	 * if (HealthConsideration)
	 * {
	 *     float Score = HealthConsideration->Score(Context);
	 * }
	 * @endcode
	 *
	 * @see UUtilityAIConsideration::GetId()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category = "AI Component|Consideration")
	UUtilityAIConsideration* GetConsiderationById(const FName& InId) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Consideration")
	void AddConsideration(UUtilityAIConsideration* InNewConsideration);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Consideration")
	void RemoveConsideration(UUtilityAIConsideration* InConsiderationToRemove);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component|Consideration")
	void RemoveConsiderationById(const FName& InId);


};