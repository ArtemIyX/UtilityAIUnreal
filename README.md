# Utility AI Plugin

A flexible Utility AI system for Unreal Engine that enables context-driven decision making for AI agents. 

The plugin provides a component-based architecture for evaluating and executing the most appropriate behaviors based on dynamic scoring.

## Setup Instructions

### 1. Add Submodule to Project
Add the plugin to your project's plugins directory:
> your-project/Plugins/UtilityAI/

### 2. Enable Plugin in uproject
Add the following entry to your `uproject` file in the `Plugins` array:
```json
{
    "Name": "UtilityAI",
    "Enabled": true
}
```

### 3. Recompile Project
Recompile your project to activate the plugin:

1. Right-click on your .uproject file
2. Select "Generate Visual Studio project files"
3. Open the solution and build your project

## Class Descriptions

### Context
```cpp
class UTILITYAI_API UUtilityAIContext : public UObject 
```
> Base context class that holds data used by states for scoring and execution. Inherit from this to create custom context data for your AI.

### ContextCollector
```cpp
class UTILITYAI_API UUtilityAIContextCollector : public UObject
```
> Responsible for gathering and populating context data before state evaluation. Implement FillContext() to provide relevant data to your AI states.

### State
```cpp
class UTILITYAI_API UUtilityAIState : public UObject
```
> Represents a single AI behavior or action. Key features:

- **GetScore()**: Calculates suitability score based on current context

- **Execute()**: Performs the state's behavior

- **StateType**: Unique identifier for the state

### Processor
```cpp
class UTILITYAI_API UUtilityAIProcessor : public UObject
```
> Manages a collection of states and determines the best one to execute. Features:

- **FindHighestScoringState()**: Evaluates all states and returns the highest scorer

- State management with **AddState()** and **RemoveState()**

- **ProcessorType**: Categorizes processors for different AI systems (e.g., movement, combat, dialogue)

### Component
```cpp
class UTILITYAI_API UUtilityAIComponent : public UActorComponent
```
> Actor component that orchestrates the Utility AI system. Main features:

- Manages context collection and updating

- Handles multiple processors for different AI aspects

- Provides template methods for easy processor access

## Usage in Game

1. Create Custom Context
```cpp
  UCLASS()
  class YOURGAME_API UYourAIContext : public UUtilityAIContext
  {
      GENERATED_BODY()
      
  public:
      UPROPERTY()
      float Health;
      
      UPROPERTY()
      float DistanceToTarget;
      
      // Add your custom context data
  };
```
2. Implement Context Collector
```cpp
UCLASS()
  class YOURGAME_API UYourContextCollector : public UUtilityAIContextCollector
  {
      GENERATED_BODY()
      
  public:
      virtual void FillContext(UUtilityAIComponent* InSystem, UUtilityAIContext* InContext) override
      {
          if (UYourAIContext* YourContext = Cast<UYourAIContext>(InContext))
          {
              // Populate context with current game data
              AActor* Owner = InSystem->GetOwner();
              // Your context collection logic here
          }
      }
  };
```
4. Create AI States
```cpp
UCLASS()
class YOURGAME_API UAttackState : public UUtilityAIState
{
    GENERATED_BODY()
    
public:
    UAttackState()
    {
        StateType = 1; // Unique state identifier
    }
    
    virtual float GetScore(const UUtilityAIContext* Context) const override
    {
        if (const UYourAIContext* YourContext = Cast<UYourAIContext>(Context))
        {
            // Calculate score based on context
            return YourContext->Health > 50.f ? 0.8f : 0.2f;
        }
        return 0.f;
    }
    
    virtual void Execute(AActor* Owner, UUtilityAIComponent* InSystem, const UUtilityAIContext* InContext) override
    {
        // Implement attack behavior
        UE_LOG(LogTemp, Warning, TEXT("Executing Attack State"));
    }
};
```
6. Set Up AI Component
```cpp
// In your character or controller class
UUtilityAIComponent* UtilityAI = CreateDefaultSubobject<UUtilityAIComponent>(TEXT("UtilityAI"));
UtilityAI->ContextClass = UYourAIContext::StaticClass();
UtilityAI->ContextCollectorClass = UYourContextCollector::StaticClass();

// Add processors (can also be done in Blueprints)
UUtilityAIProcessor* CombatProcessor = NewObject<UUtilityAIProcessor>(this);
CombatProcessor->ProcessorType = 1; // Combat processor
CombatProcessor->AddState(NewObject<UAttackState>(this));
UtilityAI->AddProcessor(CombatProcessor);
```
8. Update and Execute AI
```cpp
// In your tick function or timer
void AYourAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (UtilityAI)
    {
        UtilityAI->UpdateContext();
        UtilityAI->ExecuteBestForProcessorType(1); // Execute best combat state
    }
}
```

## Blueprint Usage
The plugin exposes most functionality to Blueprints, allowing designers to:

- Create and configure states visually
- Set up processors with different state collections
- Hook up context collectors for dynamic data gathering
- Trigger AI updates

## Example Use Cases
- NPC Behavior: Choose between idle, patrol, chase, or flee based on player proximity and NPC health
- Combat AI: Select attack patterns, defensive maneuvers, or healing based on combat context
- Dialogue System: Choose conversation responses based on character relationships and story state
- Resource Management: Decide which resources to gather based on current needs and availability

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
