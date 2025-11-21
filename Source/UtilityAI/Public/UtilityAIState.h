// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityAIStateRuntimeData.h"
#include "UtilityAIState.generated.h"


class UUtilityAIProcessor;
class UUtilityAIComponent;
class UUtilityAIContext;

/**
 * @class UUtilityAIState
 * @brief Represents a single AI behavior or action
 */
UCLASS(Blueprintable, Blueprintable)
class UTILITYAI_API UUtilityAIState : public UObject
{
	GENERATED_BODY()

public:
	UUtilityAIState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/**
	 * @brief Unique identifier for the state.
	 * @note Must be set in C++ Constructor
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="false"), Category = "AI State")
	int32 StateType;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "AI State|Runtime")
	FUtilityAIStateRuntimeData RuntimeData;

public:
	void Setup(const FUtilityAIStateRuntimeData& InParams);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI State|GET")
	FORCEINLINE UUtilityAIProcessor* GetProcessor() const { return RuntimeData.ProcessorPtr.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI State|GET")
	FORCEINLINE UUtilityAIComponent* GetUtilityAIComponent() const { return RuntimeData.AIComponentPtr.Get(); }

	/**
	 * @brief Calculates suitability score based on current context
	 * @param Context Current data context
	 * @return Evaluated Score [0.0 - 1.0]
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI State")
	float GetScore(const UUtilityAIContext* Context) const;


	/*/**
	 * @brief Performs the state's behavior
	 * @param Owner Utility AI Owner (AI Controller)
	 * @param InSystem Utility AI Actor Component (suboject of AI Controller)
	 * @param InContext Current data context
	 #1#
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AI State")
	void Execute(AActor* Owner, UUtilityAIComponent* InSystem, const UUtilityAIContext* InContext);*/

	/**
	 * Retrieves unique identifier for the state
	 * @return Unique identifier for the state
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI State")
	FORCEINLINE int32 GetStateType() const { return StateType; }
};