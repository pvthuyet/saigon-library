module;

#include "gsl/pointers"
#include <string>
#include <Windows.h>

export module Saigon.MessageEvent;

namespace saigon
{
	/// <summary>
	/// 
	/// </summary>
	export class window_procedure
	{
	public:
		virtual ~window_procedure() noexcept = default;
		LRESULT procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return do_procedure(hwnd, msg, wparam, lparam);
		}

	private:
		virtual LRESULT do_procedure(HWND, UINT, WPARAM, LPARAM) = 0;
	};

	export class message_event
	{
	public:
		explicit message_event(gsl::not_null<window_procedure*> parent) noexcept;
		~message_event() noexcept;

		// no copyable
		message_event(message_event const&) = delete;
		message_event& operator=(message_event const&) = delete;

		// movable
		message_event(message_event&& other) noexcept;
		message_event& operator=(message_event&& other) noexcept;

		void register_window_class(std::wstring_view className);
		void unregister_window_class() noexcept;

		HWND get_handle() const;

	private:
		LRESULT procedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		static LRESULT CALLBACK procedure_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND mHwnd{ nullptr };
		HINSTANCE mHinst{ nullptr };
		std::wstring mClassName;
		window_procedure* mParent;
	};
}