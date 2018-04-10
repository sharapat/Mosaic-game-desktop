//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop
#include <algorithm>
#include "MainFormUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	CURRENT_DIR = ExtractFileDir(ParamStr(0));
	sFile = new TIniFile(CURRENT_DIR + "\\Data\\Settings.ini");
	SettingsCheck();
	srand(time(0));
	File = new TIniFile(CURRENT_DIR + "\\Data\\Data.ini");
	for(int i = 1; i <= 9; i ++)
	{
		for(int j = 1; j <= 13; j++)
		{
			ImagePath[i][j] = (TPath*)FindComponent("FreeSpace"+IntToStr(i)+"_"+IntToStr(j));
			FreePath[i][j] = (TPath*)FindComponent("Path"+IntToStr(i)+"_"+IntToStr(j));
			ImagePath[i][j]->AutoCapture = true;
			ImagePath[i][j]->Visible = true;
			FreePath[i][j]->Stroke->Color = claSilver;
			FreePath[i][j]->Stroke->Dash = TStrokeDash::Dash;
			FreePath[i][j]->Fill->Kind = TBrushKind::None;
			FreePath[i][j]->Visible = true;
			tagation[i][j] = ImagePath[i][j]->Tag;
		}
	}
	LevelsWindowCreate();
	//------------------------------
	/*f = new TIniFile(CURRENT_DIR + "\\File.ini");
	for(int i = 1; i <= 9; i++)
	{
		for(int j = 1; j <= 13; j++)
		{
			int H, W, X, Y;
			H = FreePath[i][j]->Height;
			W = FreePath[i][j]->Width;
			X = FreePath[i][j]->Position->X;
			Y = FreePath[i][j]->Position->Y;
			f->WriteInteger("Path" + IntToStr(i) + "_" + IntToStr(j), "Height", H);
			f->WriteInteger("Path" + IntToStr(i) + "_" + IntToStr(j), "Width", W);
			f->WriteInteger("Path" + IntToStr(i) + "_" + IntToStr(j), "PosX", X);
			f->WriteInteger("Path" + IntToStr(i) + "_" + IntToStr(j), "PosY", Y);
		}
	}   */

	
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LevelsWindowCreate()
{
	pX = 40, pY = 100;
	LevelCount = File->ReadInteger("Index", "Count", 0);
	int isLevelOpen;
	for(int i = 1; i <= 10; i++)
	{
		R[i]->Free();
		if(S[i] != NULL) S[i]->Free();
	}
	DinComCount = 0;
	for(int i = 1; i <= LevelCount; i++)
	{
		isLevelOpen = File->ReadInteger("Level"+IntToStr(i), "Open", 0);
		R[i] = new TRectangle(this);
		TShadowEffect* E = new TShadowEffect(this);
		R[i]->Parent = LevelsWindow;
		R[i]->Position->X = pX;
		R[i]->Position->Y = pY;
		R[i]->Name = "Level" + IntToStr(i);
		R[i]->Width = 225;
		R[i]->Height = 150;
		R[i]->Fill->Kind = TBrushKind::Bitmap;
		R[i]->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
		R[i]->Stroke->Thickness = 5;
		R[i]->Stroke->Color = claGray;
		R[i]->Stroke->Kind = TBrushKind::Solid;
		R[i]->Fill->Bitmap->Bitmap->LoadFromFile(CURRENT_DIR + "\\Images\\GameImages\\Level" + IntToStr(i) + "\\MainPicture.png");
		R[i]->Tag = i;
		R[i]->OnMouseEnter = LevelButtonOnMouseEnter;
		R[i]->OnMouseLeave = LevelButtonOnMouseLeave;
		R[i]->OnClick = LevelsClick;
		//R[i]->OnClick = LevelButtonClick;
		Effect = new TShadowEffect(this);
		Effect->Parent = R[i];
		Effect->ShadowColor = claBlack;
		if(isLevelOpen)
		{
			StarsCount = File->ReadInteger("Level"+IntToStr(i), "Stars", 0);
			if(StarsCount != 0)
			{
				S[i] = new TRectangle(this);
				S[i]->Parent = LevelsWindow;
				S[i]->Position->X = pX + 52.5;
				S[i]->Position->Y = pY + 160;
				S[i]->Height = 54;
				S[i]->Width = 120;
				S[i]->Fill->Kind = TBrushKind::Bitmap;
				S[i]->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
				S[i]->Fill->Bitmap->Bitmap->LoadFromFile(CURRENT_DIR + "\\Images\\"+IntToStr(StarsCount) + "_Stars.png");
				S[i]->Stroke->Kind = TBrushKind::None;
			}
			R[i]->Cursor = crHandPoint;
		}
		else
		{
			Lock = new TRectangle(this);
			Locked = new TRectangle(this);
			Lock->Parent = R[i];
			Lock->Align = TAlignLayout::Contents;
            Lock->Fill->Color = Copy->Fill->Color;
			Locked->Parent = Lock;
			Locked->Align = TAlignLayout::Contents;
			Locked->Stroke->Kind = TBrushKind::None;
			Locked->Fill->Kind = TBrushKind::Bitmap;
			Locked->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
			Locked->Fill->Bitmap->Bitmap->LoadFromFile(CURRENT_DIR + "\\Images\\lock.png");
			Locked->Margins->Right = 48.5;
			Locked->Margins->Left = 48.5;
			Locked->Margins->Top = 11;
			Locked->Margins->Bottom = 11;
		}
		pX += 265;
		if(pX > MainForm->Width - 265){
            pX = 40;
			pY += 250;
		}
		DinComCount++;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseButtonClick(TObject *Sender)
{
	MainForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PlayButtonMouseEnter(TObject *Sender)
{
	TRectangle *R = (TRectangle*)Sender;
	TColorAnimation *A = (TColorAnimation*)R->Children->Items[1]->Children->Items[0];
	A->Inverse = false;
	A->Start();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PlayButtonMouseLeave(TObject *Sender)
{
	TRectangle *R = (TRectangle*)Sender;
	TColorAnimation *A = (TColorAnimation*)R->Children->Items[1]->Children->Items[0];
	A->Inverse = true;
	A->Start();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FreeSpace1_1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	TPath* P = (TPath*)Sender;
	sIndex_i = P->Name[10];
	sIndex_j = P->Name[12];
	if(P->Name.Length() == 13) sIndex_j += P->Name[13];
	index_i = StrToInt(sIndex_i);
	index_j = StrToInt(sIndex_j);
	P->BringToFront();
	hX = X;
	hY = Y;
	isDown = Button==TMouseButton::mbLeft && !isExamplePictureLarge;

	for(int i = 1; i <= 9; i ++)
	{
		for(int j = 1; j <= 13; j++)
		{
			if(isBusy[IntToStr(i) + "_" + IntToStr(j)].first == index_i && isBusy[IntToStr(i) + "_" + IntToStr(j)].second == index_j)
			{
				isBusy[IntToStr(i) + "_" + IntToStr(j)].first = 0;
				isBusy[IntToStr(i) + "_" + IntToStr(j)].second = 0;
				if(i == index_i && j == index_j && P->RotationAngle == 0)
				{
					Win--;
				}
				return ;
			}

		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FreeSpace1_1MouseMove(TObject *Sender, TShiftState Shift,
          float X, float Y)
{
	TPath *P = (TPath*)Sender;
	sIndex_i = P->Name[10];
	sIndex_j = P->Name[12];
	if(P->Name.Length() == 13) sIndex_j += P->Name[13];
	index_i = StrToInt(sIndex_i);
	index_j = StrToInt(sIndex_j);
	if(isDown)
	{
		if(P->RotationAngle == 0)
		{
			P->Position->X += (X-hX);
			P->Position->Y += (Y-hY);
		}
		if(P->RotationAngle == 90)
		{
			P->Position->Y -= (hX-X);
			P->Position->X += (hY-Y);
		}
		if(P->RotationAngle == 180)
		{
			P->Position->Y -= (Y-hY);
			P->Position->X -= (X-hX);
		}
		if(P->RotationAngle == 270)
		{
			P->Position->Y += (hX-X);
			P->Position->X -= (hY-Y);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FreeSpace1_1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	TPath* P = (TPath*)Sender;
	int tag;
	tag = P->Tag;
	if(Button == TMouseButton::mbRight)
	{
		if(Sound)
		{
			MediaPlayer1->FileName = CURRENT_DIR + "\\Sounds\\letterclick.wav";
			MediaPlayer1->Play();
		}
		P->Tag = tag/10+tag%10*1000;
		if(P->RotationAngle == 270){
			P->RotationAngle = 0;
			goto end;
		}
		P->RotationAngle += 90;
	}
	end:;
	isDown = false;
	Accomodation(P);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PlayButtonClick(TObject *Sender)
{
	LevelsWindowCreate();
	FirstWindow->Visible = false;
	LevelsWindow->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LevelButtonOnMouseEnter(TObject *Sender)
{
	TRectangle* MyR = (TRectangle*)Sender;
	TShadowEffect* E = (TShadowEffect*)MyR->Children->Items[0];
	TFloatAnimation* A = new TFloatAnimation(this);
	TFloatAnimation* A1 = new TFloatAnimation(this);
	A1->Parent = MyR;
	A1->PropertyName = "Position.Y";
	A1->StartValue = (MyR->Tag-1)/5*250+100;
	A1->StopValue = (MyR->Tag-1)/5*250 + 95;
	A1->Duration = 0.1;
	A->Parent = E;
	A->Duration = 0.1;
	A->StartFromCurrent = true;
	A->StopValue = 0.6;
	A->PropertyName = "Softness";
	A->Start();
	A1->Start();
}
void __fastcall TMainForm::LevelButtonOnMouseLeave(TObject *Sender)
{
	TRectangle* MyR = (TRectangle*)Sender;
	TShadowEffect* E = (TShadowEffect*)MyR->Children->Items[0];
	TFloatAnimation* A = new TFloatAnimation(this);
	TFloatAnimation* A1 = new TFloatAnimation(this);
	A1->Parent = MyR;
	A1->PropertyName = "Position.Y";
	A1->StartFromCurrent = true;
	A1->StopValue = (MyR->Tag-1)/5*250 + 100;
	A1->Duration = 0.1;
	A->Parent = E;
	A->Duration = 0.1;
	A->StartFromCurrent = true;
	A->StopValue = 0.3;
	A->PropertyName = "Softness";
	A->Start();
	A1->Start();
}
void __fastcall TMainForm::Rectangle1Click(TObject *Sender)
{
	LevelsWindow->Visible = false;
	FirstWindow->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Accomodation(TObject *Sender)
{
	TPath *P = (TPath*)Sender;
	float a, b;
	int rot = P->RotationAngle;
	int tag;
	tag = P->Tag;
	sIndex_i = P->Name[10];
	sIndex_j = P->Name[12];
	if(P->Name.Length() == 13) sIndex_j += P->Name[13];
	index_i = StrToInt(sIndex_i);
	index_j = StrToInt(sIndex_j);
	PosX = P->Position->X;
	PosY = P->Position->Y;
	H = P->Height;
	W = P->Width;
	for(int i = 1; i <= 9; i++)
	{
		for(int j = 1; j <= 13; j++)
		{
			if(isBusy[IntToStr(i) + "_" + IntToStr(j)].first != 0)
			{
				continue;
			}
			a = fabs(FreePath[i][j]->Position->X + Space->Position->X - PosX);
			b = fabs(FreePath[i][j]->Position->Y + Space->Position->Y - PosY);
			if((rot/90)&1) a -= 10, b -= 10;
			if(a <= 20 && b <= 20 && FreePath[i][j]->Tag == tag)
			{
				if((rot/90)&1)
				{
					P->Position->X = FreePath[i][j]->Position->X + Space->Position->X - (W-H)/2;
					P->Position->Y = FreePath[i][j]->Position->Y + Space->Position->Y + (W-H)/2;
				}
				else{
					P->Position->X = FreePath[i][j]->Position->X + Space->Position->X;
					P->Position->Y = FreePath[i][j]->Position->Y + Space->Position->Y;
				}
			   //	isBusy[i][j] = true;
				if(Sound)
				{
					MediaPlayer1->FileName = CURRENT_DIR + "\\Sounds\\key.mp3";
					MediaPlayer1->Play();
				}
				isBusy[IntToStr(i) + "_" + IntToStr(j)].first = index_i;
				isBusy[IntToStr(i) + "_" + IntToStr(j)].second = index_j;
				if(index_i == i && index_j == j && rot == 0)
				{
					Win++;
					MainForm->Caption = IntToStr(Win);
				}
				//ShowMessage(IntToStr(k));
				isFull++;
				if(Win == 117)
				{
					Winner();
				}
//				if(isFull == 117)
//				{
//					//Check();
//                }
				//Check();
				return;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void __fastcall TMainForm::PlayGame(int level)
{
	Current_Level = level;
	TimerSekund = 0;
	TimerMinut = 0;
	TimerHour = 0;
	Win = 0;
	for(int i = 1; i <= 9; i ++)
	{
		for(int j = 1; j <= 13; j++)
		{
			int x,y, rot, tag,z;
			isBusy[IntToStr(i) + "_" + IntToStr(j)].first = 0;
			isBusy[IntToStr(i) + "_" + IntToStr(j)].second = 0;
			if(i <= 3)
			{
				x = rand()%210;
				y = rand()%670;
				ImagePath[i][j]->Position->X = x;
				ImagePath[i][j]->Position->Y = y;
			}
			else if(i > 3 && i < 7)
			{
				x = rand()%1150;
				y = rand()%70;
				ImagePath[i][j]->Position->X = x;
				ImagePath[i][j]->Position->Y = y;
			}
			else if(i >= 7){
				x = rand()%210;
				y = rand()%600;
			   	x = MainForm->Width - 60 -x;
				y += 70;
				ImagePath[i][j]->Position->X = x;
				ImagePath[i][j]->Position->Y = y;
			}
			ImagePath[i][j]->Tag = tagation[i][j];
			ImagePath[i][j]->Position->X = z ? x : MainForm->Width - x - 60;
			ImagePath[i][j]->Position->Y = y;
			ImagePath[i][j]->Stroke->Color = claSilver;
			ImagePath[i][j]->Stroke->Dash = TStrokeDash::Solid;
			ImagePath[i][j]->Fill->Kind = TBrushKind::Bitmap;
			ImagePath[i][j]->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
			ImagePath[i][j]->Fill->Bitmap->Bitmap->LoadFromFile(CURRENT_DIR+"\\Images\\GameImages\\Level" + IntToStr(Current_Level) + "\\Picture" + IntToStr(i) + "_" + IntToStr(j) + ".png");
			rot = rand()%4;
			ImagePath[i][j]->RotationAngle = rot*90;
			while(rot--)
			{
				tag = ImagePath[i][j]->Tag;
				ImagePath[i][j]->Tag = tag/10+tag%10*1000;
			}

		}
	}
	ExamplePicture->Fill->Bitmap->Bitmap->LoadFromFile(CURRENT_DIR+"\\Images\\GameImages\\Level"+ IntToStr(Current_Level) + "\\MainPicture.png");
	MediaPlayer1->Stop();
	StartTimeHour = StrToInt(Now().FormatString("hh"));
	StartTimeMinut = StrToInt(Now().FormatString("n"));
	StartTimeSekund = StrToInt(Now().FormatString("ss"));
	LevelsWindow->Visible = false;
	Timer1->Enabled = true;
	GameWindow->Visible = true;
}

void __fastcall TMainForm::SettingsButtonClick(TObject *Sender)
{
	SettingsLayout->Visible = true;
	BlackBg->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SettingsExitClick(TObject *Sender)
{
	Music = sFile->ReadInteger("Settings", "Music", 0);
	Sound = sFile->ReadInteger("Settings", "SoundEffect", 0);
	MusicCheckBox->IsChecked = Music;
	SoundEffectCheckBox->IsChecked = Sound;
	SettingsLayout->Visible = false;
	FirstWindow->Visible = true;
	BlackBg->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SettingsCheck()
{
	Music = sFile->ReadInteger("Settings", "Music", 0);
	Sound = sFile->ReadInteger("Settings", "SoundEffect", 0);
	MusicCheckBox->IsChecked = Music;
	SoundEffectCheckBox->IsChecked = Sound;
	PlayMusic(Music);
}

void __fastcall TMainForm::SaveSettingsClick(TObject *Sender)
{
	Music = (int)MusicCheckBox->IsChecked;
	Sound = (int)SoundEffectCheckBox->IsChecked;
	sFile->WriteInteger("Settings", "Music", Music);
	sFile->WriteInteger("Settings", "SoundEffect", Sound);
	SettingsCheck();
	SettingsLayout->Visible = false;
	BlackBg->Visible = false;
	FirstWindow->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PlayMusic(int Music)
{
	if(Music)
	{
    	MediaPlayer1->FileName = CURRENT_DIR + "\\Sounds\\easter.mp3";
		MediaPlayer1->Play();
	}
	else
	{
        MediaPlayer1->Stop();
    }
}



void __fastcall TMainForm::ClearHistoryLayoutClick(TObject *Sender)
{
	WarningBlackBg->Visible = true;
	WarningLayout->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WarningNoBtnClick(TObject *Sender)
{
	WarningLayout->Visible = false;
	WarningBlackBg->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WarningYesBtnClick(TObject *Sender)
{
	File->WriteInteger("Level1", "Open", 1);
	File->WriteInteger("Level1", "Stars", 0);
	File->WriteInteger("Level1", "Time", 1000000);
	File->WriteInteger("Level1", "Attempts", 1000000);
	int LevelCount = File->ReadInteger("index", "Count", 0);
	for(int i = 2; i <= LevelCount; i++)
	{
		File->WriteInteger("Level" + IntToStr(i), "Open", 0);
		File->WriteInteger("Level" + IntToStr(i), "Stars", 0);
		File->WriteInteger("Level" + IntToStr(i), "TimeHour", 1000000);
		File->WriteInteger("Level" + IntToStr(i), "Attempts", 1000000);
	}
	WarningNoBtnClick(NULL);
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::ClearHistoryLayoutMouseEnter(TObject *Sender)
{
	TLayout *R = (TLayout*)Sender;
	TFloatAnimation *A = (TFloatAnimation*)R->Children->Items[2]->Children->Items[0];
	A->Inverse = false;
	A->Start();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClearHistoryLayoutMouseLeave(TObject *Sender)
{
	TLayout *R = (TLayout*)Sender;
	TFloatAnimation *A = (TFloatAnimation*)R->Children->Items[2]->Children->Items[0];
	A->Inverse = true;
	A->Start();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Rectangle13Click(TObject *Sender)
{
	GameBlackBg->Visible = true;
	GameBlackBg->BringToFront();
	GameWarningLayout->BringToFront();
	GameWarningLayout->Visible = true;
	if(ExamplePictureWidthAnimation->Inverse)
		ExamplePictureOut->OnClick(ExamplePictureOut);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GameWarningNoBtnClick(TObject *Sender)
{
	GameBlackBg->Visible = false;
	GameWarningLayout->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GameWarningYesBtnClick(TObject *Sender)
{
	LevelsWindowCreate();
	SettingsCheck();
	GameWindow->Visible = false;
	GameWarningLayout->Visible = false;
	GameBlackBg->Visible = false;
	LevelsWindow->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Winner()
{
	StopTimeHour = StrToInt(Now().FormatString("hh"));
	StopTimeMinut = StrToInt(Now().FormatString("nn"));
	StopTimeSekund = StrToInt(Now().FormatString("ss"));
	Timer1->Enabled = false;
	int LastHour, LastMinut, LastSekund;
	LastHour = File->ReadInteger("Level" + IntToStr(Current_Level), "TimeHour", 10000);
	LastMinut = File->ReadInteger("Level" + IntToStr(Current_Level), "TimeMinut", 10000);
	LastSekund = File->ReadInteger("Level" + IntToStr(Current_Level), "TimeMinut", 10000);
	Hour = StrToInt(HourLabel->Text);
	Minut = StrToInt(MinutLabel->Text);
	Sekund = StrToInt(SekundLabel->Text);
	if(Hour < LastHour)
	{
		File->WriteInteger("Level" + IntToStr(Current_Level), "TimeHour", Hour);
		File->WriteInteger("Level" + IntToStr(Current_Level), "TimeMinut", Minut);
		File->WriteInteger("Level" + IntToStr(Current_Level), "TimeSekund", Sekund);
	}
	else if(Hour == LastHour)
	{
		if(Minut < LastMinut)
		{
			File->WriteInteger("Level" + IntToStr(Current_Level), "TimeHour", Hour);
			File->WriteInteger("Level" + IntToStr(Current_Level), "TimeMinut", Minut);
			File->WriteInteger("Level" + IntToStr(Current_Level), "TimeSekund", Sekund);
		}
		else if(Minut == LastMinut)
		{
			if(Sekund < LastSekund)
			{
				File->WriteInteger("Level" + IntToStr(Current_Level), "TimeHour", Hour);
				File->WriteInteger("Level" + IntToStr(Current_Level), "TimeMinut", Minut);
				File->WriteInteger("Level" + IntToStr(Current_Level), "TimeSekund", Sekund);
			}
		}
	}
	LastMinut = File->ReadInteger("Level" + IntToStr(Current_Level), "TimeMinut", 10000);
	if(Minut < 20) StarsCount = 3;
	else
	if(Minut < 40) StarsCount = 2;
	else StarsCount = 1;
	StarPicture->Fill->Bitmap->Bitmap->LoadFromFile(CURRENT_DIR + "\\Images\\" + IntToStr(StarsCount) + "_Stars.png");
	if(StarsCount > File->ReadInteger("Level" + IntToStr(Current_Level), "Stars", 0)) {
		File->WriteInteger("Level" + IntToStr(Current_Level), "Stars", StarsCount);
	}
	GameBlackBg->BringToFront();
	NextLevelLayout->BringToFront();
	//int StarCount = File->ReadInteger("Level" + IntToStr(Current_Level), "Stars", 0);
	//ShowMessage(IntToStr(Hour) + " " + IntToStr(Minut) + " " + IntToStr(Sekund));
	Win = 0;
	if(Sound)
	{
		MediaPlayer1->Volume = 50;
		MediaPlayer1->FileName = CURRENT_DIR + "\\Sounds\\winn.wav";
		MediaPlayer1->Play();
    }
	GameBlackBg->Visible = true;
	NextLevelLayout->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
	TimerSekund ++;

	if(TimerSekund == 60) TimerMinut ++, TimerSekund = 0;
	if(TimerMinut == 60) TimerHour++, TimerMinut = 0;

	if(TimerHour/10 == 0) HourLabel->Text = "0" + IntToStr(TimerHour);
	else HourLabel->Text = IntToStr(TimerHour);

	if(TimerMinut/10 == 0) MinutLabel->Text = "0" + IntToStr(TimerMinut);
	else MinutLabel->Text = IntToStr(TimerMinut);

	if(TimerSekund/10 == 0) SekundLabel->Text = "0" + IntToStr(TimerSekund);
	else SekundLabel->Text = IntToStr(TimerSekund);

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LevelsClick(TObject *Sender)
{
	TRectangle *R = (TRectangle*)Sender;
	String name = R->Name;
	Win = 0;
	int k = 1;
	Current_Level = 0;
	for(int i = name.Length(); i >= 6; i--)
	{
		Current_Level = StrToInt(name[i])*k;
		k *= 10;
	}
	PlayGame(Current_Level);
}

void __fastcall TMainForm::GoButtonClick(TObject *Sender)
{
	GameBlackBg->Visible = false;
	NextLevelLayout->Visible = false;
	File->WriteInteger("Level"+IntToStr(Current_Level+1), "Open", 1);
	LevelsWindowCreate();
	LevelsWindow->Visible = true;
	HourLabel->Text = "00";
	MinutLabel->Text = "00";
	SekundLabel->Text == "00";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	Win = 116;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FreeSpace1_1DblClick(TObject *Sender)
{
	TPath *P = (TPath*)Sender;
	MainForm->Caption = IntToStr(P->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormActivate(TObject *Sender)
{
	if(MainForm->Active){
		MainForm->FullScreen = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExamplePictureOutClick(TObject *Sender)
{
	isExamplePictureLarge = !ExamplePictureWidthAnimation->Inverse;
	ExamplePictureWidthAnimation->Start();
	ExamplePictureHeightAnimation->Start();
	ExamplePicturePositionXAnimation->Start();
	ExamplePicturePositionYAnimation->Start();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExamplePictureWidthAnimationFinish(TObject *Sender)
{
	ExamplePictureWidthAnimation->Inverse = !ExamplePictureWidthAnimation->Inverse;
	ExamplePicturePositionXAnimation->Inverse = !ExamplePicturePositionXAnimation->Inverse;
	ExamplePicturePositionYAnimation->Inverse = !ExamplePicturePositionYAnimation->Inverse;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExamplePictureHeightAnimationFinish(TObject *Sender)
{
	ExamplePictureHeightAnimation->Inverse = !ExamplePictureHeightAnimation->Inverse;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormResize(TObject *Sender)
{
	ExamplePicturePositionXAnimation->StopValue = Space->Position->X-10;
	ExamplePicturePositionXAnimation->StartValue = ExamplePictureOut->Position->X;
	ExamplePicturePositionYAnimation->StartValue = ExamplePictureOut->Position->Y;
	ExamplePicturePositionYAnimation->StopValue = Space->Position->Y-10;
	ExamplePictureWidthAnimation->StopValue = Space->Width + 20;
	ExamplePictureHeightAnimation->StopValue = Space->Height + 20;
}
//---------------------------------------------------------------------------

