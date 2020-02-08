// Fill out your copyright notice in the Description page of Project Settings.

#include "MotionMatchingFactory.h"
#include "MotionMatchingData.h"
#include "MotionMatchingCreateDialog.h"


UMotionMatchingFactory::UMotionMatchingFactory(const FObjectInitializer& ObjectInitializer)
{
    bCreateNew = true;
    SupportedClass = UMotionMatchingData::StaticClass();
    MotionMatchingData = MakeShareable(new ImportMotionMatchingData());
}

bool UMotionMatchingFactory::ConfigureProperties()
{
    TSharedRef<SMotionMatchingCreateDialog> Dialog = SNew(SMotionMatchingCreateDialog);
    return Dialog->ConfigureProperties(MotionMatchingData);
}

UObject* UMotionMatchingFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UMotionMatchingData* MotionMatchingData = NewObject<UMotionMatchingData>(InParent, InClass, InName, Flags | RF_Transactional);
    if(MotionMatchingData)
    {
        return MotionMatchingData;
    }
    return nullptr;
}

