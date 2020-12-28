// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CircularDependenciesDetector/Private/NamePair.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNamePair() {}
// Cross Module References
	CIRCULARDEPENDENCIESDETECTOR_API UScriptStruct* Z_Construct_UScriptStruct_FNamePair();
	UPackage* Z_Construct_UPackage__Script_CircularDependenciesDetector();
// End Cross Module References
class UScriptStruct* FNamePair::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern CIRCULARDEPENDENCIESDETECTOR_API uint32 Get_Z_Construct_UScriptStruct_FNamePair_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FNamePair, Z_Construct_UPackage__Script_CircularDependenciesDetector(), TEXT("NamePair"), sizeof(FNamePair), Get_Z_Construct_UScriptStruct_FNamePair_Hash());
	}
	return Singleton;
}
template<> CIRCULARDEPENDENCIESDETECTOR_API UScriptStruct* StaticStruct<FNamePair>()
{
	return FNamePair::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FNamePair(FNamePair::StaticStruct, TEXT("/Script/CircularDependenciesDetector"), TEXT("NamePair"), false, nullptr, nullptr);
static struct FScriptStruct_CircularDependenciesDetector_StaticRegisterNativesFNamePair
{
	FScriptStruct_CircularDependenciesDetector_StaticRegisterNativesFNamePair()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("NamePair")),new UScriptStruct::TCppStructOps<FNamePair>);
	}
} ScriptStruct_CircularDependenciesDetector_StaticRegisterNativesFNamePair;
	struct Z_Construct_UScriptStruct_FNamePair_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Name1_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_Name1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Name2_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_Name2;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FNamePair_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "ModuleRelativePath", "Private/NamePair.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FNamePair_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FNamePair>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name1_MetaData[] = {
		{ "Category", "Name Pair" },
		{ "ModuleRelativePath", "Private/NamePair.h" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name1 = { "Name1", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FNamePair, Name1), METADATA_PARAMS(Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name1_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name2_MetaData[] = {
		{ "Category", "Name Pair" },
		{ "ModuleRelativePath", "Private/NamePair.h" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name2 = { "Name2", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FNamePair, Name2), METADATA_PARAMS(Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name2_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name2_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FNamePair_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNamePair_Statics::NewProp_Name2,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FNamePair_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_CircularDependenciesDetector,
		nullptr,
		&NewStructOps,
		"NamePair",
		sizeof(FNamePair),
		alignof(FNamePair),
		Z_Construct_UScriptStruct_FNamePair_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNamePair_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FNamePair_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNamePair_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FNamePair()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FNamePair_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_CircularDependenciesDetector();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("NamePair"), sizeof(FNamePair), Get_Z_Construct_UScriptStruct_FNamePair_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FNamePair_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FNamePair_Hash() { return 2369755535U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
