#pragma once

#include "Logger.h"
//#include <glad/glad.h>
#include <imgui.h>

using namespace Cicada;

std::shared_ptr<spdlog::logger> Log::s_logger = nullptr;
size_t Log::s_maxEntries = 1000;
bool Log::s_autoScroll = true;
bool Log::s_show = true;

void Log::Init(std::string_view logFilePath) {
	try {
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.data(), true);
		auto inMemSink = std::make_shared<InMemorySink>();

		s_logger = std::make_shared<spdlog::logger>("log", spdlog::sinks_init_list{ fileSink, inMemSink });
		spdlog::register_logger(s_logger);

		s_logger->set_level(spdlog::level::info); //NOTE: change to spdlog::level::trace for more detail
		s_logger->flush_on(spdlog::level::err); //force log to flush to file upon error
	}
	catch (const spdlog::spdlog_ex& ex) {
		//TODO: how best to handle this?
	}
}

void Log::BuildLogWindow() {
	static int logLevelFilter = 0; // Index of selected log level
	static char searchBuffer[128] = ""; // Search input buffer

	if (!s_show) return;

	ImGui::StyleColorsClassic();
	ImGui::NewFrame();
	ImGui::Begin("Debug Log", &s_show);

	// Dropdown for log level filtering
	const char* logLevels[] = { "All", "Debug", "Info", "Warning", "Error", "Critical" };
	ImGui::Combo("Log Level", &logLevelFilter, logLevels, IM_ARRAYSIZE(logLevels));

	// Search bar for filtering logs
	ImGui::InputText("Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));

	// Convert log level filter to spdlog level
	spdlog::level::level_enum minLevel = spdlog::level::trace;
	if (logLevelFilter == 1) minLevel = spdlog::level::debug;
	if (logLevelFilter == 2) minLevel = spdlog::level::info;
	if (logLevelFilter == 3) minLevel = spdlog::level::warn;
	if (logLevelFilter == 4) minLevel = spdlog::level::err;
	if (logLevelFilter == 5) minLevel = spdlog::level::critical;

	// Fetch filtered logs
	auto logs = FilterLogEntries(minLevel, searchBuffer);

	// Display logs in a scrolling region
	ImGui::BeginChild("LogWindow", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	for (const auto& log : logs) {
		ImGui::TextUnformatted(log.c_str());
	}
	ImGui::EndChild();

	ImGui::End();
}

std::vector<std::string> Log::FilterLogEntries(spdlog::level::level_enum min_level, const std::string& searchText) {

	auto inMemSink = std::dynamic_pointer_cast<InMemorySink>(s_logger->sinks()[1]);
	std::vector<std::string> filteredLogs;

	assert(inMemSink != nullptr);

	for (const auto& entry : inMemSink->logs) {
		if (entry.level < min_level) continue;
		if (entry.message.find(searchText) == std::string::npos) continue;

		filteredLogs.push_back(entry.message);
	}

	return filteredLogs;
}

/*
std::string Logger::GLTypeToStr(unsigned int glType) {
	if (s_glTypeNameMap.contains(glType))
		return s_glTypeNameMap.at(glType);
	else
		return "invalid";
}

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
*/
