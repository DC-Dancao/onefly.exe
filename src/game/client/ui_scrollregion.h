 
 
#ifndef GAME_CLIENT_UI_SCROLLREGION_H
#define GAME_CLIENT_UI_SCROLLREGION_H

#include "ui.h"

struct CScrollRegionParams
{
	float m_ScrollbarThickness;
	float m_ScrollbarMargin;
	bool m_ScrollbarNoOuterMargin;
	float m_SliderMinSize;
	float m_ScrollUnit;
	ColorRGBA m_ClipBgColor;
	ColorRGBA m_ScrollbarBgColor;
	ColorRGBA m_RailBgColor;
	ColorRGBA m_SliderColor;
	ColorRGBA m_SliderColorHover;
	ColorRGBA m_SliderColorGrabbed;
	bool m_ForceShowScrollbar;
	bool m_ScrollHorizontal;

	CScrollRegionParams()
	{
		m_ScrollbarThickness = 20.0f;
		m_ScrollbarMargin = 5.0f;
		m_ScrollbarNoOuterMargin = false;
		m_SliderMinSize = 25.0f;
		m_ScrollUnit = 10.0f;
		m_ClipBgColor = ColorRGBA(0.0f, 0.0f, 0.0f, 0.0f);
		m_ScrollbarBgColor = ColorRGBA(0.0f, 0.0f, 0.0f, 0.0f);
		m_RailBgColor = ColorRGBA(1.0f, 1.0f, 1.0f, 0.25f);
		m_SliderColor = ColorRGBA(0.8f, 0.8f, 0.8f, 1.0f);
		m_SliderColorHover = ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		m_SliderColorGrabbed = ColorRGBA(0.9f, 0.9f, 0.9f, 1.0f);
		m_ForceShowScrollbar = false;
		m_ScrollHorizontal = false;
	}

	ColorRGBA SliderColor(bool Active, bool Hovered) const
	{
		if(Active)
			return m_SliderColorGrabbed;
		else if(Hovered)
			return m_SliderColorHover;
		return m_SliderColor;
	}
};

 \
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\



class CScrollRegion : private CUIElementBase
{
public:
	enum EScrollRelative
	{
		SCROLLRELATIVE_UP = -1,
		SCROLLRELATIVE_NONE = 0,
		SCROLLRELATIVE_DOWN = 1,
	};

private:
	float m_ScrollPos;
	float m_ContentSize;
	float m_RequestScrollPos; 
	
	bool m_RequestScrollSmooth;
	
	EScrollRelative m_ScrollDirection;
	float m_ScrollSpeedMultiplier;

	float m_AnimTimeMax;
	float m_AnimTime;
	float m_AnimInitScrollPos;
	float m_AnimTargetScrollPos;

	CUIRect m_ContentAreaRect;
	CUIRect m_RailRect;
	CUIRect m_LastAddedRect; 
	float m_SliderGrabPos; 
	float m_ContentScrollOffset;
	CScrollRegionParams m_Params;

public:
	enum EScrollOption
	{
		SCROLLHERE_KEEP_IN_VIEW = 0,
		SCROLLHERE_TOP,
		SCROLLHERE_BOTTOM,
		
		SCROLLHERE_CENTER,
		
	};

	CScrollRegion();
	void Reset();

	void Begin(CUIRect *pClipRect, const CScrollRegionParams *pParams = nullptr);
	void End();
	bool AddRect(const CUIRect &Rect, bool ShouldScrollHere = false); 
	void ScrollHere(EScrollOption Option = SCROLLHERE_KEEP_IN_VIEW, bool Smooth = false); 
	void ScrollRelative(EScrollRelative Direction, float SpeedMultiplier = 1.0f);
	void ScrollRelativeDirect(vec2 ScrollAmount);
	void DoEdgeScrolling();
	bool RectClipped(const CUIRect &Rect) const;
	bool ContentOverflows() const;
	bool ScrollbarShown() const;
	bool Animating() const;
	bool Active() const;
	
	bool AtBottom(float Epsilon = 2.0f) const { return !ContentOverflows() || m_ScrollPos >= MaxScroll() - Epsilon; }
	

private:
	float ContentAreaPos() const;
	float ContentAreaSize() const;
	float MaxScroll() const;
	CUIRect SplitContentArea();
	void DrawBackground(const CUIRect &ScrollbarBg);
	void DoScrollInput();
	void UpdateHotScrollRegion();
	void AdvanceAnimation();
	void DoSlider();
};

#endif
