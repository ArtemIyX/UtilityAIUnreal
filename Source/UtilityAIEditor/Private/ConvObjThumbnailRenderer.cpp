// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "ConvObjThumbnailRenderer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "UtilityAIConvertObject.h"

bool UConvObjThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	// Try to cast the object to the UUtilityAIConvertObject
	if (UUtilityAIConvertObject* convObject = Cast<UUtilityAIConvertObject>(Object))
	{
		return true;
	}
	return false;
}

void UConvObjThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
	OutWidth = 255;
	OutHeight = 255;
}

void UConvObjThumbnailRenderer::DrawCurve(const FRichCurve& Curve, FCanvas* Canvas, const FLinearColor& Color, float MinTime, float MaxTime, float MinValue, float MaxValue)
{
	FVector2D textureSize = Canvas->GetRenderTarget()->GetSizeXY();
	if (textureSize.X == 0 || textureSize.Y == 0)
	{
		return;
	}

	//Render the curve to the canvas with line segments
	FVector2D prevPos;
	const int32 samples = textureSize.X / 2.5;
	const float Padding = samples / 5.0;
	for (int32 i = 0; i < samples; i++)
	{
		float time;
		if (i < Padding / 2)
		{
			time = MinTime;
		}
		else if (i > (samples - Padding / 2))
		{
			time = MaxTime;
		}
		else
		{
			float TimeRange = (samples - Padding) * (MaxTime - MinTime);
			time = TimeRange == 0 ? 0 : (i - Padding / 2) / TimeRange + MinTime;
		}

		const float value = Curve.Eval(time);
		FVector2D Pos;
		Pos.X = i * textureSize.X / samples;
		const float normalizedValue = 1 - (value - MinValue) / (MaxValue - MinValue);
		Pos.Y = normalizedValue * (textureSize.Y - Padding * 2) + Padding;

		if (i > 0)
		{
			FCanvasLineItem line(prevPos, Pos);
			line.LineThickness = 1.5;
			line.SetColor(Color);
			line.Draw(Canvas);
		}
		prevPos = Pos;
	}
}

void UConvObjThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	if (UUtilityAIConvertObject* convObj = Cast<UUtilityAIConvertObject>(Object))
	{
		const FVector2D textureSize = Canvas->GetRenderTarget()->GetSizeXY();
		if (!bAdditionalViewFamily)
		{
			Canvas->Clear(FLinearColor::Black);
		}

		const FRichCurve* richCurve = convObj->Curve.GetRichCurveConst();
		if (!richCurve)
			return;

		if (richCurve->GetNumKeys() < 2)
		{
			FCanvasLineItem Line(FVector2D(0, textureSize.Y / 2), FVector2D(textureSize.X, textureSize.Y / 2));
			Line.LineThickness = 1.5;
			Line.SetColor(FLinearColor::Gray);
			Line.Draw(Canvas);
			return;
		}

		float MinTime, MaxTime;
		richCurve->GetTimeRange(MinTime, MaxTime);

		float MinValue, MaxValue;
		richCurve->GetValueRange(MinValue, MaxValue);
		DrawCurve(*richCurve, Canvas, FLinearColor::Gray, MinTime, MaxTime, MinValue, MaxValue);
	}
}