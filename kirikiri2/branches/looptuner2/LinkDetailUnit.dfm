object LinkDetailForm: TLinkDetailForm
  Left = 72
  Top = 357
  Width = 637
  Height = 350
  ActiveControl = WaveAreaPanel
  Caption = '�����N�̕ҏW'
  Color = clBtnFace
  Constraints.MinHeight = 189
  Constraints.MinWidth = 612
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '�l�r �o�S�V�b�N'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnMouseWheel = FormMouseWheel
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 12
  inline EditLinkAttribFrame: TEditLinkAttribFrame
    Width = 629
    Align = alTop
    TabOrder = 3
  end
  object ToolBarPanel: TPanel
    Left = 0
    Top = 252
    Width = 629
    Height = 31
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object PosAdjustToolBar: TToolBar
      Left = 175
      Top = 4
      Width = 284
      Height = 22
      Align = alNone
      AutoSize = True
      Caption = 'PosAdjustToolBar'
      EdgeBorders = []
      Flat = True
      Images = LoopTunerMainForm.ImageList
      TabOrder = 0
      object BeforePrevCrossToolButton: TToolButton
        Left = 0
        Top = 0
        Action = BeforePrevCrossAction
        ParentShowHint = False
        ShowHint = True
      end
      object BeforePrevFastToolButton: TToolButton
        Left = 23
        Top = 0
        Action = BeforePrevFastAction
        ParentShowHint = False
        ShowHint = True
      end
      object BeforePrevStepToolButton: TToolButton
        Left = 46
        Top = 0
        Action = BeforePrevStepAction
        ParentShowHint = False
        ShowHint = True
      end
      object BeforeNextStepToolButton: TToolButton
        Left = 69
        Top = 0
        Action = BeforeNextStepAction
        ParentShowHint = False
        ShowHint = True
      end
      object BeforeNextFastToolButton: TToolButton
        Left = 92
        Top = 0
        Action = BeforeNextFastAction
        ParentShowHint = False
        ShowHint = True
      end
      object BeforeNextCrossToolButton: TToolButton
        Left = 115
        Top = 0
        Action = BeforeNextCrossAction
        ParentShowHint = False
        ShowHint = True
      end
      object ToolButton2: TToolButton
        Left = 138
        Top = 0
        Width = 8
        Caption = 'ToolButton2'
        ImageIndex = 20
        Style = tbsSeparator
      end
      object AfterPrevCrossToolButton: TToolButton
        Left = 146
        Top = 0
        Action = AfterPrevCrossAction
        ParentShowHint = False
        ShowHint = True
      end
      object AfterPrevFastToolButton: TToolButton
        Left = 169
        Top = 0
        Action = AfterPrevFastAction
        ParentShowHint = False
        ShowHint = True
      end
      object AfterPrevStepToolButton: TToolButton
        Left = 192
        Top = 0
        Action = AfterPrevStepAction
        ParentShowHint = False
        ShowHint = True
      end
      object AfterNextStepToolButton: TToolButton
        Left = 215
        Top = 0
        Action = AfterNextStepAction
        ParentShowHint = False
        ShowHint = True
      end
      object AfterNextFastToolButton: TToolButton
        Left = 238
        Top = 0
        Action = AfterNextFastAction
        ParentShowHint = False
        ShowHint = True
      end
      object AfterNextCrossToolButton: TToolButton
        Left = 261
        Top = 0
        Action = AfterNextCrossAction
        ParentShowHint = False
        ShowHint = True
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 283
    Width = 629
    Height = 33
    Align = alBottom
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 1
    object PlayBeforeLabel: TLabel
      Left = 304
      Top = 5
      Width = 109
      Height = 12
      Hint = '�����N�܂ł��� %.1f �b'
      Caption = '�����N�܂ł��� %.1f �b'
      Visible = False
    end
    object BottomToolBar: TToolBar
      Left = 4
      Top = 4
      Width = 288
      Height = 25
      Align = alLeft
      AutoSize = True
      ButtonWidth = 25
      Caption = 'BottomToolBar'
      EdgeBorders = []
      Flat = True
      Images = LoopTunerMainForm.ImageList
      TabOrder = 0
      object ZoomInToolButton: TToolButton
        Left = 0
        Top = 0
        Action = ZoomInAction
        ParentShowHint = False
        ShowHint = True
      end
      object ZoomOutToolButton: TToolButton
        Left = 25
        Top = 0
        Action = ZoomOutAction
        ParentShowHint = False
        ShowHint = True
      end
      object MagnifyLabel: TLabel
        Left = 50
        Top = 0
        Width = 47
        Height = 22
        Alignment = taCenter
        AutoSize = False
        Caption = '/1024'
        Layout = tlCenter
      end
      object ToolButton17: TToolButton
        Left = 97
        Top = 0
        Width = 8
        Caption = 'ToolButton17'
        ImageIndex = 13
        Style = tbsSeparator
      end
      object SmoothToolButton: TToolButton
        Left = 105
        Top = 0
        Action = SmoothAction
        ParentShowHint = False
        ShowHint = True
      end
      object ToolButton15: TToolButton
        Left = 130
        Top = 0
        Width = 8
        Caption = 'ToolButton15'
        ImageIndex = 25
        Style = tbsSeparator
      end
      object StopPlayToolButton: TToolButton
        Left = 138
        Top = 0
        Action = StopPlayAction
        ParentShowHint = False
        ShowHint = True
      end
      object PlayHalfSecToolButton: TToolButton
        Left = 163
        Top = 0
        Action = PlayHalfSecAction
        ParentShowHint = False
        ShowHint = True
      end
      object Play1SecToolButton: TToolButton
        Left = 188
        Top = 0
        Action = Play1SecAction
        ParentShowHint = False
        ShowHint = True
      end
      object Play2SecToolButton: TToolButton
        Left = 213
        Top = 0
        Action = Play2SecAction
        Marked = True
        ParentShowHint = False
        ShowHint = True
      end
      object Play3SecToolButton: TToolButton
        Left = 238
        Top = 0
        Action = Play3SecAction
        ParentShowHint = False
        ShowHint = True
      end
      object Play5SecToolButton: TToolButton
        Left = 263
        Top = 0
        Action = Play5SecAction
        ParentShowHint = False
        ShowHint = True
      end
    end
    object OKButton: TButton
      Left = 454
      Top = 2
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      TabOrder = 2
      OnClick = OKButtonClick
    end
    object CancelButton: TButton
      Left = 542
      Top = 2
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = '�L�����Z��'
      TabOrder = 3
      OnClick = CancelButtonClick
    end
    object PlayBeforePaintBoxPanel: TPanel
      Left = 304
      Top = 18
      Width = 105
      Height = 7
      BevelOuter = bvLowered
      TabOrder = 1
      object PlayBeforePaintBox: TPaintBox
        Left = 1
        Top = 1
        Width = 103
        Height = 5
        Align = alClient
        OnPaint = PlayBeforePaintBoxPaint
      end
    end
  end
  object WaveAreaPanel: TPanel
    Left = 0
    Top = 30
    Width = 629
    Height = 222
    Align = alClient
    BevelOuter = bvLowered
    PopupMenu = WaveAreaPopupMenu
    TabOrder = 2
    OnEnter = WaveAreaPanelEnter
    OnExit = WaveAreaPanelExit
    object WavePaintBox: TPaintBox
      Left = 1
      Top = 1
      Width = 627
      Height = 220
      Align = alClient
      OnDblClick = WavePaintBoxDblClick
      OnMouseDown = WavePaintBoxMouseDown
      OnMouseMove = WavePaintBoxMouseMove
      OnMouseUp = WavePaintBoxMouseUp
      OnPaint = WavePaintBoxPaint
    end
    object AfterLinkLabel: TLabel
      Left = 568
      Top = 8
      Width = 45
      Height = 12
      Anchors = [akTop, akRight]
      Caption = '�����N��'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = '�l�r �o�S�V�b�N'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = True
    end
    object BeforeLinkLabel: TLabel
      Left = 16
      Top = 8
      Width = 45
      Height = 12
      Caption = '�����N�O'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = '�l�r �o�S�V�b�N'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = True
    end
  end
  object ActionList: TActionList
    Images = LoopTunerMainForm.ImageList
    Left = 456
    Top = 24
    object StopPlayAction: TAction
      Category = '�Đ�'
      Caption = '�Đ���~(&S)'
      Hint = '�Đ����~'
      ImageIndex = 4
      OnExecute = StopPlayActionExecute
    end
    object ZoomInAction: TAction
      Category = '�\��'
      Caption = '�Y�[���E�C��(&I)'
      Hint = '�g�`���g�傷��'
      ImageIndex = 9
      OnExecute = ZoomInActionExecute
    end
    object ZoomOutAction: TAction
      Category = '�\��'
      Caption = '�Y�[���E�A�E�g(&O)'
      Hint = '�g�`���k������'
      ImageIndex = 10
      OnExecute = ZoomOutActionExecute
    end
    object PlayHalfSecAction: TAction
      Category = '�Đ�'
      Caption = '0.5�b�O����Đ�(&0)'
      Hint = '0.5�b�O����Đ�'
      ImageIndex = 25
      OnExecute = PlayHalfSecActionExecute
    end
    object Play1SecAction: TAction
      Category = '�Đ�'
      Caption = '1�b�O����Đ�(&1)'
      Hint = '1�b�O����Đ�'
      ImageIndex = 26
      OnExecute = Play1SecActionExecute
    end
    object Play2SecAction: TAction
      Category = '�Đ�'
      Caption = '2�b�O����Đ�(&2)'
      Hint = '2�b�O����Đ�'
      ImageIndex = 27
      OnExecute = Play2SecActionExecute
    end
    object Play3SecAction: TAction
      Category = '�Đ�'
      Caption = '3�b�O����Đ�(&3)'
      Hint = '3�b�O����Đ�'
      ImageIndex = 28
      OnExecute = Play3SecActionExecute
    end
    object Play5SecAction: TAction
      Category = '�Đ�'
      Caption = '5�b�O����Đ�(&5)'
      Hint = '5�b�O����Đ�'
      ImageIndex = 29
      OnExecute = Play5SecActionExecute
    end
    object SmoothAction: TAction
      Category = '����'
      Caption = '�X���[�Y(&S)'
      Hint = '�����N���X���[�Y�ɂ���'
      ImageIndex = 30
      OnExecute = SmoothActionExecute
    end
    object BeforePrevCrossAction: TAction
      Category = '�����N�O'
      Caption = '�O�̃N���b�V���O�E�|�C���g��(&A)'
      Hint = '�O�̃N���b�V���O�E�|�C���g��'
      ImageIndex = 22
      OnExecute = BeforePrevCrossActionExecute
    end
    object BeforePrevFastAction: TAction
      Category = '�����N�O'
      Caption = '�O��20�X�e�b�v(&S)'
      Hint = '�O��20�X�e�b�v'
      ImageIndex = 23
      OnExecute = BeforePrevFastActionExecute
    end
    object BeforePrevStepAction: TAction
      Category = '�����N�O'
      Caption = '�O��1�X�e�b�v(&D)'
      Hint = '�O��1�X�e�b�v'
      ImageIndex = 21
      OnExecute = BeforePrevStepActionExecute
    end
    object BeforeNextStepAction: TAction
      Category = '�����N�O'
      Caption = '����1�X�e�b�v(&F)'
      Hint = '����1�X�e�b�v'
      ImageIndex = 18
      OnExecute = BeforeNextStepActionExecute
    end
    object BeforeNextFastAction: TAction
      Category = '�����N�O'
      Caption = '����20�X�e�b�v(&G)'
      Hint = '����20�X�e�b�v'
      ImageIndex = 20
      OnExecute = BeforeNextFastActionExecute
    end
    object BeforeNextCrossAction: TAction
      Category = '�����N�O'
      Caption = '���̃N���b�V���O�E�|�C���g��(&H)'
      Hint = '���̃N���b�V���O�E�|�C���g��'
      ImageIndex = 19
      OnExecute = BeforeNextCrossActionExecute
    end
    object AfterPrevCrossAction: TAction
      Category = '�����N��'
      Caption = '�O�̃N���b�V���O�E�|�C���g��(&Z)'
      Hint = '�O�̃N���b�V���O�E�|�C���g��'
      ImageIndex = 22
      OnExecute = AfterPrevCrossActionExecute
    end
    object AfterPrevFastAction: TAction
      Category = '�����N��'
      Caption = '�O��20�X�e�b�v(&X)'
      Hint = '�O��20�X�e�b�v'
      ImageIndex = 23
      OnExecute = AfterPrevFastActionExecute
    end
    object AfterPrevStepAction: TAction
      Category = '�����N��'
      Caption = '�O��1�X�e�b�v(&C)'
      Hint = '�O��1�X�e�b�v'
      ImageIndex = 21
      OnExecute = AfterPrevStepActionExecute
    end
    object AfterNextStepAction: TAction
      Category = '�����N��'
      Caption = '����1�X�e�b�v(&V)'
      Hint = '����1�X�e�b�v'
      ImageIndex = 18
      OnExecute = AfterNextStepActionExecute
    end
    object AfterNextFastAction: TAction
      Category = '�����N��'
      Caption = '����20�X�e�b�v(&B)'
      Hint = '����20�X�e�b�v'
      ImageIndex = 20
      OnExecute = AfterNextFastActionExecute
    end
    object AfterNextCrossAction: TAction
      Category = '�����N��'
      Caption = '���̃N���b�V���O�E�|�C���g��(&N)'
      Hint = '���̃N���b�V���O�E�|�C���g��'
      ImageIndex = 19
      OnExecute = AfterNextCrossActionExecute
    end
  end
  object ApplicationEvents: TApplicationEvents
    OnIdle = ApplicationEventsIdle
    Left = 488
    Top = 24
  end
  object WaveAreaPopupMenu: TPopupMenu
    Images = LoopTunerMainForm.ImageList
    Left = 520
    Top = 24
    object ZoomInMenuItem: TMenuItem
      Action = ZoomInAction
      ShortCut = 73
    end
    object ZoomOutMenuItem: TMenuItem
      Action = ZoomOutAction
      ShortCut = 79
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object PlayMenuItem: TMenuItem
      Caption = '�Đ�'
      Default = True
      ImageIndex = 24
      ShortCut = 32
      OnClick = PlayMenuItemClick
    end
    object StopPlayMenuItem: TMenuItem
      Action = StopPlayAction
      ShortCut = 83
    end
    object PlayHalfSecMenuItem: TMenuItem
      Action = PlayHalfSecAction
      ShortCut = 48
    end
    object Play1SecMenuItem: TMenuItem
      Action = Play1SecAction
      ShortCut = 49
    end
    object Play2SecMenuItem: TMenuItem
      Action = Play2SecAction
      ShortCut = 50
    end
    object Play3SecMenuItem: TMenuItem
      Action = Play3SecAction
      ShortCut = 51
    end
    object Play5SecMenuItem: TMenuItem
      Action = Play5SecAction
      ShortCut = 53
    end
    object BeforePrevCrossMenuItem: TMenuItem
      Action = BeforePrevCrossAction
      Break = mbBarBreak
      Caption = ' '
      ShortCut = 65
    end
    object BeforePrevFastMenuItem: TMenuItem
      Action = BeforePrevFastAction
      Caption = ' '
      ShortCut = 83
    end
    object BeforePrevSepMenuItem: TMenuItem
      Action = BeforePrevStepAction
      Caption = ' '
      ShortCut = 68
    end
    object BeforeNextStepMenuItem: TMenuItem
      Action = BeforeNextStepAction
      Caption = ' '
      ShortCut = 70
    end
    object BeforeNextFastMenuItem: TMenuItem
      Action = BeforeNextFastAction
      Caption = ' '
      ShortCut = 71
    end
    object BeforeNextCrossMenuItem: TMenuItem
      Action = BeforeNextCrossAction
      Caption = ' '
      ShortCut = 72
    end
    object AfterPrevCrossMenuItem: TMenuItem
      Action = AfterPrevCrossAction
      Break = mbBarBreak
      Caption = ' '
      ShortCut = 90
    end
    object AfterPrevFastMenuItem: TMenuItem
      Action = AfterPrevFastAction
      Caption = ' '
      ShortCut = 88
    end
    object AfterPrevSepMenuItem: TMenuItem
      Action = AfterPrevStepAction
      Caption = ' '
      ShortCut = 67
    end
    object AfterNextStepMenuItem: TMenuItem
      Action = AfterNextStepAction
      Caption = ' '
      ShortCut = 86
    end
    object AfterNextFastMenuItem: TMenuItem
      Action = AfterNextFastAction
      Caption = ' '
      ShortCut = 66
    end
    object AfterNextCrossMenuItem: TMenuItem
      Action = AfterNextCrossAction
      Caption = ' '
      ShortCut = 78
    end
  end
end