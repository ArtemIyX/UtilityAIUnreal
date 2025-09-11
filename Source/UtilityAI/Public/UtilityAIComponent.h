// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityAIComponent.generated.h"


class UUtilityAIContextCollector;
class UUtilityAIProcessor;
class UUtilityAIContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUtilityAIComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NoClear, Category = "AI Component")
	TSubclassOf<UUtilityAIContext> ContextClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NoClear, Category = "AI Component")
	TSubclassOf<UUtilityAIContextCollector> ContextCollectorClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TArray<TSubclassOf<UUtilityAIProcessor>> DefaultProcessors;
	
	// The shared context - filled on tick
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TObjectPtr<UUtilityAIContext> Context;

	// Context collector
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TObjectPtr<UUtilityAIContextCollector> ContextCollector;
	
	// List of processors
	UPROPERTY(BlueprintReadOnly, Category = "AI Component")
	TArray<TObjectPtr<UUtilityAIProcessor>> Processors;

protected:
	// Fill context with actor data (override or extend)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
	virtual void UpdateContext();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Get processor by class (using template or specify class)
	template <class T>
	T* GetProcessorByClass() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category="AI Component")
	FORCEINLINE UUtilityAIContext* GetContext() const { return Context.Get(); }

	// Add a processor
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
	void AddProcessor(UUtilityAIProcessor* NewProcessor);

	// Remove a processor by pointer
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
	void RemoveProcessor(UUtilityAIProcessor* ProcessorToRemove);

	// Get processor by type
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
	UUtilityAIProcessor* GetProcessorByType(int32 Type) const;

	// Execute best action for a specific processor type
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
	void ExecuteBestForProcessorType(int32 Type);
};
