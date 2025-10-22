// Copyright ZAKAZANE Studio. All Rights Reserved.

using UnrealBuildTool;

public class ZakazaneUtilitiesEditor : ModuleRules
{
	public ZakazaneUtilitiesEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bWarningsAsErrors = true;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"AssetRegistry"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"PropertyEditor",
				"ZakazaneUtilities"
			}
		);
	}
}