// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MyProject/HorrorPlayerController.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeHorrorPlayerController() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
MYPROJECT_API UClass* Z_Construct_UClass_AHorrorPlayerController();
MYPROJECT_API UClass* Z_Construct_UClass_AHorrorPlayerController_NoRegister();
UPackage* Z_Construct_UPackage__Script_MyProject();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AHorrorPlayerController **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AHorrorPlayerController;
UClass* AHorrorPlayerController::GetPrivateStaticClass()
{
	using TClass = AHorrorPlayerController;
	if (!Z_Registration_Info_UClass_AHorrorPlayerController.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("HorrorPlayerController"),
			Z_Registration_Info_UClass_AHorrorPlayerController.InnerSingleton,
			StaticRegisterNativesAHorrorPlayerController,
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
	return Z_Registration_Info_UClass_AHorrorPlayerController.InnerSingleton;
}
UClass* Z_Construct_UClass_AHorrorPlayerController_NoRegister()
{
	return AHorrorPlayerController::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AHorrorPlayerController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "HorrorPlayerController.h" },
		{ "ModuleRelativePath", "HorrorPlayerController.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AHorrorPlayerController constinit property declarations ******************
// ********** End Class AHorrorPlayerController constinit property declarations ********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AHorrorPlayerController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AHorrorPlayerController_Statics
UObject* (*const Z_Construct_UClass_AHorrorPlayerController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_MyProject,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHorrorPlayerController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AHorrorPlayerController_Statics::ClassParams = {
	&AHorrorPlayerController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHorrorPlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_AHorrorPlayerController_Statics::Class_MetaDataParams)
};
void AHorrorPlayerController::StaticRegisterNativesAHorrorPlayerController()
{
}
UClass* Z_Construct_UClass_AHorrorPlayerController()
{
	if (!Z_Registration_Info_UClass_AHorrorPlayerController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AHorrorPlayerController.OuterSingleton, Z_Construct_UClass_AHorrorPlayerController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AHorrorPlayerController.OuterSingleton;
}
AHorrorPlayerController::AHorrorPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AHorrorPlayerController);
AHorrorPlayerController::~AHorrorPlayerController() {}
// ********** End Class AHorrorPlayerController ****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorPlayerController_h__Script_MyProject_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AHorrorPlayerController, AHorrorPlayerController::StaticClass, TEXT("AHorrorPlayerController"), &Z_Registration_Info_UClass_AHorrorPlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AHorrorPlayerController), 572699135U) },
	};
}; // Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorPlayerController_h__Script_MyProject_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorPlayerController_h__Script_MyProject_1017016668{
	TEXT("/Script/MyProject"),
	Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorPlayerController_h__Script_MyProject_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RiderProjectsUnreal_SPM_2026_SPM_2026_whitebox_MyProject_Source_MyProject_HorrorPlayerController_h__Script_MyProject_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
