// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GodArena_UE5 : ModuleRules
{
	public GodArena_UE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", 
			"InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem" ,"Niagara"});

        PrivateDependencyModuleNames.AddRange(new string[] { });

    }
}
