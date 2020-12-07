// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CircularDependenciesDetector/Private/CircularDependenciesLib.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCircularDependenciesLib() {}
// Cross Module References
	CIRCULARDEPENDENCIESDETECTOR_API UClass* Z_Construct_UClass_UCircularDependenciesLib_NoRegister();
	CIRCULARDEPENDENCIESDETECTOR_API UClass* Z_Construct_UClass_UCircularDependenciesLib();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	UPackage* Z_Construct_UPackage__Script_CircularDependenciesDetector();
	UMG_API UClass* Z_Construct_UClass_UListView_NoRegister();
	CIRCULARDEPENDENCIESDETECTOR_API UScriptStruct* Z_Construct_UScriptStruct_FNamePair();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
// End Cross Module References
	DEFINE_FUNCTION(UCircularDependenciesLib::execAddToDependencyStack)
	{
		P_GET_PROPERTY(FNameProperty,Z_Param_CurrentAsset);
		P_GET_TARRAY_REF(FName,Z_Param_Out_DependencyStack);
		P_GET_TSET_REF(FString,Z_Param_Out_ExcludedAssetSet);
		P_GET_TSET_REF(FNamePair,Z_Param_Out_BrokenDependecySet);
		P_GET_OBJECT(UListView,Z_Param_InvolvedAssetListView);
		P_FINISH;
		P_NATIVE_BEGIN;
		UCircularDependenciesLib::AddToDependencyStack(Z_Param_CurrentAsset,Z_Param_Out_DependencyStack,Z_Param_Out_ExcludedAssetSet,Z_Param_Out_BrokenDependecySet,Z_Param_InvolvedAssetListView);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UCircularDependenciesLib::execGetAllFunctions)
	{
		P_GET_OBJECT(UObject,Z_Param_Asset);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<FString>*)Z_Param__Result=UCircularDependenciesLib::GetAllFunctions(Z_Param_Asset);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UCircularDependenciesLib::execGetClassFromAsset)
	{
		P_GET_OBJECT(UObject,Z_Param_Asset);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UClass**)Z_Param__Result=UCircularDependenciesLib::GetClassFromAsset(Z_Param_Asset);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UCircularDependenciesLib::execSearchInBlueprint)
	{
		P_GET_OBJECT(UObject,Z_Param_Asset);
		P_GET_UBOOL(Z_Param_bAllBlueprints);
		P_GET_PROPERTY(FStrProperty,Z_Param_NewSearchTerms);
		P_FINISH;
		P_NATIVE_BEGIN;
		UCircularDependenciesLib::SearchInBlueprint(Z_Param_Asset,Z_Param_bAllBlueprints,Z_Param_NewSearchTerms);
		P_NATIVE_END;
	}
	void UCircularDependenciesLib::StaticRegisterNativesUCircularDependenciesLib()
	{
		UClass* Class = UCircularDependenciesLib::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "AddToDependencyStack", &UCircularDependenciesLib::execAddToDependencyStack },
			{ "GetAllFunctions", &UCircularDependenciesLib::execGetAllFunctions },
			{ "GetClassFromAsset", &UCircularDependenciesLib::execGetClassFromAsset },
			{ "SearchInBlueprint", &UCircularDependenciesLib::execSearchInBlueprint },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics
	{
		struct CircularDependenciesLib_eventAddToDependencyStack_Parms
		{
			FName CurrentAsset;
			TArray<FName> DependencyStack;
			TSet<FString> ExcludedAssetSet;
			TSet<FNamePair> BrokenDependecySet;
			UListView* InvolvedAssetListView;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InvolvedAssetListView_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InvolvedAssetListView;
		static const UE4CodeGen_Private::FSetPropertyParams NewProp_BrokenDependecySet;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_BrokenDependecySet_ElementProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ExcludedAssetSet_MetaData[];
#endif
		static const UE4CodeGen_Private::FSetPropertyParams NewProp_ExcludedAssetSet;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ExcludedAssetSet_ElementProp;
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_DependencyStack;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_DependencyStack_Inner;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_CurrentAsset;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_InvolvedAssetListView_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_InvolvedAssetListView = { "InvolvedAssetListView", nullptr, (EPropertyFlags)0x0010000000080080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventAddToDependencyStack_Parms, InvolvedAssetListView), Z_Construct_UClass_UListView_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_InvolvedAssetListView_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_InvolvedAssetListView_MetaData)) };
	const UE4CodeGen_Private::FSetPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_BrokenDependecySet = { "BrokenDependecySet", nullptr, (EPropertyFlags)0x0010000008000180, UE4CodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventAddToDependencyStack_Parms, BrokenDependecySet), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_BrokenDependecySet_ElementProp = { "BrokenDependecySet", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FNamePair, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FSetPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet = { "ExcludedAssetSet", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventAddToDependencyStack_Parms, ExcludedAssetSet), METADATA_PARAMS(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet_MetaData)) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet_ElementProp = { "ExcludedAssetSet", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_DependencyStack = { "DependencyStack", nullptr, (EPropertyFlags)0x0010000008000180, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventAddToDependencyStack_Parms, DependencyStack), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_DependencyStack_Inner = { "DependencyStack", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_CurrentAsset = { "CurrentAsset", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventAddToDependencyStack_Parms, CurrentAsset), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_InvolvedAssetListView,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_BrokenDependecySet,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_BrokenDependecySet_ElementProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_ExcludedAssetSet_ElementProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_DependencyStack,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_DependencyStack_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::NewProp_CurrentAsset,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::Function_MetaDataParams[] = {
		{ "Category", "Circular Dependencies EWBP Lib" },
		{ "ModuleRelativePath", "Private/CircularDependenciesLib.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCircularDependenciesLib, nullptr, "AddToDependencyStack", nullptr, nullptr, sizeof(CircularDependenciesLib_eventAddToDependencyStack_Parms), Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics
	{
		struct CircularDependenciesLib_eventGetAllFunctions_Parms
		{
			UObject* Asset;
			TArray<FString> ReturnValue;
		};
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ReturnValue_Inner;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Asset;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventGetAllFunctions_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::NewProp_Asset = { "Asset", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventGetAllFunctions_Parms, Asset), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::NewProp_ReturnValue_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::NewProp_Asset,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::Function_MetaDataParams[] = {
		{ "Category", "Circular Dependencies Lib" },
		{ "Keywords", "method class" },
		{ "ModuleRelativePath", "Private/CircularDependenciesLib.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCircularDependenciesLib, nullptr, "GetAllFunctions", nullptr, nullptr, sizeof(CircularDependenciesLib_eventGetAllFunctions_Parms), Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics
	{
		struct CircularDependenciesLib_eventGetClassFromAsset_Parms
		{
			UObject* Asset;
			UClass* ReturnValue;
		};
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Asset;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventGetClassFromAsset_Parms, ReturnValue), Z_Construct_UClass_UObject_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::NewProp_Asset = { "Asset", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventGetClassFromAsset_Parms, Asset), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::NewProp_Asset,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::Function_MetaDataParams[] = {
		{ "Category", "Circular Dependencies Lib" },
		{ "ModuleRelativePath", "Private/CircularDependenciesLib.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCircularDependenciesLib, nullptr, "GetClassFromAsset", nullptr, nullptr, sizeof(CircularDependenciesLib_eventGetClassFromAsset_Parms), Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics
	{
		struct CircularDependenciesLib_eventSearchInBlueprint_Parms
		{
			UObject* Asset;
			bool bAllBlueprints;
			FString NewSearchTerms;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_NewSearchTerms;
		static void NewProp_bAllBlueprints_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAllBlueprints;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Asset;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_NewSearchTerms = { "NewSearchTerms", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventSearchInBlueprint_Parms, NewSearchTerms), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_bAllBlueprints_SetBit(void* Obj)
	{
		((CircularDependenciesLib_eventSearchInBlueprint_Parms*)Obj)->bAllBlueprints = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_bAllBlueprints = { "bAllBlueprints", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(CircularDependenciesLib_eventSearchInBlueprint_Parms), &Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_bAllBlueprints_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_Asset = { "Asset", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CircularDependenciesLib_eventSearchInBlueprint_Parms, Asset), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_NewSearchTerms,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_bAllBlueprints,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::NewProp_Asset,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::Function_MetaDataParams[] = {
		{ "Category", "Circular Dependencies Lib" },
		{ "Keywords", "find editor" },
		{ "ModuleRelativePath", "Private/CircularDependenciesLib.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCircularDependenciesLib, nullptr, "SearchInBlueprint", nullptr, nullptr, sizeof(CircularDependenciesLib_eventSearchInBlueprint_Parms), Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UCircularDependenciesLib_NoRegister()
	{
		return UCircularDependenciesLib::StaticClass();
	}
	struct Z_Construct_UClass_UCircularDependenciesLib_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCircularDependenciesLib_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_CircularDependenciesDetector,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UCircularDependenciesLib_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UCircularDependenciesLib_AddToDependencyStack, "AddToDependencyStack" }, // 1736563032
		{ &Z_Construct_UFunction_UCircularDependenciesLib_GetAllFunctions, "GetAllFunctions" }, // 1383064194
		{ &Z_Construct_UFunction_UCircularDependenciesLib_GetClassFromAsset, "GetClassFromAsset" }, // 1296630271
		{ &Z_Construct_UFunction_UCircularDependenciesLib_SearchInBlueprint, "SearchInBlueprint" }, // 3001568290
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCircularDependenciesLib_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "CircularDependenciesLib.h" },
		{ "ModuleRelativePath", "Private/CircularDependenciesLib.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCircularDependenciesLib_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCircularDependenciesLib>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UCircularDependenciesLib_Statics::ClassParams = {
		&UCircularDependenciesLib::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCircularDependenciesLib_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCircularDependenciesLib_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCircularDependenciesLib()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UCircularDependenciesLib_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UCircularDependenciesLib, 2930954345);
	template<> CIRCULARDEPENDENCIESDETECTOR_API UClass* StaticClass<UCircularDependenciesLib>()
	{
		return UCircularDependenciesLib::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UCircularDependenciesLib(Z_Construct_UClass_UCircularDependenciesLib, &UCircularDependenciesLib::StaticClass, TEXT("/Script/CircularDependenciesDetector"), TEXT("UCircularDependenciesLib"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCircularDependenciesLib);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
