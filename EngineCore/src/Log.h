#pragma once

#include <imgui.h>
#include <mutex>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <unordered_map>
#include <vector>

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(Log::GetLogger(), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Log::GetLogger(), __VA_ARGS__)
#define LOG_INFO(...)  SPDLOG_LOGGER_INFO(Log::GetLogger(), __VA_ARGS__)
#define LOG_WARN(...)  SPDLOG_LOGGER_WARN(Log::GetLogger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Log::GetLogger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Log::GetLogger(), __VA_ARGS__)

namespace Cicada {

class InMemorySink : public spdlog::sinks::base_sink<std::mutex> {
friend class Log;
private:
	struct LogEntry {
		spdlog::level::level_enum level;
		std::string message;
	};

	std::vector<LogEntry> logs;

protected:
	void sink_it_(const spdlog::details::log_msg& msg) override {
		
		//format the message with level and timestamp, etc
		spdlog::memory_buf_t formatted;
		spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);

		logs.push_back({ msg.level, fmt::to_string(formatted) });
	}

	void flush_() override {} //never actually needs to flush, since msgs are being appended immediately
};

class Log {
private:
	
	static std::shared_ptr<spdlog::logger> s_logger;
	static size_t s_maxEntries;
	static bool s_autoScroll;
	static bool s_show;

	static std::unordered_map<spdlog::level::level_enum, ImVec4> s_logColorMap;

	//static const std::unordered_map<unsigned int, std::string> s_glTypeNameMap;
	//static std::string GLTypeToStr(unsigned int glType);

	Log() = default;
	
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_logger; }
	static std::vector<InMemorySink::LogEntry> FilterLogEntries(spdlog::level::level_enum min_level, const std::string& searchText);

public:
	static void Init(std::string_view logFilePath);
	static void BuildLogWindow();
	
	inline static void ToggleLogWindow() { s_show = !s_show; }

	template<typename... Args>
	inline static void Trace(spdlog::format_string_t<Args...> fmt, Args &&... args) { s_logger->trace(fmt, std::forward<Args>(args)...); }
	template<typename... Args>
	inline static void Debug(spdlog::format_string_t<Args...> fmt, Args &&... args) { s_logger->debug(fmt, std::forward<Args>(args)...); }
	template<typename... Args>
	inline static void Info(spdlog::format_string_t<Args...> fmt, Args &&... args) { s_logger->info(fmt, std::forward<Args>(args)...); }
	template<typename... Args>
	inline static void Warn(spdlog::format_string_t<Args...> fmt, Args &&... args) { s_logger->warn(fmt, std::forward<Args>(args)...); }
	template<typename... Args>
	inline static void Error(spdlog::format_string_t<Args...> fmt, Args &&... args) { s_logger->error(fmt, std::forward<Args>(args)...); }
	template<typename... Args>
	inline static void Critical(spdlog::format_string_t<Args...> fmt, Args &&... args) { s_logger->critical(fmt, std::forward<Args>(args)...); }
};

}
