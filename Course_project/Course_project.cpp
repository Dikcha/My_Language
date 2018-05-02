
#include "stdafx.h"
#include <iostream>
#include <locale>
#include <cwchar>

#include "Rules.h"
#include "Error.h"
#include "Parm.h"

#include "In.h"
#include "Log.h"
#include "LexicalAnalyse.h"
#include "MFST.h"
#include "MFST_TRACE.h"
#include "Polish_Notation.h"
#include "SemanticAnalyse.h"
#include "CodeGeneration.h"
#define FILE_PATH_ASM "D:/�����/2 ����/��/ready_proj/Course_project/ASM/ASM.asm"

int _tmain(int argc, _TCHAR* argv[])
{
	
	setlocale(LC_ALL, "");
	bool Param_Ok = false;
	Log::LOG log = Log::INITLOG;
	try
	{
		
		Parm::PARM parm = Parm::getparm(argc, argv);
		Param_Ok = true;
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "����:", "��� ������ ", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);

		//���� ������������ �������
		In::ArrWord arr(4096);
		arr.Split((char*)in.text);
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);

		if (!Lex::LexicalAnalyse(arr, lextable, idtable, log))
		{



			//���� ��������������� �������
			MFST::Mfst mfst(lextable, GRB::greibach);

			if (mfst.start(log))
			{
				mfst.printrules(log);
				/*GRB::Print_Rules(GRB::greibach);*/
			    
				//���� �������������� �������
				if (!SA::SemanticAnalyse(log, lextable, idtable))
				{
					


					//���������
					CG::ConstSegment CnstS(idtable.size);
					CG::DataSegment Data(idtable.size);
					CG::CodeSegment Code;
					CG::add(idtable, CnstS);
					CG::add(idtable, Data);
					CG::CodeSegmentGeneration(Code, lextable, idtable);
					CG::startGeneration(FILE_PATH_ASM, CnstS, Data, Code);

				}

			}
		}
	

	}
	catch (Error::ERROR e)
	{
		if (e.id == 100)
			std::cout << "�������� -in ������ ���� ����� �����������!\n";
		else if(e.id==104)
			std::cout << "��������� ����� �������� ���������\n";
		else
		{
			
			Log::WriteError(log, e);
		}

	}


	if (Param_Ok)
		Log::Close(log);
	return 0;
}
