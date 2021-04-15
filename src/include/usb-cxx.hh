// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#if !defined(USB_CXX_HH)
#define USB_CXX_HH

#include <libusb.h>

#include <string_view>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>


namespace usb_cxx {
	using std::literals::string_view_literals::sv;
	struct usb_endpoint_t final {
	public:
		enum class direction_t : uint8_t {
			out = 0x00U,
			in  = 0x80U,
		};
	private:
		uint8_t _num;
		direction_t _dir;
	public:
		usb_endpoint_t(uint8_t num, direction_t dir) noexcept : _num{num}, _dir{dir} { /* NOP */ }

		uint8_t number() const noexcept { return _num; }
		void number(const uint8_t number) noexcept { _num = number; }

		direction_t direction() const noexcept { return _dir; }
		void direction(const direction_t dir) noexcept { _dir = dir; }

		operator uint8_t() const noexcept { return (static_cast<uint8_t>(_dir) | ( _num & 0x7F)); }
	};

	/* ========== */

	struct usb_ctl_t final {
	/* TODO: _req_t breakdown */
	private:
		uint8_t _req_t;
		uint8_t _req;
		uint16_t _value;
		uint16_t _index;
	public:
		usb_ctl_t(uint8_t req_t, uint8_t req, uint16_t value, uint16_t idx) noexcept :
			_req_t{req_t}, _req{req}, _value{value}, _index{idx} { /* NOP */ }

		uint16_t index() const noexcept { return _index; }
		void index(const uint16_t index) noexcept { _index = index; }

		uint16_t value() const noexcept { return _value; }
		void value(const uint16_t value) noexcept { _value = value; }

		uint8_t request() const noexcept { return _req; }
		void request(const uint8_t request) noexcept { _req = request; }

		uint8_t request_type() const noexcept { return _req_t; }
		void request_type(const uint8_t request_type) noexcept { _req_t = request_type; }
	};

	/* ========== */

	struct usb_dev_handle_t final {
	private:
		std::unique_ptr<libusb_device_handle, decltype(&libusb_close)> _dev_handle;
	public:
		usb_dev_handle_t() noexcept = default;
		usb_dev_handle_t(libusb_device_handle* hndl) noexcept :
			_dev_handle{hndl, libusb_close} { /* NOP */ }

		bool valid() const noexcept { return bool(_dev_handle); }

		/* TODO?: libusb_get_device */
		/* TODO: libusb_get_configuration */
		/* TODO: libusb_set_configuration */

		bool claim(const int32_t interface) const noexcept {
			const auto res = libusb_claim_interface(_dev_handle.get(), interface);
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		bool release(const int32_t interface) const noexcept {
			const auto res = libusb_release_interface(_dev_handle.get(), interface);
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		/* TODO: libusb_set_interface_alt_setting */

		bool clear_halt(const usb_endpoint_t ep) const noexcept {
			const auto res = libusb_clear_halt(_dev_handle.get(), ep);
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		bool reset() const noexcept {
			const auto res = libusb_reset_device(_dev_handle.get());
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		std::optional<bool> kernel_driver_active(const int32_t interface) const noexcept {
			const auto res = libusb_kernel_driver_active(_dev_handle.get(), interface);
			if (res < 0) {
				/* TODO: Error reporting */
				return std::nullopt;
			}

			return std::make_optional<bool>(res);
		}

		bool detach_kernel_driver(const int32_t interface) const noexcept {
			const auto res = libusb_detach_kernel_driver(_dev_handle.get(), interface);
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		bool attach_kernel_driver(const int32_t interface) const noexcept {
			const auto res = libusb_attach_kernel_driver(_dev_handle.get(), interface);
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		bool kernel_auto_detach(bool auto_detach) const noexcept {
			const auto res = libusb_set_auto_detach_kernel_driver(_dev_handle.get(), auto_detach);
			if (res) {
				/* TODO: Error reporting */
			}

			return !res;
		}

		std::optional<int32_t> control_transfer(const usb_ctl_t ctl, void *const buff, const int16_t len, const uint32_t timeout = 0) const noexcept {
			const auto res = libusb_control_transfer(
				_dev_handle.get(), ctl.request_type(), ctl.request(),
				ctl.value(), ctl.index(), static_cast<uint8_t*>(buff), len, timeout);

			if (res < 0) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional<int32_t>(res);
		}

		std::optional<int32_t> bulk_transfer(const usb_endpoint_t ep, void* const buff, const int32_t len, const uint32_t timeout = 0) const noexcept {
			int32_t txd{0};
			const auto res = libusb_bulk_transfer(_dev_handle.get(), ep, static_cast<uint8_t*>(buff), len, &txd, timeout);
			if (res) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional<int32_t>(txd);
		}

		std::optional<int32_t> interrupt_transfer(const usb_endpoint_t ep, void* const buff, const int32_t len, const uint32_t timeout = 0) const noexcept {
			int32_t txd{0};
			const auto res = libusb_interrupt_transfer(_dev_handle.get(), ep, static_cast<uint8_t*>(buff), len, &txd, timeout);
			if (res) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional<int32_t>(txd);
		}
	};

	/* ========== */

	struct usb_dev_t final {
	public:
		enum class speed_t : int {
			unknown    = LIBUSB_SPEED_UNKNOWN,
			low        = LIBUSB_SPEED_LOW,
			full       = LIBUSB_SPEED_FULL,
			high       = LIBUSB_SPEED_HIGH,
			super      = LIBUSB_SPEED_SUPER,
			super_plus = LIBUSB_SPEED_SUPER_PLUS,
		};
	private:
		libusb_device* _dev{nullptr};
		libusb_device_descriptor _dev_descriptor{};
	public:
		usb_dev_t() noexcept = default;
		usb_dev_t(libusb_device* dev) noexcept : _dev{dev} {
			if (_dev) {
				libusb_ref_device(_dev);
				if (const auto res = libusb_get_device_descriptor(_dev, &_dev_descriptor); res) {
					/* TODO: Error reporting */
					_dev_descriptor = {};
				}
			}
		}

		~usb_dev_t() noexcept {
			if (_dev)
				libusb_unref_device(_dev);
		}

		bool valid() const noexcept { return _dev; }

		uint8_t bus() const noexcept { return libusb_get_bus_number(_dev); }
		uint8_t port() const noexcept { return libusb_get_port_number(_dev); }
		/* TODO: libusb_get_port_numbers */
		/* TODO: libusb_get_port_path */
		std::optional<usb_dev_t> parent() const noexcept {
			auto dev = libusb_get_parent(_dev);
			if (!dev) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional(dev);

		}
		uint8_t address() const noexcept { return libusb_get_device_address(_dev); }
		speed_t speed() const noexcept { return static_cast<speed_t>(libusb_get_device_speed(_dev)); }

		std::optional<size_t> max_packet_size(const usb_endpoint_t ep) const noexcept {
			auto res = libusb_get_max_packet_size(_dev, ep);
			if (res < 0) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional<size_t>(res);
		}
		std::optional<size_t> max_iso_packet_size(const usb_endpoint_t ep) const noexcept {
			auto res = libusb_get_max_iso_packet_size(_dev, ep);
			if (res < 0) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional<size_t>(res);
		}

		uint16_t vid() const noexcept { return _dev_descriptor.idVendor; }
		uint16_t pid() const noexcept { return _dev_descriptor.idProduct; }

		/* TODO?: libusb_wrap_sys_device */

		std::optional<usb_dev_handle_t> open() const noexcept {
			libusb_device_handle* hndl{nullptr};

			if (const auto res = libusb_open(_dev, &hndl); res) {
				/* TODO: Error reporting */
				return std::nullopt;
			}

			return std::make_optional<usb_dev_handle_t>(hndl);
		}

	};

	/* ========== */

	struct usb_dev_list_t final {
	public:
		struct iterator_t final {
		/* FIXME: This can just run off into arbitrary memory, should clamp it. */
		private:
			libusb_device** _current{nullptr};
			size_t _max{0};
		public:
			iterator_t(libusb_device** current, size_t max) noexcept :
				_current{current}, _max{max} { /* NOP */ }

			usb_dev_t operator*() const noexcept { return {*_current}; }
			usb_dev_t operator->() const noexcept { return {*_current}; }

			auto operator++() noexcept { ++_current; return *this; }
			auto operator--() noexcept { --_current; return *this; }

			bool operator==(const iterator_t& itr) { return (_current == itr._current); }
			bool operator!=(const iterator_t& itr) { return !operator==(itr); }

			bool operator>(const iterator_t& itr) { return (_current > itr._current); }
			bool operator>=(const iterator_t& itr) { return (operator>(itr) || operator==(itr)); }

			bool operator<(const iterator_t& itr) { return (_current < itr._current); }
			bool operator<=(const iterator_t& itr) { return (operator<(itr) || operator==(itr)); }
		};
	private:
		ssize_t _dev_count{0};
		libusb_device** _dev_list{nullptr}; /* This is needed because libusb has a stupid API */
	public:
		usb_dev_list_t(libusb_context* ctx) noexcept {
			_dev_count = libusb_get_device_list(ctx, &_dev_list);
			if (_dev_count < 0) {
				/* TODO: Error reporting */
			}
		}

		~usb_dev_list_t() noexcept {
			if (_dev_count > 0) {
				libusb_free_device_list(_dev_list, false);
			}
		}

		bool valid() const noexcept { return _dev_count > 0 && _dev_list; }
		size_t size() const noexcept { return (_dev_count > 0) ? _dev_count : 0; }

		auto begin() noexcept { return iterator_t(_dev_list, _dev_count); }
		auto end() noexcept { return iterator_t(_dev_list + _dev_count, _dev_count); }
	};

	/* ========== */

	struct usb_t final {
	private:
		std::unique_ptr<libusb_context, decltype(&libusb_exit)> _usb_ctx;
		usb_dev_list_t _usb_devs;
	public:
		usb_t() noexcept : _usb_ctx{[](){
			libusb_context* _ctx{nullptr};

			if (auto res = libusb_init(&_ctx); res) {
				/* TODO: Error reporting */
			}

			return _ctx;

		}(), libusb_exit}, _usb_devs{_usb_ctx.get()} { /* NOP */ }


		std::optional<usb_dev_handle_t> open(uint16_t vid, uint16_t pid) const noexcept {
			auto hndl = libusb_open_device_with_vid_pid(_usb_ctx.get(), vid, pid);
			if (!hndl) {
				/* TODO: Error reporting */
				return std::nullopt;
			}
			return std::make_optional<usb_dev_handle_t>(hndl);
		}

		bool valid() const noexcept { return bool(_usb_ctx); }

		auto begin() noexcept { return _usb_devs.begin(); }
		auto end() noexcept { return _usb_devs.end(); }

		/* TODO: libusb_set_debug */
		/* TODO: libusb_set_log_cb */
		/* TODO: libusb_set_option */
	};
}


#endif /* USB_CXX_HH */
