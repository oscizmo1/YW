del *.obj *.sbr *.bsc /s /f
del C:\AMT_NEW\MT_NEW.exe /s /f
del C:\AMT_NEW\MT_NEW.ilk /s /f
del C:\AMT_NEW\MT_NEW.pdb /s /f

@ECHO OFF

SETLOCAL

: 'devenv.com'�� �ִ� ���
:SET MSDEV="D:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com"
SET MSDEV="D:\VS2008\Common7\IDE\devenv.com"

@ECHO ON


@ECHO [Start ReleaseFinal Build]
:@for %%i in (�ַ���̸� ����.sln) do %MSDEV% /build ReleaseFinal_KOR "%%i"
: �������� �ַ���� �Ѱ��϶��� *.sln���� ����, Debug�� Release Ȥ�� �߰��� Configuration Name

@for %%i in (AMT_VS2008\MT.sln) do %MSDEV% /build debug "%%i"
:@for %%i in (*.sln) do %MSDEV% /build Release "%%i"


@ECHO OFF

ENDLOCAL
:PAUSE