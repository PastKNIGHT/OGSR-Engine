#pragma once

#include "MbHelpers.h"

#include "../Include/xrRender/FontRender.h"

class ENGINE_API CGameFont
{
    friend class dxFontRender;

public:
    enum EAligment
    {
        alLeft = 0,
        alRight,
        alCenter
    };

private:
    struct String
    {
        string1024 string;
        float x, y;
        float height;
        u32 c;
        EAligment align;
    };

protected:
    Fvector2 vHalfPixel;
    Ivector2 vTS;

    EAligment eCurrentAlignment;
    u32 dwCurrentColor;
    float fCurrentHeight;
    float fCurrentX, fCurrentY;
    Fvector2 vInterval;

    std::unique_ptr<Fvector[]> TCMap;
    float fHeight;
    float fXStep;
    float fYStep;
    float fTCHeight;
    float fXScale{};
    float fYScale{};
    xr_vector<String> strings;

    IFontRender* pFontRender;

    u32 uFlags;

public:
    enum
    {
        fsGradient = (1 << 0),
        fsDeviceIndependent = (1 << 1),
        fsValid = (1 << 2),
        fsMultibyte = (1 << 3),

        fsForceDWORD = u32(-1)
    };

    IC const Fvector& GetCharTC(u16 c) const { return TCMap[c]; }

public:
    CGameFont(LPCSTR section, u32 flags = 0);
    CGameFont(LPCSTR shader, LPCSTR texture, u32 flags = 0);
    ~CGameFont();

    void Initialize(LPCSTR shader, LPCSTR texture);

    IC void SetColor(u32 C) { dwCurrentColor = C; };

    void SetHeightI(float S);
    void SetHeight(float S);

    IC float GetHeight() const { return fCurrentHeight; }

    IC void SetInterval(float x, float y) { vInterval.set(x, y); };
    IC void SetInterval(const Fvector2& v) { vInterval.set(v); };

    IC const Fvector2& GetInterval() const { return vInterval; }

    IC void SetAligment(EAligment aligment) { eCurrentAlignment = aligment; }
    IC EAligment GetAligment() { return eCurrentAlignment; }

    float SizeOf_(LPCSTR s);
    float SizeOf_(const wide_char* wsStr);
    float SizeOf_(const char cChar); // only ANSII
    float SizeOf_(const u16 cChar); // only UTF-8

    float CurrentHeight_();

    float ScaleHeightDelta() const { return (fCurrentHeight * vInterval.y * GetHeightScale() - fCurrentHeight * vInterval.y) / 2; }

    void OutSetI(float x, float y);
    void OutSet(float x, float y);

    void MasterOut(BOOL bCheckDevice, BOOL bUseCoords, BOOL bScaleCoords, BOOL bUseSkip, float _x, float _y, float _skip, LPCSTR fmt, va_list p);

    u32 smart_strlen(const char* S);
    BOOL IsMultibyte() const { return (uFlags & fsMultibyte); }
    u16 SplitByWidth(u16* puBuffer, u16 uBufferSize, float fTargetWidth, const char* pszText);
    u16 GetCutLengthPos(float fTargetWidth, const char* pszText);

    void OutI(float _x, float _y, LPCSTR fmt, ...);
    void Out(float _x, float _y, LPCSTR fmt, ...);
    void OutNext(LPCSTR fmt, ...);

    void OutSkip(float val = 1.f);

    void OnRender();

    IC void Clear() { strings.clear(); };

    float GetWidthScale() const;
    float GetHeightScale() const;

    void SetWidthScale(float f) { fXScale = f; }
    void SetHeightScale(float f) { fYScale = f; }

    shared_str m_font_name;
    bool m_bCustom{};
};
