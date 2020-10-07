#pragma once
#include "testing_system.h"
namespace testing_system {
	interface config;
	interface module {
		virtual void listen() noexcept = 0;
		virtual void free() noexcept = 0;
		virtual void set_config(config* cfg) noexcept = 0;
	};
}