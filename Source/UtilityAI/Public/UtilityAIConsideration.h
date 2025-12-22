// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "UtilityAIConsideration.generated.h"

class UUtilityAIContext;
class UUtilityAIComponent;

USTRUCT(Blueprintable, BlueprintType)
struct UTILITYAI_API FConsiderationParamValue
{
	GENERATED_BODY()

public:
	FConsiderationParamValue()
		: Value(0.0f) {}

	explicit FConsiderationParamValue(float InValue)
		: Value(InValue) {}

	explicit FConsiderationParamValue(const FConsiderationParamValue& Other)
	{
		Value = Other.Value;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

public:
	FConsiderationParamValue& operator=(float InValue)
	{
		Value = InValue;
		return *this;
	}

	FConsiderationParamValue& operator=(const FConsiderationParamValue& Other)
	{
		Value = Other.Value;
		return *this;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct UTILITYAI_API FConsiderationParamKey
{
	GENERATED_BODY()


public:
	FConsiderationParamKey()
		: Key() {}

	explicit FConsiderationParamKey(const FGameplayTag& InKey)
		: Key(InKey) {}

	explicit FConsiderationParamKey(const FConsiderationParamKey& Other)
	{
		Key = Other.Key;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Key;

	// Equality operator
	bool operator==(const FConsiderationParamKey& Other) const
	{
		return Key == Other.Key;
	}

public:
	FConsiderationParamKey& operator=(const FGameplayTag& InValue)
	{
		Key = InValue;
		return *this;
	}

	FConsiderationParamKey& operator=(const FConsiderationParamKey& Other)
	{
		Key = Other.Key;
		return *this;
	}
};

// Global hash function (outside the struct)
FORCEINLINE uint32 GetTypeHash(const FConsiderationParamKey& ParamKey)
{
	return GetTypeHash(ParamKey.Key.GetTagName());
}


/**
 * @brief Base class for all Utility AI Considerations.
 *
 * A Consideration represents a single factor that influences the desirability (score) of an AI action or decision.
 * Derived blueprint or C++ classes implement the scoring logic by overriding GetScore().
 *
 * Each consideration has a unique Id (FName) that can be used to look it up at runtime
 * (e.g. g. via UUtilityAISystem::GetConsiderationById()).
 *
 * Considerations are typically added to a UUtilityAIComponent or similar container and evaluated
 * together with other considerations to produce a final weighted action score in a Utility AI system.
 *
 * @see UUtilityAIComponent, UUtilityAIProcessor
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "Utility AI", meta = (ShortTooltip = "Base class for scoring factors in Utility AI"))
class UTILITYAI_API UUtilityAIConsideration : public UObject
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	UUtilityAIConsideration(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Consideration|Settings")
	TMap<FConsiderationParamKey, FConsiderationParamValue> DefaultParams;

	UPROPERTY(BlueprintReadOnly, Category = "AI Consideration|Runtime")
	TMap<FConsiderationParamKey, FConsiderationParamValue> RuntimeParams;

protected:

	bool AddParam(const FConsiderationParamKey& InKey, const FConsiderationParamValue& InValue);

public:
	/**
	 * @brief Unique identifier for this consideration.
	 *
	 * Used for runtime lookups and debugging. Should be unique within the owning AI component/system.
	 * Visible and editable in blueprints and the editor (Defaults only).
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Consideration|Settings")
	FGameplayTag Id;

public:
	/**
	 * @brief Calculates and returns the raw score for this consideration.
	 *
	 * This function is called every time the owning Utility AI system evaluates actions.
	 * The returned value is usually in the range [0.0, 1.0], but any float is valid
	 * (higher values = more desirable). The score will later be modified by response curves,
	 * weights, etc., in the parent system.
	 *
	 *
	 * @param Owner       The actor that owns the Utility AI component (usually the AI controller or pawn).
	 * @param InSystem    The Utility AI component that is performing the evaluation.
	 * @param InContext   Optional read-only context object containing blackboard data, targets, etc.
	 *
	 * @return The raw consideration score as a float.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "AI Consideration",
		meta = (Tooltip = "Returns how desirable this consideration currently is (typically 0..1)"))
	float GetScore(AActor* Owner, const UUtilityAIComponent* InSystem, const UUtilityAIContext* InContext) const;

	/**
	 * @brief Returns the unique identifier of this consideration.
	 * @return The Id set in the details panel or defaults.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI Consideration")
	FORCEINLINE FGameplayTag GetId() const { return Id; }

	#pragma region RuntimeParams

	void InitDefaultParams();

	UFUNCTION(BlueprintCallable, Category = "AI Consideration|Runtime")
	bool SetParam(FGameplayTag InKey, const FConsiderationParamValue& InValue);

	UFUNCTION(BlueprintCallable, Category = "AI Consideration|Runtime")
	bool TryGetParamValue(FGameplayTag InKey, FConsiderationParamValue& OutValue);

	UFUNCTION(BlueprintCallable, Category = "AI Consideration|Runtime")
	bool ContainsParam(FGameplayTag InKey) const;

	#pragma endregion
};