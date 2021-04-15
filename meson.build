# SPDX-License-Identifier: BSD-3-Clause
project(
	'usb-cxx',
	'cpp',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
		'buildtype=debugoptimized',
		'strip=true',
		'b_ndebug=if-release',
		'b_lto=true'
	],
	version: '0.1',
	license: [
		'BSD-3-Clause',
	],
	meson_version: '>=0.57.0'
)

cxx = meson.get_compiler('cpp')

extended_warnings = [
	'-Wdouble-promotion',
	'-Wformat=2',
	'-Wformat-overflow=2',
	'-Wformat-signedness',
	'-Wformat-truncation',
	'-Wnull-dereference',
	'-Wmissing-attributes',
	'-Wmissing-braces',
	'-Wsequence-point',
	'-Wreturn-type',
	'-Wunused',
	'-Wunused-local-typedefs',
	'-Wunused-const-variable=2',
	'-Wmaybe-uninitialized',
	'-Wunknown-pragmas',
	'-Wstrict-aliasing',
	'-Wstrict-overflow=3',
	'-Wstring-compare',
	'-Wstringop-overflow',
	'-Warith-conversion',
	'-Wvla-parameter',
	'-Wduplicated-branches',
	'-Wshadow=local',
	'-Wunsafe-loop-optimizations',
	'-Wbad-function-cast',
	'-Wcast-qual',
	'-Wcast-align=strict',
	'-Wcast-function-type',
	'-Wconversion',
	'-Wdangling-else',
	'-Wsign-conversion',
	'-Wfloat-conversion',
	'-Wpacked',
	'-Wpadded',
	'-Wredundant-decls',
	'-Winline',
	'-Wvla',
	'-Wstack-protector',
	'-Wunsuffixed-float-constant',
	'-Wimplicit-fallthrough',
]

add_project_arguments(
	cxx.get_supported_arguments(extended_warnings),
	language: 'cpp'
)

subdir('src')


if not meson.is_subproject()
	pkg = import('pkgconfig')
	pkg.generate(
		subdirs: '.',
		name: 'usb-cxx',
		filebase: 'usb-cxx',
		version: meson.project_version(),
		requires: [ 'libusb-1.0' ],
		description: 'A C++17 header only libusb wrapper'
	)
endif

usb_cxx_dep = declare_dependency(
	dependencies: [ libusb ],
	include_directories: usb_cxx_inc,
	version: meson.project_version()
)
