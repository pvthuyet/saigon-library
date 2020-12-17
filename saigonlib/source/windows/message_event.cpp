module;

#include <string>
#include <system_error>
#include "fmt/format.h"
#include <Windows.h>

module Saigon.MessageEvent;

namespace saigon
{
	message_event::message_event(gsl::not_null<window_procedure*> parent) noexcept :
		mHwnd{ nullptr },
		mHinst{ nullptr },
		mClassName{},
		mParent{ parent }
	{}

	message_event::~message_event() noexcept
	{
		unregister_window_class();
	}

	// movable
	message_event::message_event(message_event&& other) noexcept :
		mHwnd{ std::exchange(other.mHwnd, nullptr) },
		mHinst{ std::exchange(other.mHinst, nullptr) },
		mClassName{ std::exchange(other.mClassName, std::wstring{}) },
		mParent{ std::exchange(other.mParent, nullptr) }
	{}

	message_event& message_event::operator=(message_event&& other) noexcept
	{
		if (this != &other) {
			this->~message_event();
			mHwnd = std::exchange(other.mHwnd, nullptr);
			mHinst = std::exchange(other.mHinst, nullptr);
			mClassName = std::exchange(other.mClassName, std::wstring{});
			mParent = std::exchange(other.mParent, nullptr);
		}
		return *this;
	}

	HWND message_event::get_handle() const
	{
		return mHwnd;
	}

	void message_event::register_window_class(std::wstring_view className)
	{
		if (mHwnd) {
			return;
		}

		mClassName = className;
		mHinst = ::GetModuleHandle(nullptr);
		if (not mHinst)
		{
			throw std::system_error(::GetLastError(),
				std::system_category(),
				fmt::format("Failed to call GetModuleHandleA. {}:{}",
					__FILE__,
					__LINE__));
		}

		WNDCLASS wndClass{};
		wndClass.lpfnWndProc = procedure_callback;
		wndClass.hInstance = mHinst;
		wndClass.lpszClassName = mClassName.c_str();

		auto succ = ::RegisterClass(&wndClass);
		if (not succ)
		{
			mHinst = nullptr;
			throw std::system_error(::GetLastError(),
				std::system_category(),
				fmt::format("Failed to call RegisterClassA. {}:{}",
					__FILE__,
					__LINE__)); //++ TODO
		}

		mHwnd = ::CreateWindowEx(
			0,
			wndClass.lpszClassName,
			L"message event name",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			HWND_MESSAGE,
			NULL,
			mHinst,
			this);

		if (not mHwnd)
		{
			auto err = ::GetLastError();
			::UnregisterClass(wndClass.lpszClassName, mHinst);
			mHinst = nullptr;
			throw std::system_error(err,
				std::system_category(),
				fmt::format("Failed to call CreateWindowExA. {}:{}",
					__FILE__,
					__LINE__));
		}
	}

	void message_event::unregister_window_class() noexcept
	{
		if (mHwnd) {
			::DestroyWindow(mHwnd);
		}

		if (mHinst) {
			::UnregisterClass(mClassName.c_str(), mHinst);
		}
		mHinst = nullptr;
		mHwnd = nullptr;
	}

	LRESULT message_event::procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		return mParent->procedure(hWnd, uMsg, wParam, lParam);
	}

	LRESULT message_event::procedure_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		message_event* pThis = nullptr;
		if (WM_NCCREATE == uMsg) {
			// Recover the "this" pointer which was passed as a parameter to CreateWindowExA
			LPCREATESTRUCTA lpcs = reinterpret_cast<LPCREATESTRUCTA>(lParam);
			pThis = static_cast<message_event*>(lpcs->lpCreateParams);
			// Put the value in a safe place for future use
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else {
			// Recover the "this" pointer from where our WM_NCCREATE handler stashed it
			pThis = reinterpret_cast<message_event*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (pThis) {
			return pThis->procedure(hWnd, uMsg, wParam, lParam);
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}