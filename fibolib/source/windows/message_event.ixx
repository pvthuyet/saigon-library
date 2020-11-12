module;

#include "gsl/pointers"
#include <string>
#include <Windows.h>

export module Fibo.MessageEvent;

namespace fibo
{
	/// <summary>
	/// 
	/// </summary>
	export class WindowProcedure
	{
	public:
		virtual ~WindowProcedure() noexcept {}
		virtual LRESULT procedure(HWND, UINT, WPARAM, LPARAM) const = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	export class MessageEvent
	{
	public:
		explicit MessageEvent(gsl::not_null<WindowProcedure const*> parent) noexcept;
		~MessageEvent() noexcept;

		// no copyable
		MessageEvent(MessageEvent const&) = delete;
		MessageEvent& operator=(MessageEvent const&) = delete;

		// movable
		MessageEvent(MessageEvent&& other) noexcept;
		MessageEvent& operator=(MessageEvent&& other) noexcept;

		void registerWndClass(std::wstring_view className);
		void unregisterWndClass() noexcept;

		HWND getWndHandle() const;

	private:
		LRESULT procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		static LRESULT CALLBACK WndProcCb(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND hWnd_{ nullptr };
		HINSTANCE hInst_{ nullptr };
		std::wstring className_;
		WindowProcedure const* parent_;
	};
}