namespace UnrealBuildTool.Rules
{
    public class SplineRenderer : ModuleRules
    {
        public SplineRenderer(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicDependencyModuleNames.AddRange(new string[] {
                "Core",
    			"CoreUObject",
                "Engine",
                "RenderCore",
                "ShaderCore",
                "RHI"
            });
        }
    }
}
