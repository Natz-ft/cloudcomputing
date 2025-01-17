#pragma once

#define WM_MULTI_PROCESS_CREATE_NEW_PAGE			(WM_USER + 0x4000)

#define WM_GLOBAL_CREATE_NEW_WEB_PAGE				(WM_USER + 0x4001)
#define WM_MULTI_PROCESS_NOTIFY_AXUI_CREATED		(WM_USER + 0x4002)
#define WM_FAV_BANK_CHANGE							(WM_USER + 0x400A) // gao 当用户收藏银行时
#define WM_FAV_BANK_DOWNLOAD						(WM_USER + 0x400B) // gao 用户对收藏银行控件进行下载
#define WM_MY_MENU_CLICKED							(WM_USER + 0x400C) // gao 用户点击了自定义菜单
#define WM_GLOBAL_GET_EXIST_WEB_PAGE				(WM_USER + 0x4014)

#define WM_MAINFRAME_CLOSE							(WM_USER + 0x4020)

#define WM_ITEM_NOTIFY_CREATED						(WM_USER + 0x4003)
#define WM_ITEM_SET_MAIN_TOOLBAR					(WM_USER + 0x4004)
#define WM_ITEM_SET_STATUS							(WM_USER + 0x4005)
#define WM_ITEM_SET_TAB_TEXT						(WM_USER + 0x4006)
#define WM_ITEM_SET_TAB_URL							(WM_USER + 0x4007)
#define WM_ITEM_GET_AX_CONTROL_WND					(WM_USER + 0x4008)
#define WM_ITEM_CHECK_URL_REQUIRE					(WM_USER + 0x4009)
#define WM_ITEM_SET_SSL_STATE						(WM_USER + 0x4010)
#define WM_ITEM_AUTOCLOSE							(WM_USER + 0x4011)
#define WM_ITEM_SET_PAGE_PROGRESS					(WM_USER + 0x4012)
#define WM_ITEM_TOGGLE_CATECTRL						(WM_USER + 0x4013)

#define WM_ITEM_ASK_SAVE_FAV						(WM_USER + 0x4040)
#define WM_ITEM_TEST_SAVE_FAV						(WM_USER + 0x4041)

#define WM_TAB_AUTOCLOSE							(WM_USER + 0x4020)
//	取消获取账单
#define WM_CANCEL_GET_BILL							(WM_USER + 0x4021)
#define WM_GETTING_BILL								(WM_USER + 0x4022)
// 结束获取账单
#define WM_FINISH_GET_BILL							(WM_USER + 0x4023)
#define WM_SET_DISPLAYHWND							(WM_USER + 0x4024)

#define WM_AX_NAVIGATE								(WM_USER + 0x5000)
#define WM_AX_GET_WEBBROWSER2_CROSS_THREAD			(WM_USER + 0x5001)

#define WM_AX_GOBACK								(WM_USER + 0x5002)
#define WM_AX_GOFORWARD								(WM_USER + 0x5003)
#define WM_AX_REFRESH								(WM_USER + 0x5004)
#define WM_AX_SSLSTATUS								(WM_USER + 0x5005)


#define WM_AX_FRAME_SETALARM						(WM_USER + 0x5006)
#define WM_AX_FRAME_ADDFAV							(WM_USER + 0x5007) // 添加收藏
#define WM_AX_FRAME_CHANGE_PROGRESS					(WM_USER + 0x5008) // 更新进度
#define WM_AX_TOOLS_CHANGE							(WM_USER + 0x5009) // 更新事件
#define WM_AX_COUPON_CHANGE							(WM_USER + 0x5010) // 更新优惠券
#define WM_AX_FRAME_DELETEFAV						(WM_USER + 0x5011) // 取消收藏

#define WM_CLOUDALARM								(WM_USER + 0x2100)
#define WM_CLOUDCLEAR								(WM_USER + 0x2101)
#define WM_CLOUDCHECK								(WM_USER + 0x2102)
#define WM_CLOUDNCHECK								(WM_USER + 0x2103)
#define WM_CLOUDNDESTORY							(WM_USER + 0x2104)
#define WM_RESTARTMONHUB							(WM_USER + 0x2105)

#define WM_CANCEL_ADDFAV							(WM_USER + 0x2140)// 取消收藏
#define WM_AX_GET_BILL								(WM_USER + 0x2141)
#define WM_AX_CLOSE_GET_BILL						(WM_USER + 0x2142)

#define WM_USB_ADD_FAV								(WM_USER + 0x2200)
#define WM_USB_CHANGE								(WM_USER + 0x2201)
#define WM_MONEYHUB_UAC								(WM_USER + 0x2204)

#define WM_MYDANGEROUS                              (WM_USER + 0x2202)//检测到危险，显示提示