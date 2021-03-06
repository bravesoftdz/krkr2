//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2005  W.Dee <dee@kikyou.info>

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------

#ifndef ConfSettingsUnitH
#define ConfSettingsUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ConfMainFrameUnit.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TConfSettingsForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TButton *OKButton;
	TButton *CancelButton;
	TConfMainFrame *ConfMainFrame;
	TOpenDialog *OpenDialog;
	TLabel *SpecifiedFileIsNotKrKrLabel;
	TLabel *IconChangeWarnLabel;
	TLabel *FileWroteLabel;
	TLabel *ProgramMayRunningLabel;
	void __fastcall OKButtonClick(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TConfSettingsForm(TComponent* Owner);

	bool __fastcall SelectFile();

	bool __fastcall ProcessFile(AnsiString filename);

	void __fastcall ShowUserConfig(AnsiString exefile);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfSettingsForm *ConfSettingsForm;
//---------------------------------------------------------------------------
#endif
