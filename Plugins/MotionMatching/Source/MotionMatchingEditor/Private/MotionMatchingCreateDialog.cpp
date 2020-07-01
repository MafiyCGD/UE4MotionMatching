// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatchingCreateDialog.h"

#include "MotionMatchingFactory.h"
#include "Animation/AnimSequence.h"
#include "EditorStyleSet.h"

#include "Widgets/SWindow.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"

#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Layout/SScrollBox.h"
#include "WIdgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "MotionMatchingDialog"

void SMotionMatchingCreateDialog::Construct(const FArguments& InArgs)
{
    bOkClicked = false;

    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FEditorStyle::GetBrush("Menu.Background"))
        [
            SNew(SVerticalBox)
            +SVerticalBox::Slot()
            [
                SNew(SBorder)
                [
                    SAssignNew(AssetsContainer, SVerticalBox)
                ]
            ]
            +SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(0.0f, 4.0f, 0.0f, 0.0f)
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
                .Padding(0.0f)
                [
                    SAssignNew(BonesListView, SListView<CheckBoxListItem::ListItemPtr>)
                    .SelectionMode(ESelectionMode::None)
                    .ListItemsSource(&BoneItems)
                    .OnGenerateRow(this, &SMotionMatchingCreateDialog::OnGenerateRowForList)
                    .HeaderRow
                    (
                        SNew(SHeaderRow)
                        +SHeaderRow::Column("Bone Name")
                        .DefaultLabel(LOCTEXT("BoneName", "Bone Name"))
                 
                        +SHeaderRow::Column("Is Motion")
                        .DefaultLabel(LOCTEXT("IsMotion", "Is Motion"))
                        .HAlignCell(HAlign_Center)
                        .HAlignHeader(HAlign_Center)
                    )
                ]
            ]
            +SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Bottom)
            .Padding(8)
            [
                SNew(SUniformGridPanel)
                +SUniformGridPanel::Slot(0, 0)
                [
                    SNew(SButton)
                    .ContentPadding( FEditorStyle::GetMargin("StandardDialog.ContentPadding") )
                    .HAlign(HAlign_Center)
                    .Text(LOCTEXT("OK", "OK"))
                    .OnClicked(this, &SMotionMatchingCreateDialog::OnOk)
                ]
                +SUniformGridPanel::Slot(1, 0)
                [
                    SNew(SButton)
                    .ContentPadding( FEditorStyle::GetMargin("StandardDialog.ContentPadding") )
                    .HAlign(HAlign_Center)
                    .Text(LOCTEXT("Cancel", "Cancel"))
                    .OnClicked(this, &SMotionMatchingCreateDialog::OnCancel)
                ]
            ]
        ]
    ];
    
    MakeSkeletonPicker();
    MakeAnimationPicker();
    BuildBonesList();
}

TSharedRef<ITableRow> SMotionMatchingCreateDialog::OnGenerateRowForList(CheckBoxListItem::ListItemPtr Item, const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(SCheckBoxListRow, OwnerTable)
            .Item(Item);
}

void SMotionMatchingCreateDialog::MakeSkeletonPicker()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    
    FAssetPickerConfig AssetPickerConfig;
    AssetPickerConfig.Filter.ClassNames.Add(USkeleton::StaticClass()->GetFName());
    AssetPickerConfig.bAllowNullSelection = false;
    AssetPickerConfig.bShowTypeInColumnView = false;
    AssetPickerConfig.SelectionMode = ESelectionMode::SingleToggle;
    AssetPickerConfig.InitialAssetViewType = EAssetViewType::Column;
    AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SMotionMatchingCreateDialog::OnSkeletonSelected);
    
    AssetsContainer->AddSlot()
        .AutoHeight()
        [
            SNew(STextBlock)
            .Text(LOCTEXT("TargetSkeleton", "Target Skeleton:"))
        ];
    AssetsContainer->AddSlot()
        [
            ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
        ];
}

void SMotionMatchingCreateDialog::MakeAnimationPicker()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    
    FAssetPickerConfig AssetPickerConfig;
    AssetPickerConfig.Filter.ClassNames.Add(UAnimSequence::StaticClass()->GetFName());
    AssetPickerConfig.SelectionMode = ESelectionMode::Multi;
    AssetPickerConfig.bAllowNullSelection = false;
    AssetPickerConfig.bShowTypeInColumnView = false;
    AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
    AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SMotionMatchingCreateDialog::OnAnimationSelected);
    
    AssetsContainer->AddSlot()
    .AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("Animations", "Animations:"))
    ];
    
    AssetsContainer->AddSlot()
    [
        ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
    ];
}

void SMotionMatchingCreateDialog::BuildBonesList()
{
    if(TargetSkeleton.IsValid())
    {
        if(!TargetSkeleton.IsAssetLoaded())
        {
            TargetSkeleton.GetPackage()->FullyLoad();
        }
        
        USkeleton* Skeleton = Cast<USkeleton>(TargetSkeleton.GetAsset());
        if(Skeleton)
        {
            BoneItems.Reset();
            
            int BonesNum = Skeleton->GetReferenceSkeleton().GetNum();
            for(int i =0 ; i < BonesNum; ++i)
            {
                BoneItems.Add(MakeShareable(new CheckBoxListItem(Skeleton->GetReferenceSkeleton().GetBoneName(i))));
            }
            
            BonesListView->RequestListRefresh();
        }
    }
}

void SMotionMatchingCreateDialog::OnSkeletonSelected(const FAssetData& AssetData)
{
    TargetSkeleton = AssetData;
    BuildBonesList();
}

void SMotionMatchingCreateDialog::OnAnimationSelected(const FAssetData& AssetData)
{
    TargetAnimation = AssetData;
}

void SMotionMatchingCreateDialog::CloseDialog(bool bWasPicked)
{
    bOkClicked = bWasPicked;
    if ( PickerWindow.IsValid() )
    {
        PickerWindow.Pin()->RequestDestroyWindow();
    }
}

FReply SMotionMatchingCreateDialog::OnOk()
{
    for(int i=0; i < BoneItems.Num(); ++i)
    {
        if(BoneItems[i]->IsChecked())
        {
            ImportMotionMatchingData->TargetMotionBones.Add(BoneItems[i]->GetName());
        }
    }
    
    if (!TargetAnimation.IsValid())
    {
        // if TargetAnimation is not valid
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NeedValidAnimation", "Must specify a valid animation."));
        return FReply::Handled();
    }
    
    if (!TargetSkeleton.IsValid())
    {
        // if TargetSkeleton is not valid
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NeedValidSkeleton", "Must specify a skeleton."));
        return FReply::Handled();
    }
    
    ImportMotionMatchingData->TargetSkeleton = Cast<USkeleton>(TargetSkeleton.GetAsset());
    ImportMotionMatchingData->TargetAnimation = Cast<UAnimSequence>(TargetAnimation.GetAsset());
    
    CloseDialog(true);
    return FReply::Handled();
}

FReply SMotionMatchingCreateDialog::OnCancel()
{
    CloseDialog();
    return FReply::Handled();
}

bool SMotionMatchingCreateDialog::ConfigureProperties(TSharedPtr<FImportMotionMatchingData> ImportData)
{
    this->ImportMotionMatchingData = ImportData;
    
    TSharedRef<SWindow> Window = SNew(SWindow)
        .Title(LOCTEXT("Motion Matrching Options", "Motion Matching"))
        .ClientSize(FVector2D(400.0f, 700.0f))
        .SupportsMinimize(false).SupportsMaximize(false)
        [
            AsShared()
        ];
    
    PickerWindow = Window;
    
    GEditor->EditorAddModalWindow(Window);
    
    return bOkClicked;
}

#undef LOCTEXT_NAMESPACE
