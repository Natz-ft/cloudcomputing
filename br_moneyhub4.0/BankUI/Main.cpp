/**
 *-----------------------------------------------------------*
 *  ��Ȩ���У�  (c), 2010 - 2999, �������ź�ͨ�Ƽ����޹�˾
 *    �ļ�����  main.cpp
 *      ˵����  �ƽ����ʽ��ͻ��˽������ʵ���ļ�
 *    �汾�ţ�  1.0.0
 * 
 *  �汾��ʷ��
 *	�汾��		����	����	˵��
 *	1.0.0	2010.10.19	���ź�ͨ	��ʼ�汾

 *  ����������
 *  Visual Studio 2008
 *-----------------------------------------------------------*
 */
#include "stdafx.h"

#include "Util.h"
#include "Version.h"


#include "UIControl/BaseClass/PathRichEdit.h"
#include "UIControl/MainFrame.h"
#include "UIControl/SignalWnd.h"
#include "UIControl/SecuCheckDlg.h"
#include "UIControl/AuthenDlg.h"

#include "Util/Util.h"
#include "Util/ProcessManager.h"
#include "Util/Config.h"

#include "Util/CleanHistory.h"

#include "MainBranch.h"
#include "HookKeyboard.h"

#include "../Utils/HardwareID/genhwid.h"
#include "../Utils/ListManager/ResourceManager.h"
#include "../Utils/UserBehavior/UserBehavior.h"
#include "../Utils/Config/HostConfig.h"
#include "../Utils/sn/SNManager.h"
#include "../Utils/ExceptionHandle/ExceptionHandle.h"


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	CExceptionHandle::MapSEtoCE();
	try
	{
		// ���ʼ��Ҫ���г�ʼ����������ܻᵼ�³����е�Ӳ��id��һ��.
		// ��ȡHardware id
		InitHardwareId();

	}
	catch(CExceptionHandle eH)
	{
		g_strHardwareId = "NULL Can not get Hardware";
		eH.SetThreadName ("BKUI _tWinMain GetHardwareId Error");
		eH.RecordException ();
	}

	CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"��ȡ��Ӳ��ID");
	//�ж�gid�ĳ��ȣ������Ϊ32λ���򲹳�Ϊ32λ����
	g_AppBranch.CheckHID();

	// ��������������Ȩ�޵ľ����������Ʒ��ʣ��˳�
	if(!g_AppBranch.CheckToken())
	{
		CRecordProgram::GetInstance()->FeedbackError(MY_PRO_NAME, MY_COMMON_ERROR, L"CheckToken����");
		return 0;
	}
	if(wcslen(lpstrCmdLine) > 0)
	{
		WCHAR sLine[256] = {0};
		swprintf(sLine, 256, L"�������%s",lpstrCmdLine);
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, sLine);
	}

	if(_tcsncmp(lpstrCmdLine, _T("-renamechk"), 10) == 0)
	{
		g_AppBranch.RenameChk();
		return 0;
	}

	//ע�͵��ò��ֵ�֧�֣���Ϊ����������ʱ������Ϊ5s��ʱ
	//if (_tcsncmp(lpstrCmdLine, _T("-ifeedback"), 10) == 0)
	//{
	//	// �������û��SN������һ��SN��д��ע��������򷵻�ԭ�е�SN
	//	CSNManager::GetInstance()->GetSN();

	//	//CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!�������������������̣߳�������Щ��Ϣ��������ȥ
	//	CUserBehavior::GetInstance()->Action_Install(1);

	//	CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"��װ����");
	//	//g_AppBranch.Install();
	//	//CUserBehavior::GetInstance()->CloseFeedBack();

	//	// ��װ����������
	//	return 0;
	//}
	//else if (_tcsncmp(lpstrCmdLine, _T("-dfeedback"), 10) == 0)
	//{
	//	// ����ж�س�������ݵ�������
	//	CUserBehavior::GetInstance()->Action_Uninstall(1);
	//	//CUserBehavior::GetInstance()->BeginFeedBack();
	//	CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"ж������");
	//	// ж������
	//	//g_AppBranch.UnInstall();
	//	//CUserBehavior::GetInstance()->CloseFeedBack();
	//	return 0;
	//}


	setlocale(LC_ALL, "CHS");
	// ��ʼ��
	CResourceManager::Initialize();
	if(_tcsncmp(lpstrCmdLine, _T("-startuac "), 10) == 0)
	{
		g_AppBranch.RunUAC(lpstrCmdLine);
		return 0;
	}

	if(_tcsncmp(lpstrCmdLine, _T("-ShellExecute "), 14) == 0)
	{
		g_AppBranch.Shell(lpstrCmdLine);
		return 0;
	}

	if(_tcsncmp(lpstrCmdLine, _T("-uac"), 4) == 0)
	{
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"������UAC");
		// ����Ȩ�ޣ�����������װ
		CListManager::Initialize(true);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��������
	if (_tcsncmp(lpstrCmdLine, _T("-wait"), 5) == 0)
	{
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"�ӳ�����");
		Sleep(5000);//5s�������ƽ��
	}
	// IE�ں˽���
	else if (_tcsncmp(lpstrCmdLine, _T("-! "), 3) == 0)
	{
		// �����ں˽���
		CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!�������������������̣߳�������Щ��Ϣ��������ȥ
		CHostContainer::GetInstance()->GetHostName(kFeedback);
		g_AppBranch.RunIECoreProcess(lpstrCmdLine);
		return 0;
	}

	// ��װ����
	if (_tcsncmp(lpstrCmdLine, _T("-i"), 2) == 0)
	{
		// �������û��SN������һ��SN��д��ע��������򷵻�ԭ�е�SN
		CSNManager::GetInstance()->GetSN();

		CUserBehavior::GetInstance()->Action_Install(1);
		//CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!�������������������̣߳�������Щ��Ϣ��������ȥ

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"��װ����");
		g_AppBranch.Install();
		//CUserBehavior::GetInstance()->CloseFeedBack();

		// ��װ����������
		return 0;
	}
	else if (_tcsncmp(lpstrCmdLine, _T("-upi"), 4) == 0)
	{
		// �������û��SN������һ��SN��д��ע��������򷵻�ԭ�е�SN
		CSNManager::GetInstance()->GetSN();

		//CBankData::GetInstance()->UpdateDB();//�°汾�İ�װҪ�����ɰ汾�����ݿ�

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"������װ����");
		// ��װ����������
		return g_AppBranch.Install();
	}
	// ж�س���
	else if (_tcsncmp(lpstrCmdLine, _T("-d"), 2) == 0)
	{
		// ����ж�س�������ݵ�������
		CUserBehavior::GetInstance()->Action_Uninstall(1);
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"ж������");
		// ж������
		g_AppBranch.UnInstall();
		TerminateProcess(::GetCurrentProcess(), 0);
		return 0;
	}
	else if (_tcsncmp(lpstrCmdLine, _T("-upd"), 4) == 0 && wcslen(lpstrCmdLine) == 4)
	{
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"����ж������");
		// ж������
		return g_AppBranch.UnInstall();
	}
	// ����û���Ϣ�����ں˽����б�����
	else if (_tcsncmp(lpstrCmdLine, _T("-clean"), 6) == 0)
	{
		HANDLE hMutex = CreateMutexW(NULL, FALSE, L"UI_SECURITY_MUTEX");

		// ���ݲ���ϵͳ�İ汾�������ʷ��¼
		CleanHistoryMain();

		//if( hMutex )
			//::CloseHandle(hMutex);
		
		return 0;
	}

	// pop ���ã�����������ʾҳ
	if (_tcsncmp(lpstrCmdLine, _T("-agent"), 6) == 0)
	{
		wstring mail = lpstrCmdLine;
		mail = mail.substr(6, wstring::npos);
		if(g_AppBranch.PopSetPage(mail))
			return 0;
		CUserBehavior::GetInstance()->Action_ProgramStartup(kPopupWin);//Svr��������
		// �������д�뵽������־��
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"Svr�����ƽ��");
	}
		//RunUAC();
	//�������ɺ�����
	//add by bh 2011 3 20 ����UI
	if (_tcsncmp(lpstrCmdLine, _T("-ncheck"), 7) != 0 &&  _tcsncmp(lpstrCmdLine, _T("-verchange"), 10) != 0)
	{
		g_AppBranch.GenerationBlackCache();
		//�ڰ�������pop���͵��������ʰѸü��pop�Ƿ�������λ���ᵽ��ǰ�����������ܽ��UI���̱��е�ע�������
		if(!g_AppBranch.CheckPop(false)) // ���Pop�Ƿ���������,���popû��������ô����������
		{
			g_AppBranch.CloseHandle();
			return 0;
		}
	}

	if (g_AppBranch.IsAlreadyRunning())
	{
		return 0;
	}

	if (_tcsncmp(lpstrCmdLine, _T("-ncheck"), 7) != 0 &&  _tcsncmp(lpstrCmdLine, _T("-verchange"), 10) != 0)
	{
		CProcessManager::_()->SetFilterId( 0 );
		CProcessManager::_()->SetFilterId( (UINT32)GetCurrentProcessId() );
	}
	
		g_strSkinDir = ::GetModulePath();
		g_strSkinDir += _T("\\Skin\\");

		::OleInitialize(NULL);

		// ��ʼ��GDI����
		Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		// ��ʼ��CRichEditCtrl���󣬲�������йҹ�
		::AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);
		::LoadLibrary(CRichEditCtrl::GetLibraryName());

		DoPatchRichEditHook();

		// ��ʼ��Ƥ����ʾ
		CSkinLoader::Initialize();
		ThreadCacheDC::CreateThreadCacheDC();
		new CSkinManager();
		g_pSkin->RefreshAllSkins(false);

		// ��װʱ�İ�ȫ��⣬������Ҫ��Դ������Ҫ�������λ��
		if (_tcsncmp(lpstrCmdLine, _T("-ncheck"), 7) == 0)
		{			
			g_AppBranch.CloseHandle();
			return  g_AppBranch.InstallCheck();
		}

		INT64 tBase = 129383136000000000;//s2��100����Ϊ��λ����������ʱ��2011.1.1�ź�1601��֮��Ĳ�ֵ
		FILETIME tCurrent;
		GetSystemTimeAsFileTime(&tCurrent);
		INT64 tIntCur = tCurrent.dwHighDateTime * 0x100000000 + tCurrent.dwLowDateTime;//
		if(tIntCur < tBase)
		{
			mhMessageBox(NULL,L"��⵽����ϵͳʱ���쳣�������޷�ʹ�òƽ�㣬���޸�����ϵͳʱ��������вƽ��",L"�ƽ��",MB_OK);
			return 1;		
		}

		if (_tcsncmp(lpstrCmdLine, _T("-verchange"), 10) == 0 && wcslen(lpstrCmdLine) == 10)
		{
			// �������û��SN������һ��SN��д��ע��������򷵻�ԭ�е�SN
			CBankData::GetInstance()->InstallUpdateDB();//�������ݿ�

			// ��װ����������
			return 0;
		}
		// ����Ƿ���Ҫ��װ������
		if(!g_AppBranch.UpdateCheck())
		{
			g_AppBranch.CloseHandle();
			return 0;
		}
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"����Ƿ���Ҫ��װ������");

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// gao 2010-12-16 ��listmanager��ȡ�ں˺�UI���ݷֿ�
		CListManager::Initialize(false);
		CHostContainer::GetInstance()->GetHostName(kFeedback);
		CBankData::GetInstance()->CloseDB();
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"��ʼ��ListManager");		// ��ʼ��ȫ�ֱ���
		// �ȴ�IE�ں˽����˳�
		int testtime = 0;
		while(g_AppBranch.CheckIECoreProcess() == true)
		{
			testtime ++;
			if(testtime > 15)
			{
				mhMessageBox(NULL,L"�ƽ�������쳣�����ԵȺ����������ƽ��",L"�ƽ����",MB_OK);
				CRecordProgram::GetInstance()->FeedbackError(MY_PRO_NAME, MY_COMMON_ERROR, L"�ں������쳣���ر��ں�");

				// ����Ӧ�����ӹر���һ�����̵ĺ�����
				//::TerminateProcess();
				g_AppBranch.TerminateIECore();
				g_AppBranch.CloseHandle();
				return 0;			
			}

			Sleep(2000);
		}

		//////////////////////////////////////////////////////////////////////////
		// Main Frame

		// ��ʾ����
		int nShowWindow = SW_SHOW;
		RECT rcWnd;
		DWORD dwMax = 0;
		//���frame�Ĵ�С����ʾ״̬
		g_AppBranch.GetFramePos(nShowWindow,rcWnd,dwMax);

		// ��װʱ�����Ҵ����õĿմ���
		CSignalWnd* pSignalWnd = new CSignalWnd;
		HWND hSignalWnd = pSignalWnd->Create(NULL, CWindow::rcDefault);
	


#ifndef SINGLE_PROCESS
		// ��ȫ���
		if(!g_AppBranch.SecurityCheck())
		{
			g_AppBranch.CloseHandle();
			return 0;
		}

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"ͨ����ȫ���");

#endif
		CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!�������������������̣߳�������Щ��Ϣ��������ȥ
		//////////////////////////////////////////////////////////////////////////
		// �û���Ϊ����(����)
		if (_tcsncmp(lpstrCmdLine, _T(""), 1) == 0)
		{
			CUserBehavior::GetInstance()->Action_ProgramStartup(kDesktop);

			// �ڱ��ؼ�¼������־
			CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"�û���������");
		}


		//////////////////////////////////////////////////////////////////////////

		g_AppBranch.RefreshCurrency();
#ifndef SINGLE_PROCESS
		// ������ʱ���ҹ���ע����ĺ���
		g_AppBranch.StartMonitor();
#endif

		//////////////////////////////////////////////////////////////////////////

		// ��������ʾUI���
		CMainFrame *pFrame = new CMainFrame();
		dwMax = 1;//�������
		HWND hFrame = pFrame->Create(NULL, rcWnd, _T("�ƽ��"), (dwMax ? WS_MAXIMIZE : 0) | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"�ƽ���������");

		pFrame->ShowWindow(nShowWindow);
		pFrame->UpdateWindow();

		// ����ie�ں˽���
		CProcessManager::_()->CreateProcess(pFrame->m_hWnd);

		//RunUAC();
		HOOKKEY::disablePrintKey();
	
 		HOOKKEY::installHook();// ��װ���̹���
 		HOOKKEY::addNPB();

		//������ʾ��
		//g_AppBranch.CheckGuide(hFrame);ȥ������ʾ����

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"�ƽ�������Ϣ");


		//��Ϣѭ��
		MSG msg;
		while (::GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message == WM_KEYDOWN && msg.wParam == VK_TAB)
			{
				if (GetKeyState(VK_CONTROL) & 0x8000)
				{
					pFrame->FS()->pCate->ToggleItem();
					continue;
				}
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
 			HOOKKEY::uninstallHook();
 			HOOKKEY::installHook();
		}

		//////////////////////////////////////////////////////////////////////////
		// �û���Ϊ����(�˳�)


		CUserBehavior::GetInstance()->Action_ProgramExit();
		CUserBehavior::GetInstance()->CloseFeedBack();//�رշ����߳�
		// �ڱ��ؼ�¼������־
		// ������������
		//////////////////////////////////////////////////////////////////////////

#ifndef SINGLE_PROCESS
		// �رռ��
		g_AppBranch.StopMonitor();
#endif

		CleanHistoryMain(FALSE);


	try
	{
		OleUninitialize( );
		HOOKKEY::EnablePrintKey();
		g_AppBranch.CloseHandle();
	}
	catch(CExceptionHandle eH)
	{
		eH.SetThreadName ("BKUI _tWinMain Thread Error");
		eH.RecordException ();
	}
	return 0;
}