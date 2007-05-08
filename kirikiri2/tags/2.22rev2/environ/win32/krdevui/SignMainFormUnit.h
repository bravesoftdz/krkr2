//---------------------------------------------------------------------------

#ifndef MainFormUnitH
#define MainFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TSignMainForm : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TButton *CloseButton;
	TPageControl *PageControl;
	TTabSheet *KeyGenTabSheet;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TButton *GenerateButton;
	TLabel *Label6;
	TEdit *GeneratedPublicKeyEdit;
	TLabel *Label7;
	TEdit *GeneratedPrivateKeyEdit;
	TLabel *Label8;
	TButton *CopyPublicKeyButton;
	TButton *CopyPrivateKeyButton;
	TLabel *Label9;
	TTabSheet *SignTabSheet;
	TLabel *Label10;
	TEdit *SignPrivateKeyEdit;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TEdit *SignFileNameEdit;
	TLabel *Label14;
	TButton *SignFileNameRefButton;
	TLabel *Label15;
	TLabel *Label16;
	TButton *SignButton;
	TOpenDialog *OpenDialog;
	TLabel *SignDoneLabel;
	TTabSheet *CheckSignTabSheet;
	TLabel *SignatureIsValidLabel;
	TButton *CheckSignatureButton;
	TLabel *Label18;
	TLabel *Label19;
	TButton *CheckSignFileNameRefButton;
	TEdit *CheckSignFileNameEdit;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TEdit *CheckSignPublicKeyEdit;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *SignatureIsInvalidLabel;
	void __fastcall GenerateButtonClick(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall CopyPublicKeyButtonClick(TObject *Sender);
	void __fastcall CopyPrivateKeyButtonClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SignFileNameRefButtonClick(TObject *Sender);
	void __fastcall SignButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckSignFileNameRefButtonClick(TObject *Sender);
	void __fastcall CheckSignatureButtonClick(TObject *Sender);
	void __fastcall SignPrivateKeyEditChange(TObject *Sender);
	void __fastcall CheckSignPublicKeyEditChange(TObject *Sender);
private:	// ���[�U�[�錾
protected:
BEGIN_MESSAGE_MAP
	VCL_MESSAGE_HANDLER( WM_DROPFILES,  TMessage, WMDropFiles);
END_MESSAGE_MAP(TForm)
	void __fastcall WMDropFiles(TMessage &Msg);

public:		// ���[�U�[�錾
	__fastcall TSignMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSignMainForm *SignMainForm;
//---------------------------------------------------------------------------
#endif