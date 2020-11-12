module;

#include "fmt/core.h"
#include "gsl/pointers"
#include <Windows.h>
#include "constants.h"

module Fibo.MessageEvent;

namespace fibo
{
	MessageEvent::MessageEvent(gsl::not_null<WindowProcedure*> parent) noexcept :
		hWnd_{ nullptr },
		hInst_{ nullptr },
		className_{},
		parent_{ parent }
	{}

	MessageEvent::~MessageEvent() noexcept
	{
		unregisterWndClass();
	}

	// movable
	MessageEvent::MessageEvent(MessageEvent&& other) noexcept :
		hWnd_{ std::exchange(other.hWnd_, nullptr) },
		hInst_{ std::exchange(other.hInst_, nullptr) },
		className_{ std::exchange(other.className_, std::wstring{}) },
		parent_{ std::exchange(other.parent_, nullptr) }
	{}

	MessageEvent& MessageEvent::operator=(MessageEvent&& other) noexcept
	{
		if (this != &other) {
			this->~MessageEvent();
			hWnd_		= std::exchange(other.hWnd_, nullptr);
			hInst_		= std::exchange(other.hInst_, nullptr);
			className_	= std::exchange(other.className_, std::wstring{});
			parent_		= std::exchange(other.parent_, nullptr);
		}
		return *this;
	}

	HWND MessageEvent::getWndHandle() const
	{
		return hWnd_;
	}

	void MessageEvent::registerWndClass(std::wstring_view className)
	{
		if (hWnd_) {
			return;
		}

		className_ = className;
		hInst_ = ::GetModuleHandle(nullptr);
		if (not hInst_)
		{
			throw std::system_error(::GetLastError(),
				std::system_category(),
				fmt::format("Failed to call GetModuleHandleA. {}:{}",
					__FILE__,
					__LINE__));
		}

		WNDCLASS wndClass{};
		wndClass.lpfnWndProc = WndProcCb;
		wndClass.hInstance = hInst_;
		wndClass.lpszClassName = className_.c_str();

		auto succ = ::RegisterClass(&wndClass);
		if (not succ)
		{
			hInst_ = nullptr;
			throw std::system_error(::GetLastError(),
				std::system_category(),
				fmt::format("Failed to call RegisterClassA. {}:{}",
					__FILE__,
					__LINE__));
		}

		hWnd_ = ::CreateWindowEx(
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
			hInst_,
			this);

		if (not hWnd_)
		{
			auto err = ::GetLastError();
			::UnregisterClass(wndClass.lpszClassName, hInst_);
			hInst_ = nullptr;
			throw std::system_error(err,
				std::system_category(),
				fmt::format("Failed to call CreateWindowExA. {}:{}",
					__FILE__,
					__LINE__));
		}
	}

	void MessageEvent::unregisterWndClass() noexcept
	{
		if (hWnd_) {
			::DestroyWindow(hWnd_);
		}

		if (hInst_) {
			::UnregisterClass(className_.c_str(), hInst_);
		}
		hInst_ = nullptr;
		hWnd_ = nullptr;
	}

	LRESULT MessageEvent::procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		return parent_->procedure(hWnd, uMsg, wParam, lParam);
	}

	LRESULT MessageEvent::WndProcCb(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MessageEvent* pThis = nullptr;
		if (WM_NCCREATE == uMsg) {
			// Recover the "this" pointer which was passed as a parameter to CreateWindowExA
			LPCREATESTRUCTA lpcs = reinterpret_cast<LPCREATESTRUCTA>(lParam);
			pThis = static_cast<MessageEvent*>(lpcs->lpCreateParams);
			// Put the value in a safe place for future use
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else {
			// Recover the "this" pointer from where our WM_NCCREATE handler stashed it
			pThis = reinterpret_cast<MessageEvent*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (pThis) {
			return pThis->procedure(hWnd, uMsg, wParam, lParam);
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}