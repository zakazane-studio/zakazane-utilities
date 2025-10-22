﻿using UnrealBuildTool;

public class ZakazaneUtilitiesTests : ModuleRules
{
	public ZakazaneUtilitiesTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ZakazaneUtilities",
				"ZakazaneTestUtilities"
			}
		);
	}
}