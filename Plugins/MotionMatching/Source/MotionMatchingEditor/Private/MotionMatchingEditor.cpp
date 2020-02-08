// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MotionMatchingEditor.h"
#include "AssetToolsModule.h"
#include "AssetTypeActions_MotionMatching.h"

#define LOCTEXT_NAMESPACE "FMotionMatchingEditorModule"

void FMotionMatchingEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    TSharedRef<IAssetTypeActions> Action = MakeShareable(new FAssetTypeActions_MotionMatching());
    AssetTools.RegisterAssetTypeActions(Action);
    //CreatedAssetTypeActions.Add(Action);
}

void FMotionMatchingEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMotionMatchingEditorModule, MotionMatching)
