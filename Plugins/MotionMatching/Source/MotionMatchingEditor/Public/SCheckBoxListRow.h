// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class CheckBoxListItem
{
public:
    CheckBoxListItem(const FName& Name) { SetName(Name);}
    void SetIsChecked(bool InVal) { bIsChecked = InVal; }
    bool IsChecked() const { return bIsChecked; }
    void SetName(const FName& InVal) { ItemName = InVal; }
    const FName& GetName() const { return ItemName; }
    
    using ListItemPtr = TSharedPtr<class CheckBoxListItem, ESPMode::ThreadSafe>;
private:
    FName ItemName = "";
    bool bIsChecked = false;
};

class SCheckBoxListRow : public SMultiColumnTableRow<CheckBoxListItem::ListItemPtr>
{
public:
    SLATE_BEGIN_ARGS(SCheckBoxListRow) {}
        SLATE_ARGUMENT(CheckBoxListItem::ListItemPtr, Item)
    SLATE_END_ARGS()
    
    void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
    
    //~ SMultiColumnTableRow interface
    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;
    //~ SMultiColumnTableRow interface
private:
    CheckBoxListItem::ListItemPtr Item;
    
    ECheckBoxState HandleCheckBoxIsChecked() const;
    void HandleCheckBoxStateChanged(ECheckBoxState NewState);
};
