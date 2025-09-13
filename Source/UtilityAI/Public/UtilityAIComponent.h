// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityAIComponent.generated.h"

class UUtilityAIContextCollector;
class UUtilityAIProcessor;
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
    
    /** @brief Shared context filled on tick */
    UPROPERTY(BlueprintReadOnly, Category = "AI Component")
    TObjectPtr<UUtilityAIContext> Context;

    /** @brief Context collector instance */
    UPROPERTY(BlueprintReadOnly, Category = "AI Component")
    TObjectPtr<UUtilityAIContextCollector> ContextCollector;
    
    /** @brief List of active processor instances */
    UPROPERTY(BlueprintReadOnly, Category = "AI Component")
    TArray<TObjectPtr<UUtilityAIProcessor>> Processors;

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
     * @brief Gets a processor by its class type
     * @tparam T Type of the processor
     * @return Pointer to the processor of type T, or nullptr if not found
     */
    template <class T>
    T* GetProcessorByClass() const;

    /**
     * @brief Gets the current AI context
     * @return Pointer to the current context
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintAuthorityOnly, Category="AI Component")
    FORCEINLINE UUtilityAIContext* GetContext() const { return Context.Get(); }

    /**
     * @brief Adds a processor to the component
     * @param NewProcessor Processor to add
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
    void AddProcessor(UUtilityAIProcessor* NewProcessor);

    /**
     * @brief Removes a processor from the component
     * @param ProcessorToRemove Processor to remove
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
    void RemoveProcessor(UUtilityAIProcessor* ProcessorToRemove);

    /**
     * @brief Gets a processor by its type
     * @param Type Type identifier of the processor
     * @return Pointer to the processor, or nullptr if not found
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
    UUtilityAIProcessor* GetProcessorByType(int32 Type) const;

    /**
     * @brief Executes the best action for a specific processor type
     * @param Type Type identifier of the processor
     */
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI Component")
    void ExecuteBestForProcessorType(int32 Type);
};