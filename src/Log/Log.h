#pragma once

#include <memory>
#include <stdio.h>

#include <spdlog/spdlog.h>

namespace EASM {

class Log {
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};

}

#define LOG_TRACE(...)		::EASM::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)		::EASM::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		::EASM::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		::EASM::Log::GetLogger()->error(__VA_ARGS__)