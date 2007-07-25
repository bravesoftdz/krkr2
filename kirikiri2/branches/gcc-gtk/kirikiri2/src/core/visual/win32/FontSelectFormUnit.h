//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2005  W.Dee <dee@kikyou.info>

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Font Selector Form
//---------------------------------------------------------------------------
#ifndef FontSelectFormUnitH
#define FontSelectFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
extern HDWP TVPShowFontSelectFormTop(HDWP hdwp);
//---------------------------------------------------------------------------
extern void TVPShowFontSelectFormAtAppActivate();
extern void TVPHideFontSelectFormAtAppDeactivate();
//---------------------------------------------------------------------------
class TTVPFontSelectForm : public TForm
{
	friend int CALLBACK TVPFSFEnumFontsProc(LOGFONT *lplf, TEXTMETRIC *lptm,
		DWORD type, LPARAM data);
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TListBox *ListBox;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label4;
	TMemo *Memo;
	TButton *OKButton;
	TButton *CancelButton;
	void __fastcall ListBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall ListBoxClick(TObject *Sender);
	void __fastcall ListBoxDblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// ���[�U�[�錾
	int Flags;
	LOGFONT RefFont;
protected:

BEGIN_MESSAGE_MAP
	VCL_MESSAGE_HANDLER( WM_USER+0x30, TMessage, WMShowTop)
	VCL_MESSAGE_HANDLER( WM_USER+0x31, TMessage, WMSetVisible)
END_MESSAGE_MAP(TForm)
	void __fastcall WMShowTop(TMessage &Msg);
	void __fastcall WMSetVisible(TMessage &Msg);

public:		// ���[�U�[�錾
	__fastcall TTVPFontSelectForm(TComponent* Owner, TCanvas *RefCanvas, int flags,
		AnsiString caption, AnsiString prompt, AnsiString samplestring);

	AnsiString FontName;

	void InvokeShowTop();
	void InvokeSetVisible();
	HDWP ShowTop(HDWP hdwp);
};
//---------------------------------------------------------------------------
extern PACKAGE TTVPFontSelectForm *TVPFontSelectForm;
//---------------------------------------------------------------------------
#endif