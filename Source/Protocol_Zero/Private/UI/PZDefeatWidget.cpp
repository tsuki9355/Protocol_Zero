// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/PZDefeatWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Player/PZPlayerController.h"

namespace
{
	constexpr int32 TitleFontSize = 48;
	constexpr int32 ButtonFontSize = 24;
	const FLinearColor OverlayColor(0.f, 0.f, 0.f, 0.6f);
	const FLinearColor TitleColor(1.f, 0.25f, 0.2f);

	UTextBlock* MakeText(UWidgetTree* Tree, const FText& Text, int32 Size, const FLinearColor& Color)
	{
		UTextBlock* Block = Tree->ConstructWidget<UTextBlock>();
		Block->SetText(Text);
		Block->SetColorAndOpacity(FSlateColor(Color));
		Block->SetJustification(ETextJustify::Center);
		FSlateFontInfo Font = Block->GetFont();
		Font.Size = Size;
		Block->SetFont(Font);
		return Block;
	}
}

TSharedRef<SWidget> UPZDefeatWidget::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		UBorder* Overlay = WidgetTree->ConstructWidget<UBorder>();
		Overlay->SetBrushColor(OverlayColor);
		Overlay->SetHorizontalAlignment(HAlign_Center);
		Overlay->SetVerticalAlignment(VAlign_Center);
		WidgetTree->RootWidget = Overlay;

		UVerticalBox* Column = WidgetTree->ConstructWidget<UVerticalBox>();
		Overlay->SetContent(Column);

		UTextBlock* Title = MakeText(WidgetTree, NSLOCTEXT("ProtocolZero", "DefeatTitle", "OPERATIVE DOWN"), TitleFontSize, TitleColor);
		if (UVerticalBoxSlot* TitleSlot = Column->AddChildToVerticalBox(Title))
		{
			TitleSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 24.f));
			TitleSlot->SetHorizontalAlignment(HAlign_Center);
		}

		UButton* RestartButton = WidgetTree->ConstructWidget<UButton>();
		RestartButton->SetContent(MakeText(WidgetTree, NSLOCTEXT("ProtocolZero", "Restart", "Restart"), ButtonFontSize, FLinearColor::Black));
		RestartButton->OnClicked.AddDynamic(this, &UPZDefeatWidget::HandleRestartClicked);
		if (UVerticalBoxSlot* ButtonSlot = Column->AddChildToVerticalBox(RestartButton))
		{
			ButtonSlot->SetHorizontalAlignment(HAlign_Center);
		}
	}

	return Super::RebuildWidget();
}

void UPZDefeatWidget::HandleRestartClicked()
{
	if (APZPlayerController* Controller = GetOwningPlayer<APZPlayerController>())
	{
		Controller->RestartRun();
	}
}
