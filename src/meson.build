# SPDX-License-Identifier: BSD-3-Clause
libusb = dependency(
	'libusb-1.0',
	required: true,
	version: '>=1.0.21'
)

subdir('include')

if not meson.is_subproject()
	subdir('test')
endif
