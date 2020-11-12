module;

#include "define.h"
#include "fmt/core.h"
#include "gsl/pointers"
#include <string>
#include <system_error>
#include <Windows.h>
#include "constants.h"

export module Fibo.MessageEvent;

namespace fibo
{
	export class WindowProcedure
	{
	public:
		virtual ~WindowProcedure() noexcept {}
		virtual LRESULT procedure(HWND, UINT, WPARAM, LPARAM) const = 0;
	};

	export class MessageEvent
	{
	public:
		explicit MessageEvent(gsl::not_null<WindowProcedure const*> parent) noexcept :
			hWnd_{ nullptr },
			hInst_{ nullptr },
			className_{},
			parent_{parent}
		{}

		~MessageEvent() noexcept
		{
			unregisterWndClass();
		}

		// no copyable
		MessageEvent(MessageEvent const&) = delete;
		MessageEvent& operator=(MessageEvent const&) = delete;

		// movable
		MessageEvent(MessageEvent&& other) noexcept:
			hWnd_{ std::exchange(other.hWnd_, nullptr) },
			hInst_{ std::exchange(other.hInst_, nullptr) },
			className_{ std::exchange(other.className_, std::string{}) },
			parent_{ std::exchange(other.parent_, nullptr) }
		{}

		MessageEvent& operator=(MessageEvent&& other) noexcept
		{
			if (this != &other) {
				this->~MessageEvent();
				hWnd_		= std::exchange(other.hWnd_, nullptr);
				hInst_		= std::exchange(other.hInst_, nullptr),
				className_	= std::exchange(other.className_, std::string{});
				parent_		= std::exchange(other.parent_, nullptr);
			}
			return *this;
		}

		void registerWndClass(std::string_view className, std::string_view wndName)
		{
			if (hWnd_) {
				return;
			}

			className_ = className;
			hInst_ = ::GetModuleHandleA(nullptr);
			if (!hInst_) 
			{
				throw std::system_error(::GetLastError(), 
					std::system_category(),
					fmt::format("Failed to call GetModuleHandleA. Error: {}. {}:{}",
						::GetLastError(),
						__FILE__,
						__LINE__));
			}

			WNDCLASSA wndClass = { 0 };
			wndClass.lpfnWndProc = WndProcCb;
			wndClass.hInstance = hInst_;
			wndClass.lpszClassName = className_.c_str();

			auto ret = ::RegisterClassA(&wndClass);
			if (!ret) 
			{
				hInst_ = nullptr;
				throw std::system_error(::GetLastError(), 
					std::system_category(),
					fmt::format("Failed to call RegisterClassA. Error: {}. {}:{}",
					::GetLastError(),
					__FILE__,
					__LINE__));
			}

			hWnd_ = ::CreateWindowExA(0,
				wndClass.lpszClassName,
				wndName.data(),
				0,
				CW_USEDEFAULT,
				0,
				CW_USEDEFAULT,
				0,
				nullptr,
				nullptr,
				hInst_,
				this);

			if (!hWnd_)
			{
				::UnregisterClassA(wndClass.lpszClassName, hInst_);
				hInst_ = nullptr;
				throw std::system_error(::GetLastError(),
					std::system_category(),
					fmt::format("Failed to call CreateWindowExA. Error: {}. {}:{}",
						::GetLastError(),
						__FILE__,
						__LINE__));
			}
		}

		void unregisterWndClass() noexcept
		{
			if (hWnd_) {
				::DestroyWindow(hWnd_);
			}

			if (hInst_) {
				::UnregisterClassA(className_.c_str(), hInst_);
			}
			hInst_ = nullptr;
			hWnd_ = nullptr;
		}

		//++ TODO not use
		[[maybe_unused]] long long stop() const noexcept
		{
			if (hWnd_) {
				return ::SendMessageA(hWnd_, WM_STOP_LOOP, 0, 0); // must wait to finish
			}
			return F_ERROR_SUCCESS;
		}

	private:
		LRESULT procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
		{
			return parent_->procedure(hWnd, uMsg, wParam, lParam);
		}

		static LRESULT CALLBACK WndProcCb(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			MessageEvent* pThis = nullptr;
			if (WM_NCCREATE == uMsg) {
				// Recover the "this" pointer which was passed as a parameter to CreateWindowExA
				LPCREATESTRUCTA lpcs = reinterpret_cast<LPCREATESTRUCTA>(lParam);
				pThis = static_cast<MessageEvent*>(lpcs->lpCreateParams);
				// Put the value in a safe place for future use
				::SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			}
			else {
				// Recover the "this" pointer from where our WM_NCCREATE handler stashed it
				pThis = reinterpret_cast<MessageEvent*>(::GetWindowLongPtrA(hWnd, GWLP_USERDATA));
			}

			if (pThis) {
				return pThis->procedure(hWnd, uMsg, wParam, lParam);
			}
			return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
		}

	private:
		HWND hWnd_{ nullptr };
		HINSTANCE hInst_{ nullptr };
		std::string className_;
		WindowProcedure const* parent_;
	};
}