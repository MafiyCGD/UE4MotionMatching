// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "SCheckBoxListRow.h"

class SMotionMatchingCreateDialog : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMotionMatchingCreateDialog) {}
    SLATE_END_ARGS()
    
    void Construct(const FArguments& InArgs);
    // Sets properties for the supplied MotionFieldFactory
    bool ConfigureProperties(TSharedPtr<struct FImportMotionMatchingData> ImportData);
    
private:
    FReply OnCancel();
    FReply OnOk();
    
    void CloseDialog(bool bWasPicked=false);
    
    void MakeSkeletonPicker();
    void MakeAnimationPicker();
    void BuildBonesList();
    void OnSkeletonSelected(const FAssetData& AssetData);
    void OnAnimationSelected(const FAssetData& AssetData);
    
    TSharedRef<ITableRow> OnGenerateRowForList(CheckBoxListItem::ListItemPtr, const TSharedRef<STableViewBase>& OwnerTable);
    
    TSharedPtr<SVerticalBox>                              AssetsContainer;
    TSharedPtr<SListView<CheckBoxListItem::ListItemPtr>>  BonesListView;
    TArray<CheckBoxListItem::ListItemPtr>                 BoneItems;
    
    // The factory for which we are setting up properties
    TSharedPtr<struct FImportMotionMatchingData> ImportMotionMatchingData;
    
    FAssetData TargetSkeleton;
    FAssetData TargetAnimation;
    
    TWeakPtr<SWindow> PickerWindow;
    bool bOkClicked;
};
