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
				"UtilityAI",
				"Slate",
				"SlateCore",
				"CoreUObject",
				"Engine",
				"EditorStyle",
				"InputCore",
				"Projects",
				"UMG",
				"AssetTools",
				"AssetDefinition",
				"GameplayTags",
				"GameplayTagsEditor"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd",
				"PropertyEditor"
			}
		);
	}
}