// Fill out your copyright notice in the Description page of Project Settings.

#include "SCheckBoxListRow.h"

#define LOCTEXT_NAMESPACE "SCheckBoxListRow"

void SCheckBoxListRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
    check(InArgs._Item.IsValid());
    
    Item = InArgs._Item;
    
    SMultiColumnTableRow<CheckBoxListItem::ListItemPtr>::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

TSharedRef<SWidget> SCheckBoxListRow::GenerateWidgetForColumn(const FName& ColumnName)
{
    if(ColumnName == TEXT("Bone Name"))
    {
        return SNew(SBox)
            .Padding(2.0f)
            .VAlign(VAlign_Center)
            .HAlign(HAlign_Center)
            [
                SNew(STextBlock)
                .Text(FText::FromName(Item->GetName()))
             ];
    }
    else if(ColumnName == TEXT("Is Motion"))
    {
        return SNew(SBox)
            .Padding(2.0f)
            .VAlign(VAlign_Center)
            .HAlign(HAlign_Center)
            [
                SNew(SCheckBox)
                .IsChecked(this, &SCheckBoxListRow::HandleCheckBoxIsChecked)
                .OnCheckStateChanged(this, &SCheckBoxListRow::HandleCheckBoxStateChanged)
                
             ];
    }
    
    return SNullWidget::NullWidget;
}

ECheckBoxState SCheckBoxListRow::HandleCheckBoxIsChecked() const
{
    if (Item->IsChecked())
    {
        return ECheckBoxState::Checked;
    }

    return ECheckBoxState::Unchecked;
}

void SCheckBoxListRow::HandleCheckBoxStateChanged(ECheckBoxState NewState)
{
    Item->SetIsChecked(NewState == ECheckBoxState::Checked);
}

#undef LOCTEXT_NAMESPACE
