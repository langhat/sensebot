import ctypes
from ctypes import wintypes

# 兼容 Windows XP/Vista+ 的 NOTIFYICONDATA 结构（不依赖 GUID）
class NOTIFYICONDATA(ctypes.Structure):
    _fields_ = [
        ('cbSize', wintypes.DWORD),
        ('hWnd', wintypes.HWND),
        ('uID', wintypes.UINT),
        ('uFlags', wintypes.UINT),
        ('uCallbackMessage', wintypes.UINT),
        ('hIcon', wintypes.HICON),
        ('szTip', wintypes.WCHAR * 128),
        ('dwState', wintypes.DWORD),
        ('dwStateMask', wintypes.DWORD),
        ('szInfo', wintypes.WCHAR * 256),
        ('uVersion', wintypes.UINT),  # Windows 2000/XP+
        ('szInfoTitle', wintypes.WCHAR * 64),
        ('dwInfoFlags', wintypes.DWORD),
        # 移除 guidItem 和 hBalloonIcon（避免旧系统兼容问题）
    ]

def show_notification(title, message):
    # 关键：Shell_NotifyIconW 属于 shell32.dll，不是 user32.dll
    shell32 = ctypes.WinDLL('shell32', use_last_error=True)
    
    # 初始化结构体（适配 Windows XP/Vista+）
    nid = NOTIFYICONDATA()
    # 根据系统版本设置正确的结构体大小（XP: 504, Vista+: 528，这里用兼容值）
    nid.cbSize = ctypes.sizeof(NOTIFYICONDATA)
    nid.hWnd = None  # 无窗口句柄时设为 NULL
    nid.uID = 1
    # uFlags：NIF_INFO（显示气泡）| NIF_ICON（图标）| NIF_TIP（提示）
    nid.uFlags = 0x10 | 0x02 | 0x01  # NIF_INFO=0x10, NIF_ICON=0x02, NIF_TIP=0x01
    nid.szInfoTitle = title
    nid.szInfo = message
    nid.dwInfoFlags = 0x01  # NIIF_INFO（信息图标）
    
    # 加载默认图标（可选，增强兼容性）
    user32 = ctypes.WinDLL('user32', use_last_error=True)
    nid.hIcon = user32.LoadIconW(None, 32516)  # IDI_INFORMATION（系统信息图标）
    
    # 显示通知（NIM_ADD = 0x00000000）
    shell32.Shell_NotifyIconW(0x00000000, ctypes.byref(nid))
    
    # 延迟移除图标（避免通知残留）
    import time
    time.sleep(5)
    shell32.Shell_NotifyIconW(0x00000002, ctypes.byref(nid))  # NIM_DELETE = 0x00000002