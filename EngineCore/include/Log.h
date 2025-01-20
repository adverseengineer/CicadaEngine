#pragma once

#include <ctime>
#include <string>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>
#include <imgui.h>

struct LogEntry {

	enum class Severity {
		Info,
		Warning,
		Error
	};

	std::string m_content;
	std::time_t m_timeStamp;
	Severity m_severity;

	LogEntry(const std::string& content, Severity severity) :
		m_content(content), m_severity(severity), m_timeStamp(std::time(0)) {}
};

class Log {
private:
	static bool s_showLog;
	static size_t s_maxEntries;
	static std::vector<LogEntry> s_log;
	static std::unordered_map<LogEntry::Severity, const char*> s_msgSeverityMap;

public:
	static void ToggleLog() {
		s_showLog = !s_showLog;
	}
	
	static void RenderLog() {
		ImGui::Begin("Log", &s_showLog);
		
		if (ImGui::Button("Clear")) {
			s_log.clear();
		}

		ImGui::InputInt("Max Entries", (int*) & s_maxEntries);
		if (s_log.size() > s_maxEntries) {
			auto firstErased = s_log.begin();
			auto lastErased = firstErased + (s_log.size() - s_maxEntries);
			s_log.erase(firstErased, lastErased);
			//s_log.erase(s_log.begin(), s_log.begin() + (s_log.size() - s_maxEntries));
		}

		// Make the child window scrollable
		if (ImGui::BeginChild("LogScroll", ImVec2(0, 0), true)) {
			for (const auto& entry : s_log) {
				
				std::string text = fmt::format("{:s} {:s}", s_msgSeverityMap[entry.m_severity], entry.m_content);
				//ImGui::TextUnformatted(entry.m_content.c_str());
				ImGui::TextUnformatted(text.c_str());
			}

			// Auto-scroll to the bottom
			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
				ImGui::SetScrollHereY(1.0f);
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}

	inline static void Write(LogEntry::Severity severity, const std::string& msg) {
		s_log.emplace_back(msg, severity);
	}
	inline static void Write(LogEntry::Severity severity, const char* msg) {
		s_log.emplace_back(msg, severity);
	}

	inline static void Writef(LogEntry::Severity severity, const char* fmt, ...) {
		static const unsigned int CHAR_LIMIT = 256;
		va_list args;
		va_start(args, fmt);
		char message[CHAR_LIMIT];
		vsnprintf(message, CHAR_LIMIT, fmt, args);
		va_end(args);
		s_log.emplace_back(message, severity);
	}
};
