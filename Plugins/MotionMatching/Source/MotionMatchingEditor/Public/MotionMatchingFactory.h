// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MotionMatchingFactory.generated.h"

struct FImportMotionMatchingData
{
    class USkeleton*        TargetSkeleton;
    class UAnimSequence*    TargetAnimation;
    TArray<FName>           TargetMotionBones;
};

UCLASS()
class MOTIONMATCHINGEDITOR_API UMotionMatchingFactory : public UFactory
{
	GENERATED_BODY()
    
public:
    UMotionMatchingFactory(const FObjectInitializer& ObjectInitializer);
    
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
    virtual bool ConfigureProperties();
    
private:
    TSharedPtr<FImportMotionMatchingData> ImportMotionMatchingData;
};
