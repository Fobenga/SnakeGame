#include "MainWindow.h"
#include "Game.h"
#include "ChiliException.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	try
	{
		// game starts here -- 
		MainWindow wnd(hInst, pArgs);
		try
		{
			game theGame(wnd);
			while (wnd.ProcessMessage())
				theGame.go();

		} catch (const ChiliException& e)
		{
			const std::wstring eMsg = e.GetFullMessage() +
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox(e.GetExceptionType(), eMsg);
		} catch (const std::exception& e)
		{
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr(e.what());
			const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
				L"\n\nException caught at Windows message loop.";
			wnd.ShowMessageBox(L"Unhandled STL Exception", eMsg);
		} catch (...)
		{
			wnd.ShowMessageBox(L"Unhandled Non-STL Exception",
							   L"\n\nException caught at Windows message loop.");
		}
	} catch (const ChiliException& e)
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
		MessageBox(nullptr, eMsg.c_str(), e.GetExceptionType().c_str(), MB_OK);
	} catch (const std::exception& e)
	{
		// need to convert std::exception what() string from narrow to wide string
		const std::string whatStr(e.what());
		const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
			L"\n\nException caught at main window creation.";
		MessageBox(nullptr, eMsg.c_str(), L"Unhandled STL Exception", MB_OK);
	} catch (...)
	{
		MessageBox(nullptr, L"\n\nException caught at main window creation.",
				   L"Unhandled Non-STL Exception", MB_OK);
	}

	return 0;
}