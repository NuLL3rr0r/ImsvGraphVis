// Copyright 2017 Oh-Hyun Kwon. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "RHICommandList.h"
#include "UniformBuffer.h"

#include "Runtime/Launch/Resources/Version.h"

// DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(uint32, Degree)
BEGIN_UNIFORM_BUFFER_STRUCT(FSplineComputeShaderUniformParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, WorldSize)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Width)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(uint32, NumSides)
END_UNIFORM_BUFFER_STRUCT(FSplineComputeShaderUniformParameters)

typedef TUniformBufferRef<FSplineComputeShaderUniformParameters>
	FSplineComputeShaderUniformParametersRef;

class SPLINERENDERER_API FBaseSplineComputeShader : public FGlobalShader
{
private:
	FShaderResourceParameter InSplineControlPointData;
	FShaderResourceParameter InSplineSegmentData;
	FShaderResourceParameter InSplineData;

	FShaderResourceParameter OutMeshVertexData;

public:
	static bool ShouldCache(EShaderPlatform Platform)
	{
		return RHISupportsComputeShaders(Platform);
	}

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& PermutationParams,
#else
	static void ModifyCompilationEnvironment(EShaderPlatform Platform,
#endif
											 FShaderCompilerEnvironment& OutEnvironment)
	{
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19
		FGlobalShader::ModifyCompilationEnvironment(PermutationParams, OutEnvironment);
#else
		FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
#endif
		OutEnvironment.SetDefine(TEXT("SPLINE_COMPUTE_SHADER_MAX_SAMPLES_PER_SEGMENT"), 64);
	}

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 19
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& PermutationParams) {
		return true;
	}
#endif

	FBaseSplineComputeShader()
	{
	}

	explicit FBaseSplineComputeShader(
		ShaderMetaType::CompiledShaderInitializerType const& Initializer);

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
		Ar << InSplineControlPointData;
		Ar << InSplineSegmentData;
		Ar << InSplineData;
		Ar << OutMeshVertexData;
		return bShaderHasOutdatedParams;
	}

	void SetUniformBuffers(FRHICommandList& RHICmdList,
						   FSplineComputeShaderUniformParameters& ConstantParameters);

	void SetBuffers(FRHICommandList& RHICmdList,
					FShaderResourceViewRHIRef InSplineControlPointBufferSRV,
					FShaderResourceViewRHIRef InSplineSegmentBufferSRV,
					FShaderResourceViewRHIRef InSplineBufferSRV,
					FUnorderedAccessViewRHIRef OutMeshVertexBufferUAV);

	void UnbindBuffers(FRHICommandList& RHICmdList);
};

class SPLINERENDERER_API FSplineComputeShader_Sphere : public FBaseSplineComputeShader
{
	DECLARE_SHADER_TYPE(FSplineComputeShader_Sphere, Global);

public:
	FSplineComputeShader_Sphere()
	{
	}

	explicit FSplineComputeShader_Sphere(
		ShaderMetaType::CompiledShaderInitializerType const& Initializer);
};
