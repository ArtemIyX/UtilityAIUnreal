#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUtilityAIEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

public:
	
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> StyleSet;
	static const ISlateStyle& Get();
};
