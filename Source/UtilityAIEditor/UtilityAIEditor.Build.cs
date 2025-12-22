using UnrealBuildTool;

public class UtilityAIEditor : ModuleRules
{
    public UtilityAIEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UtilityAI",
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}