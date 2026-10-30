// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/PZHudWidget.h"
#include "UI/PZHudFormat.h"
#include "Blueprint/WidgetTree.h"
#include "Combat/PZHealthComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"

namespace
{
	constexpr int32 FontSize = 28;
	const FVector2D ScreenOffset(32.f, 24.f);
	const FLinearColor LabelColor(0.6f, 0.85f, 1.f);
	const FLinearColor ValueColor(1.f, 1.f, 1.f);
	// Dark backing keeps the text readable over the light greybox floor.
	const FLinearColor PanelColor(0.f, 0.f, 0.f, 0.55f);
	const FMargin PanelPadding(16.f, 8.f);

	UTextBlock* MakeText(UWidgetTree* Tree, const FText& Text, const FLinearColor& Color)
	{
		UTextBlock* Block = Tree->ConstructWidget<UTextBlock>();
		Block->SetText(Text);
		Block->SetColorAndOpacity(FSlateColor(Color));
		Block->SetShadowOffset(FVector2D(2.f, 2.f));
		FSlateFontInfo Font = Block->GetFont();
		Font.Size = FontSize;
		Block->SetFont(Font);
		return Block;
	}
}

TSharedRef<SWidget> UPZHudWidget::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		UCanvasPanel* Canvas = WidgetTree->ConstructWidget<UCanvasPanel>();
		WidgetTree->RootWidget = Canvas;

		UBorder* Panel = WidgetTree->ConstructWidget<UBorder>();
		Panel->SetBrushColor(PanelColor);
		Panel->SetPadding(PanelPadding);
		if (UCanvasPanelSlot* PanelSlot = Canvas->AddChildToCanvas(Panel))
		{
			PanelSlot->SetPosition(ScreenOffset);
			PanelSlot->SetAutoSize(true);
		}

		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>();
		Panel->SetContent(Row);

		UTextBlock* Label = MakeText(WidgetTree, NSLOCTEXT("ProtocolZero", "HealthLabel", "HEALTH"), LabelColor);
		if (UHorizontalBoxSlot* LabelSlot = Row->AddChildToHorizontalBox(Label))
		{
			LabelSlot->SetPadding(FMargin(0.f, 0.f, 16.f, 0.f));
		}

		HealthValue = MakeText(WidgetTree, FText::GetEmpty(), ValueColor);
		Row->AddChildToHorizontalBox(HealthValue);

		// Display only: let every click fall through to the game.
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	TSharedRef<SWidget> Result = Super::RebuildWidget();
	Refresh();
	return Result;
}

void UPZHudWidget::TrackHealth(UPZHealthComponent* InHealth)
{
	if (UPZHealthComponent* Previous = Health.Get())
	{
		Previous->OnDamaged.RemoveDynamic(this, &UPZHudWidget::HandleDamaged);
	}

	Health = InHealth;
	if (InHealth)
	{
		InHealth->OnDamaged.AddUniqueDynamic(this, &UPZHudWidget::HandleDamaged);
	}
	Refresh();
}

void UPZHudWidget::HandleDamaged(float Amount, FVector HitDirection, float CurrentHealth)
{
	Refresh();
}

void UPZHudWidget::Refresh()
{
	const UPZHealthComponent* Tracked = Health.Get();
	if (HealthValue && Tracked)
	{
		HealthValue->SetText(PZHudFormat::HealthText(Tracked->GetCurrentHealth(), Tracked->GetMaxHealth()));
	}
}
