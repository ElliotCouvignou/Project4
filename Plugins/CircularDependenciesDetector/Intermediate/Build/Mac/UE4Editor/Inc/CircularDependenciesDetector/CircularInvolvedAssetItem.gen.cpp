// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CircularDependenciesDetector/Private/CircularInvolvedAssetItem.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCircularInvolvedAssetItem() {}
// Cross Module References
	CIRCULARDEPENDENCIESDETECTOR_API UClass* Z_Construct_UClass_UCircularInvolvedAssetItem_NoRegister();
	CIRCULARDEPENDENCIESDETECTOR_API UClass* Z_Construct_UClass_UCircularInvolvedAssetItem();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_CircularDependenciesDetector();
// End Cross Module References
	void UCircularInvolvedAssetItem::StaticRegisterNativesUCircularInvolvedAssetItem()
	{
	}
	UClass* Z_Construct_UClass_UCircularInvolvedAssetItem_NoRegister()
	{
		return UCircularInvolvedAssetItem::StaticClass();
	}
	struct Z_Construct_UClass_UCircularInvolvedAssetItem_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DependencyStack_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_DependencyStack;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_DependencyStack_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetName_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_AssetName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_CircularDependenciesDetector,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "CircularInvolvedAssetItem.h" },
		{ "ModuleRelativePath", "Private/CircularInvolvedAssetItem.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack_MetaData[] = {
		{ "Category", "Circular Involved Asset Item" },
		{ "ModuleRelativePath", "Private/CircularInvolvedAssetItem.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack = { "DependencyStack", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCircularInvolvedAssetItem, DependencyStack), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack_Inner = { "DependencyStack", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_AssetName_MetaData[] = {
		{ "Category", "Circular Involved Asset Item" },
		{ "ModuleRelativePath", "Private/CircularInvolvedAssetItem.h" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_AssetName = { "AssetName", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCircularInvolvedAssetItem, AssetName), METADATA_PARAMS(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_AssetName_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_AssetName_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_DependencyStack_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::NewProp_AssetName,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCircularInvolvedAssetItem>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::ClassParams = {
		&UCircularInvolvedAssetItem::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::PropPointers),
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCircularInvolvedAssetItem()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UCircularInvolvedAssetItem_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UCircularInvolvedAssetItem, 2705959049);
	template<> CIRCULARDEPENDENCIESDETECTOR_API UClass* StaticClass<UCircularInvolvedAssetItem>()
	{
		return UCircularInvolvedAssetItem::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UCircularInvolvedAssetItem(Z_Construct_UClass_UCircularInvolvedAssetItem, &UCircularInvolvedAssetItem::StaticClass, TEXT("/Script/CircularDependenciesDetector"), TEXT("UCircularInvolvedAssetItem"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCircularInvolvedAssetItem);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
