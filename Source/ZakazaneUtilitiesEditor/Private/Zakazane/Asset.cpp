// Copyright ZAKAZANE Studio. All Rights Reserved.

#include "Zakazane/Asset.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetToolsModule.h"
#include "Editor.h"
#include "EditorScriptingHelpers.h"
#include "Engine/DataAsset.h"
#include "Factories/DataAssetFactory.h"
#include "FileHelpers.h"
#include "IAssetTools.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Zakazane/ReturnIfMacros.h"
#include "ZakazaneGameEditorLog.h"

namespace Zkz::Game::Editor
{

IAssetEditorInstance* FindAssetEditorFor(UObject& Asset)
{
	ZKZ_RETURN_IF_INVALID(GEditor, nullptr);

	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	ZKZ_RETURN_IF_INVALID(AssetEditorSubsystem, nullptr);

	return AssetEditorSubsystem->FindEditorForAsset(&Asset, false);
}

}  // namespace Zkz::Game::Editor

UObject* UZkzAssetFunctionLibrary::CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* Factory)
{
	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();

	if (Factory == nullptr)
	{
		for (UFactory* Fac : AssetTools.GetNewAssetFactories())
		{
			if (Fac->SupportedClass == AssetClass)
			{
				Factory = Fac;
				break;
			}
		}

		if (Factory == nullptr)
		{
			UE_LOG(
				LogZakazaneGameEditor,
				Error,
				TEXT("Can't create asset, because factory for provided class (%s) could not be found"),
				*AssetClass->GetName());

			return nullptr;
		}
	}

	if (Factory->SupportedClass != AssetClass)
	{
		UE_LOG(
			LogZakazaneGameEditor,
			Error,
			TEXT("Can't create asset, because factory (%s) does not support provided class (%s)"),
			*Factory->GetName(),
			*AssetClass->GetName());

		return nullptr;
	}

	FString AssetName;
	FString PackageName;
	AssetTools.CreateUniqueAssetName(AssetPath, TEXT(""), PackageName, AssetName);

	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
	UObject* Asset = AssetTools.CreateAsset(AssetName, PackagePath, AssetClass, Factory);
	if (Asset == nullptr)
	{
		UE_LOG(LogZakazaneGameEditor, Error, TEXT("Can't create asset. Make sure provided path is valid"));

		return nullptr;
	}

	return Asset;
}

UObject* UZkzAssetFunctionLibrary::DuplicateAsset(UObject* SourceAsset, const FString& AssetPathTarget)
{
	if (!ensureAlwaysMsgf(SourceAsset != nullptr, TEXT("Provided SourceAsset is null")))
	{
		return nullptr;
	}

	FString AssetName;
	FString PackageName;
	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	AssetTools.CreateUniqueAssetName(AssetPathTarget, TEXT(""), PackageName, AssetName);

	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
	UObject* DuplicatedObject = AssetTools.DuplicateAsset(AssetName, PackagePath, SourceAsset);
	if (DuplicatedObject == nullptr)
	{
		UE_LOG(LogZakazaneGameEditor, Error, TEXT("Can't create asset. Make sure provided path is valid"));

		return nullptr;
	}

	return DuplicatedObject;
}
UObject* UZkzAssetFunctionLibrary::CreateDataAsset(const FString& AssetPath, UClass* DataAssetClass)
{
	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	ZKZ_RETURN_IF_INVALID(Factory, nullptr)

	Factory->DataAssetClass = DataAssetClass;

	UObject* CreatedAsset = CreateAsset(AssetPath, UDataAsset::StaticClass(), Factory);
	return CreatedAsset;
}

bool UZkzAssetFunctionLibrary::IsAssetExists(const FString& AssetPath)
{
	return IsValid(GetAssetByPath(AssetPath));
}

UObject* UZkzAssetFunctionLibrary::GetAssetByPath(const FString& AssetPath)
{
	ZKZ_RETURN_IF(AssetPath.IsEmpty(), nullptr);

	// Get file and path
	const FName FileName = FName(*FPaths::GetBaseFilename(AssetPath));
	const FName Path = FName(*FPaths::GetPath(AssetPath));
	ZKZ_RETURN_IF(!FileName.IsValid(), nullptr)
	ZKZ_RETURN_IF(!Path.IsValid(), nullptr)

	// Get assets from the registry
	IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();
	//const TScriptInterface<IAssetRegistry> AssetRegistry = UAssetRegistryHelpers::GetAssetRegistry();
	TArray<FAssetData> OutAssetsData;
	AssetRegistry.GetAssetsByPath(Path, OutAssetsData);
	ZKZ_RETURN_IF(OutAssetsData.IsEmpty(), nullptr)

	// Find the asset by name
	for (FAssetData AssetData : OutAssetsData)
	{
		if (AssetData.AssetName == FileName)
		{
			return AssetData.GetAsset();
		}
	}

	return nullptr;
}
FString UZkzAssetFunctionLibrary::GetWorldAssetPath(const UWorld* World, const FString& AssetName)
{
	ZKZ_RETURN_IF_INVALID(World, {})

	// Prepare path
	FString ResultPath = FPaths::GetPath(World->GetPathName()) + TEXT("/") + World->GetName() + TEXT("/") + AssetName;

	// Validate
	FText ErrorMessage;
	if (!FPaths::ValidatePath(ResultPath, &ErrorMessage))
	{
		UE_LOG(LogTemp, Warning, TEXT("Error while generating a path for the asset:%s"), *ErrorMessage.ToString())
		return {};
	}

	return ResultPath;
}

void UZkzAssetFunctionLibrary::PromptForCheckoutAndSave(UObject* _ObjectToSave, bool _CheckDirty, bool _PromptToSave)
{
	TArray<UPackage*> packagesToSave = {_ObjectToSave->GetPackage()};
	FEditorFileUtils::PromptForCheckoutAndSave(packagesToSave, _CheckDirty, _PromptToSave);
}

TArray<FString> UZkzAssetFunctionLibrary::GetAssetPathsInDirectory(
	const FString& _DirectoryPath, bool _Recursive, TSubclassOf<UObject> _AssetClass, bool _RecursiveClasses)
{
	TArray<FString> assetPaths;
	IAssetRegistry& assetRegistry =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")).Get();

	FString failureReason;
	FString directoryPathConverted =
		EditorScriptingHelpers::ConvertAnyPathToLongPackagePath(_DirectoryPath, failureReason);

	FARFilter arFilter;
	arFilter.PackagePaths = {FName(directoryPathConverted)};
	arFilter.bRecursivePaths = _Recursive;

	if (_AssetClass != nullptr)
	{
		arFilter.ClassPaths = {_AssetClass->GetClassPathName()};
		arFilter.bRecursiveClasses = _RecursiveClasses;
	}

	TArray<FAssetData> assetDataArray;
	assetRegistry.GetAssets(arFilter, assetDataArray);

	if (assetDataArray.Num() > 0)
	{
		assetPaths.Reserve(assetDataArray.Num());
		for (const auto assetData : assetDataArray)
		{
			assetPaths.Add(assetData.GetObjectPathString());
		}
	}

	assetPaths.Sort();
	return assetPaths;
}

UObject* UZkzAssetFunctionLibrary::LoadAssetFromPath(const FString& _AssetPath)
{
	// IAssetRegistry& assetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")).Get();
	FString failureReason;
	FString assetPathConverted = EditorScriptingHelpers::ConvertAnyPathToObjectPath(_AssetPath, failureReason);
	UObject* assetLoaded = LoadObject<UObject>(nullptr, *assetPathConverted);

	return assetLoaded;
}
