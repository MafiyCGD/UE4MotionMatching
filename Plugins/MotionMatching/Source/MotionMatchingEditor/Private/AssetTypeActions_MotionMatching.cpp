// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_MotionMatching.h"
#include "MotionMatchingData.h"

FText FAssetTypeActions_MotionMatching::GetName() const
{
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_MotionMatching", "Motion Matching");
}

FColor FAssetTypeActions_MotionMatching::GetTypeColor() const
{
    return FColor::Magenta;
}

UClass* FAssetTypeActions_MotionMatching::GetSupportedClass() const
{
    return UMotionMatchingData::StaticClass();
}

uint32 FAssetTypeActions_MotionMatching::GetCategories()
{
    return EAssetTypeCategories::Animation;
}
