/**
 *-----------------------------------------------------------*
 *  版权所有：  (c), 2010 - 2999, 北京融信恒通科技有限公司
 *    文件名：  main.cpp
 *      说明：  财金汇正式版客户端界面进程实现文件
 *    版本号：  1.0.0
 * 
 *  版本历史：
 *	版本号		日期	作者	说明
 *	1.0.0	2010.10.19	融信恒通	初始版本

 *  开发环境：
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
		// 在最开始需要进行初始化，否则可能会导致程序中的硬件id不一致.
		// 读取Hardware id
		InitHardwareId();

	}
	catch(CExceptionHandle eH)
	{
		g_strHardwareId = "NULL Can not get Hardware";
		eH.SetThreadName ("BKUI _tWinMain GetHardwareId Error");
		eH.RecordException ();
	}

	CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"读取了硬件ID");
	//判断gid的长度，如果不为32位，则补充为32位长度
	g_AppBranch.CheckHID();

	// 检测访问自身进程权限的句柄，如果限制访问，退出
	if(!g_AppBranch.CheckToken())
	{
		CRecordProgram::GetInstance()->FeedbackError(MY_PRO_NAME, MY_COMMON_ERROR, L"CheckToken错误");
		return 0;
	}
	if(wcslen(lpstrCmdLine) > 0)
	{
		WCHAR sLine[256] = {0};
		swprintf(sLine, 256, L"输入参数%s",lpstrCmdLine);
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, sLine);
	}

	if(_tcsncmp(lpstrCmdLine, _T("-renamechk"), 10) == 0)
	{
		g_AppBranch.RenameChk();
		return 0;
	}

	//注释掉该部分的支持，改为反馈处理，时间设置为5s超时
	//if (_tcsncmp(lpstrCmdLine, _T("-ifeedback"), 10) == 0)
	//{
	//	// 如果本地没有SN，生成一个SN并写入注册表，否则返回原有的SN
	//	CSNManager::GetInstance()->GetSN();

	//	//CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!这个放在这里，创建反馈线程，否则有些消息反馈不回去
	//	CUserBehavior::GetInstance()->Action_Install(1);

	//	CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"安装反馈");
	//	//g_AppBranch.Install();
	//	//CUserBehavior::GetInstance()->CloseFeedBack();

	//	// 安装并启动驱动
	//	return 0;
	//}
	//else if (_tcsncmp(lpstrCmdLine, _T("-dfeedback"), 10) == 0)
	//{
	//	// 发送卸载程序的数据到服务器
	//	CUserBehavior::GetInstance()->Action_Uninstall(1);
	//	//CUserBehavior::GetInstance()->BeginFeedBack();
	//	CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"卸载驱动");
	//	// 卸载驱动
	//	//g_AppBranch.UnInstall();
	//	//CUserBehavior::GetInstance()->CloseFeedBack();
	//	return 0;
	//}


	setlocale(LC_ALL, "CHS");
	// 初始化
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
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"启动了UAC");
		// 提升权限，进行驱动安装
		CListManager::Initialize(true);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 参数解析
	if (_tcsncmp(lpstrCmdLine, _T("-wait"), 5) == 0)
	{
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"延迟启动");
		Sleep(5000);//5s后重启财金汇
	}
	// IE内核进程
	else if (_tcsncmp(lpstrCmdLine, _T("-! "), 3) == 0)
	{
		// 启动内核进程
		CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!这个放在这里，创建反馈线程，否则有些消息反馈不回去
		CHostContainer::GetInstance()->GetHostName(kFeedback);
		g_AppBranch.RunIECoreProcess(lpstrCmdLine);
		return 0;
	}

	// 安装驱动
	if (_tcsncmp(lpstrCmdLine, _T("-i"), 2) == 0)
	{
		// 如果本地没有SN，生成一个SN并写入注册表，否则返回原有的SN
		CSNManager::GetInstance()->GetSN();

		CUserBehavior::GetInstance()->Action_Install(1);
		//CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!这个放在这里，创建反馈线程，否则有些消息反馈不回去

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"安装驱动");
		g_AppBranch.Install();
		//CUserBehavior::GetInstance()->CloseFeedBack();

		// 安装并启动驱动
		return 0;
	}
	else if (_tcsncmp(lpstrCmdLine, _T("-upi"), 4) == 0)
	{
		// 如果本地没有SN，生成一个SN并写入注册表，否则返回原有的SN
		CSNManager::GetInstance()->GetSN();

		//CBankData::GetInstance()->UpdateDB();//新版本的安装要升级旧版本的数据库

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"升级安装驱动");
		// 安装并启动驱动
		return g_AppBranch.Install();
	}
	// 卸载程序
	else if (_tcsncmp(lpstrCmdLine, _T("-d"), 2) == 0)
	{
		// 发送卸载程序的数据到服务器
		CUserBehavior::GetInstance()->Action_Uninstall(1);
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"卸载驱动");
		// 卸载驱动
		g_AppBranch.UnInstall();
		TerminateProcess(::GetCurrentProcess(), 0);
		return 0;
	}
	else if (_tcsncmp(lpstrCmdLine, _T("-upd"), 4) == 0 && wcslen(lpstrCmdLine) == 4)
	{
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"升级卸载驱动");
		// 卸载驱动
		return g_AppBranch.UnInstall();
	}
	// 清除用户信息，在内核进程中被调用
	else if (_tcsncmp(lpstrCmdLine, _T("-clean"), 6) == 0)
	{
		HANDLE hMutex = CreateMutexW(NULL, FALSE, L"UI_SECURITY_MUTEX");

		// 根据操作系统的版本来清除历史记录
		CleanHistoryMain();

		//if( hMutex )
			//::CloseHandle(hMutex);
		
		return 0;
	}

	// pop 调用，用来更改显示页
	if (_tcsncmp(lpstrCmdLine, _T("-agent"), 6) == 0)
	{
		wstring mail = lpstrCmdLine;
		mail = mail.substr(6, wstring::npos);
		if(g_AppBranch.PopSetPage(mail))
			return 0;
		CUserBehavior::GetInstance()->Action_ProgramStartup(kPopupWin);//Svr启动反馈
		// 错误代码写入到本地日志中
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"Svr启动财金汇");
	}
		//RunUAC();
	//重新生成黑名单
	//add by bh 2011 3 20 保护UI
	if (_tcsncmp(lpstrCmdLine, _T("-ncheck"), 7) != 0 &&  _tcsncmp(lpstrCmdLine, _T("-verchange"), 10) != 0)
	{
		g_AppBranch.GenerationBlackCache();
		//黑白名单由pop发送到驱动，故把该检测pop是否启动的位置提到最前，这样做才能解决UI进程被有道注入的问题
		if(!g_AppBranch.CheckPop(false)) // 检测Pop是否在运行中,如果pop没有运行那么就启动泡泡
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

		// 初始化GDI对象
		Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		// 初始化CRichEditCtrl对象，并对其进行挂钩
		::AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);
		::LoadLibrary(CRichEditCtrl::GetLibraryName());

		DoPatchRichEditHook();

		// 初始化皮肤显示
		CSkinLoader::Initialize();
		ThreadCacheDC::CreateThreadCacheDC();
		new CSkinManager();
		g_pSkin->RefreshAllSkins(false);

		// 安装时的安全检测，由于需要资源，所以要放在这个位置
		if (_tcsncmp(lpstrCmdLine, _T("-ncheck"), 7) == 0)
		{			
			g_AppBranch.CloseHandle();
			return  g_AppBranch.InstallCheck();
		}

		INT64 tBase = 129383136000000000;//s2，100纳秒为单位，格林尼治时间2011.1.1号和1601年之间的差值
		FILETIME tCurrent;
		GetSystemTimeAsFileTime(&tCurrent);
		INT64 tIntCur = tCurrent.dwHighDateTime * 0x100000000 + tCurrent.dwLowDateTime;//
		if(tIntCur < tBase)
		{
			mhMessageBox(NULL,L"检测到您的系统时间异常，您将无法使用财金汇，请修改您的系统时间后再运行财金汇",L"财金汇",MB_OK);
			return 1;		
		}

		if (_tcsncmp(lpstrCmdLine, _T("-verchange"), 10) == 0 && wcslen(lpstrCmdLine) == 10)
		{
			// 如果本地没有SN，生成一个SN并写入注册表，否则返回原有的SN
			CBankData::GetInstance()->InstallUpdateDB();//升级数据库

			// 安装并启动驱动
			return 0;
		}
		// 检查是否需要安装升级包
		if(!g_AppBranch.UpdateCheck())
		{
			g_AppBranch.CloseHandle();
			return 0;
		}
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"检查是否需要安装升级包");

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// gao 2010-12-16 将listmanager读取内核和UI数据分开
		CListManager::Initialize(false);
		CHostContainer::GetInstance()->GetHostName(kFeedback);
		CBankData::GetInstance()->CloseDB();
		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"初始化ListManager");		// 初始化全局变量
		// 等待IE内核进程退出
		int testtime = 0;
		while(g_AppBranch.CheckIECoreProcess() == true)
		{
			testtime ++;
			if(testtime > 15)
			{
				mhMessageBox(NULL,L"财金汇运行异常，请稍等后重新启动财金汇",L"财金汇检测",MB_OK);
				CRecordProgram::GetInstance()->FeedbackError(MY_PRO_NAME, MY_COMMON_ERROR, L"内核运行异常，关闭内核");

				// 这里应该增加关闭另一个进程的函数。
				//::TerminateProcess();
				g_AppBranch.TerminateIECore();
				g_AppBranch.CloseHandle();
				return 0;			
			}

			Sleep(2000);
		}

		//////////////////////////////////////////////////////////////////////////
		// Main Frame

		// 显示界面
		int nShowWindow = SW_SHOW;
		RECT rcWnd;
		DWORD dwMax = 0;
		//获得frame的大小和显示状态
		g_AppBranch.GetFramePos(nShowWindow,rcWnd,dwMax);

		// 安装时用于找窗口用的空窗口
		CSignalWnd* pSignalWnd = new CSignalWnd;
		HWND hSignalWnd = pSignalWnd->Create(NULL, CWindow::rcDefault);
	


#ifndef SINGLE_PROCESS
		// 安全检测
		if(!g_AppBranch.SecurityCheck())
		{
			g_AppBranch.CloseHandle();
			return 0;
		}

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"通过安全检查");

#endif
		CUserBehavior::GetInstance()->BeginFeedBack();//!!!!!这个放在这里，创建反馈线程，否则有些消息反馈不回去
		//////////////////////////////////////////////////////////////////////////
		// 用户行为反馈(启动)
		if (_tcsncmp(lpstrCmdLine, _T(""), 1) == 0)
		{
			CUserBehavior::GetInstance()->Action_ProgramStartup(kDesktop);

			// 在本地记录错误日志
			CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"用户启动程序");
		}


		//////////////////////////////////////////////////////////////////////////

		g_AppBranch.RefreshCurrency();
#ifndef SINGLE_PROCESS
		// 开启定时检测挂钩和注册表的函数
		g_AppBranch.StartMonitor();
#endif

		//////////////////////////////////////////////////////////////////////////

		// 创建亲显示UI框架
		CMainFrame *pFrame = new CMainFrame();
		dwMax = 1;//启动最大化
		HWND hFrame = pFrame->Create(NULL, rcWnd, _T("财金汇"), (dwMax ? WS_MAXIMIZE : 0) | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"财金汇界面启动");

		pFrame->ShowWindow(nShowWindow);
		pFrame->UpdateWindow();

		// 创建ie内核进程
		CProcessManager::_()->CreateProcess(pFrame->m_hWnd);

		//RunUAC();
		HOOKKEY::disablePrintKey();
	
 		HOOKKEY::installHook();// 安装键盘钩子
 		HOOKKEY::addNPB();

		//控制显示向导
		//g_AppBranch.CheckGuide(hFrame);去掉该显示功能

		CRecordProgram::GetInstance()->RecordCommonInfo(MY_PRO_NAME, MY_COMMON_PROCESS, L"财金汇界面消息");


		//消息循环
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
		// 用户行为反馈(退出)


		CUserBehavior::GetInstance()->Action_ProgramExit();
		CUserBehavior::GetInstance()->CloseFeedBack();//关闭反馈线程
		// 在本地记录错误日志
		// 反馈过滤名单
		//////////////////////////////////////////////////////////////////////////

#ifndef SINGLE_PROCESS
		// 关闭检测
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
