// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MyProject/HorrorGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeHorrorGameMode() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
ENGINE_API UClass* Z_Construct_UClass_USoundBase_NoRegister();
MYPROJECT_API UClass* Z_Construct_UClass_AHorrorGameMode();
MYPROJECT_API UClass* Z_Construct_UClass_AHorrorGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_MyProject();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AHorrorGameMode Function OnPlayerDeathVisuals ****************************
static FName NAME_AHorrorGameMode_OnPlayerDeathVisuals = FName(TEXT("OnPlayerDeathVisuals"));
void AHorrorGameMode::OnPlayerDeathVisuals()
{
	UFunction* Func = FindFunctionChecked(NAME_AHorrorGameMode_OnPlayerDeathVisuals);
	ProcessEvent(Func,NULL);
}
struct Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Game Rules" },
		{ "ModuleRelativePath", "HorrorGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function OnPlayerDeathVisuals constinit property declarations ******************
// ********** End Function OnPlayerDeathVisuals constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AHorrorGameMode, nullptr, "OnPlayerDeathVisuals", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals_Statics::Function_MetaDataParams), Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class AHorrorGameMode Function OnPlayerDeathVisuals ******************************

// ********** Begin Class AHorrorGameMode Function PlayerDied **************************************
struct Z_Construct_UFunction_AHorrorGameMode_PlayerDied_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Game Rules" },
		{ "ModuleRelativePath", "HorrorGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function PlayerDied constinit property declarations ****************************
// ********** End Function PlayerDied constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AHorrorGameMode_PlayerDied_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AHorrorGameMode, nullptr, "PlayerDied", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AHorrorGameMode_PlayerDied_Statics::Function_MetaDataParams), Z_Construct_UFunction_AHorrorGameMode_PlayerDied_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AHorrorGameMode_PlayerDied()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AHorrorGameMode_PlayerDied_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AHorrorGameMode::execPlayerDied)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PlayerDied();
	P_NATIVE_END;
}
// ********** End Class AHorrorGameMode Function PlayerDied ****************************************

// ********** Begin Class AHorrorGameMode **********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AHorrorGameMode;
UClass* AHorrorGameMode::GetPrivateStaticClass()
{
	using TClass = AHorrorGameMode;
	if (!Z_Registration_Info_UClass_AHorrorGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("HorrorGameMode"),
			Z_Registration_Info_UClass_AHorrorGameMode.InnerSingleton,
			StaticRegisterNativesAHorrorGameMode,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AHorrorGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_AHorrorGameMode_NoRegister()
{
	return AHorrorGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AHorrorGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "HorrorGameMode.h" },
		{ "ModuleRelativePath", "HorrorGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DeathSound_MetaData[] = {
		{ "Category", "Death" },
		{ "ModuleRelativePath", "HorrorGameMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RestartDelay_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Game Rules" },
		{ "ModuleRelativePath", "HorrorGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AHorrorGameMode constinit property declarations **************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DeathSound;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RestartDelay;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AHorrorGameMode constinit property declarations ****************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("PlayerDied"), .Pointer = &AHorrorGameMode::execPlayerDied },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AHorrorGameMode_OnPlayerDeathVisuals, "OnPlayerDeathVisuals" }, // 634089585
		{ &Z_Construct_UFunction_AHorrorGameMode_PlayerDied, "PlayerDied" }, // 4217446612
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AHorrorGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AHorrorGameMode_Statics

// ********** Begin Class AHorrorGameMode Property Definitions *************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AHorrorGameMode_Statics::NewProp_DeathSound = { "DeathSound", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHorrorGameMode, DeathSound), Z_Construct_UClass_USoundBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DeathSound_MetaData), NewProp_DeathSound_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AHorrorGameMode_Statics::NewProp_RestartDelay = { "RestartDelay", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHorrorGameMode, RestartDelay), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RestartDelay_MetaData), NewProp_RestartDelay_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AHorrorGameMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHorrorGameMode_Statics::NewProp_DeathSound,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHorrorGameMode_Statics::NewProp_RestartDelay,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHorrorGameMode_Statics::PropPointers) < 2048);
// ********** End Class AHorrorGameMode Property Definitions ***************************************
UObject* (*const Z_Construct_UClass_AHorrorGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_MyProject,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHorrorGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AHorrorGameMode_Statics::ClassParams = {
	&AHorrorGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AHorrorGameMode_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AHorrorGameMode_Statics::PropPointers),
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHorrorGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AHorrorGameMode_Statics::Class_MetaDataParams)
};
void AHorrorGameMode::StaticRegisterNativesAHorrorGameMode()
{
	UClass* Class = AHorrorGameMode::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_AHorrorGameMode_Statics::Funcs));
}
UClass* Z_Construct_UClass_AHorrorGameMode()
{
	if (!Z_Registration_Info_UClass_AHorrorGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AHorrorGameMode.OuterSingleton, Z_Construct_UClass_AHorrorGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AHorrorGameMode.OuterSingleton;
}
AHorrorGameMode::AHorrorGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AHorrorGameMode);
AHorrorGameMode::~AHorrorGameMode() {}
// ********** End Class AHorrorGameMode ************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorGameMode_h__Script_MyProject_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AHorrorGameMode, AHorrorGameMode::StaticClass, TEXT("AHorrorGameMode"), &Z_Registration_Info_UClass_AHorrorGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AHorrorGameMode), 333331252U) },
	};
}; // Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorGameMode_h__Script_MyProject_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorGameMode_h__Script_MyProject_3296917985{
	TEXT("/Script/MyProject"),
	Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorGameMode_h__Script_MyProject_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorGameMode_h__Script_MyProject_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
