#pragma once

#include "Logger.h"
#include <fmt/chrono.h>
#include <glad/glad.h>
#include <imgui.h>

using namespace Cicada;

LogEntry::LogEntry(Level level, const std::string& content) :
	m_severity(level), m_timeStamp(std::time(nullptr)), m_content(content) {}

bool Logger::s_showLog = true;
bool Logger::s_autoScroll = true;
size_t Logger::s_maxEntries = 1000;
std::vector<LogEntry> Logger::s_log;

void Logger::ToggleLog() {
	s_showLog = !s_showLog;
}

void Logger::RenderLog() {

	if (!s_showLog)
		return;

	ImGui::Begin("Log", &s_showLog);

	if (ImGui::Button("Clear")) {
		s_log.clear();
	}

	ImGui::InputInt("Max Entries", (int*)&s_maxEntries);
	if (s_log.size() > s_maxEntries) {
		auto firstErased = s_log.begin();
		auto lastErased = firstErased + (s_log.size() - s_maxEntries);
		s_log.erase(firstErased, lastErased);
		//s_log.erase(s_log.begin(), s_log.begin() + (s_log.size() - s_maxEntries));
	}

	ImGui::Checkbox("Auto-scroll", &s_autoScroll);

	//make the child window scrollable
	if (ImGui::BeginChild("LogScroll", ImVec2(0, 0), true)) {
		for (const auto& entry : s_log) {
			std::string text = fmt::format("{:%H:%M:%S} {:s} {:s}",
				fmt::localtime(entry.m_timeStamp),
				s_msgSeverityMap.at(entry.m_severity),
				entry.m_content
			);

			ImGui::TextUnformatted(text.c_str());
		}

		//if autoscroll is enabled and we're not at the bottom, go there
		if (s_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1.0f);
		}
	}
	ImGui::EndChild();
	ImGui::End();
}

void Logger::Write(LogEntry::Level severity, const std::string& msg) {
	s_log.emplace_back(severity, msg);
}

void Logger::_Writefv(LogEntry::Level severity, fmt::string_view fmt, fmt::format_args args) {
	std::string msg = fmt::vformat(fmt, args);
	s_log.emplace_back(severity, msg);
}

void Logger::Log(const std::string& msg) {
	Write(LogEntry::Level::Info, msg);
}

void Logger::Warn(const std::string& msg) {
	Write(LogEntry::Level::Warning, msg);
}

void Logger::Error(const std::string& msg) {
	Write(LogEntry::Level::Error, msg);
}

std::string Logger::GLTypeToStr(unsigned int glType) {
	if (s_glTypeNameMap.contains(glType))
		return s_glTypeNameMap.at(glType);
	else
		return "invalid";
}

const std::unordered_map<LogEntry::Level, std::string> Logger::s_msgSeverityMap = {
	{ LogEntry::Level::Info, "[INFO]" },
	{ LogEntry::Level::Warning, "[WARN]" },
	{ LogEntry::Level::Error, "[ERROR]" }
};

const std::unordered_map<unsigned int, std::string> Logger::s_glTypeNameMap = {
	{ GL_BYTE, "byte" },
	{ GL_UNSIGNED_BYTE, "ubyte" },
	{ GL_SHORT, "short"},
	{ GL_UNSIGNED_SHORT, "ushort" },
	{ GL_INT, "int" },
	{ GL_UNSIGNED_INT, "uint" },
	{ GL_FLOAT, "float" },
	{ GL_DOUBLE, "double" },
	{ GL_FLOAT_VEC2, "vec2f" },
	{ GL_FLOAT_VEC3, "vec3f" },
	{ GL_FLOAT_VEC4, "vec4f" },
	{ GL_FLOAT_MAT3, "mat3f" },
	{ GL_FLOAT_MAT4, "mat4f" },
	{ GL_SAMPLER_2D, "sampler2D"},
	{ GL_VERTEX_SHADER, "vertexShader" },
	{ GL_FRAGMENT_SHADER, "fragShader" }
};
