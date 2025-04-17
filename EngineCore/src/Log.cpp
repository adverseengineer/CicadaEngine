#pragma once

#include "Log.h"

using namespace Cicada;

int InMemorySink::s_maxEntries = 10; //TODO: set back to 1000 after UBO testing

std::shared_ptr<spdlog::logger> Log::s_logger = nullptr;
bool Log::s_autoScroll = true;
bool Log::s_show = true;

std::unordered_map<spdlog::level::level_enum, ImVec4> Log::s_logColorMap = {
	{ spdlog::level::trace, ImVec4(1.0, 1.0, 1.0, 1.0) }, //white
	{ spdlog::level::debug, ImVec4(1.0, 0.0, 1.0, 1.0) }, //magenta
	{ spdlog::level::info, ImVec4(0.7, 0.9, 1.0, 1.00) }, //baby blue
	{ spdlog::level::warn, ImVec4(1.0, 1.0, 0.0, 1.0) }, //yellow
	{ spdlog::level::err, ImVec4(1.0, 0.5, 0.0, 1.0) }, //orange
	{ spdlog::level::critical, ImVec4(1.0, 0.0, 0.0, 1.0) } //red
};

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

	static int logLevelFilter = 0;
	static char searchBuffer[128] = "";

	if (!s_show) return;

	ImGui::StyleColorsClassic();
	ImGui::NewFrame();
	ImGui::Begin("Debug Log", &s_show);

	ImGui::InputInt("Max Entries", &InMemorySink::s_maxEntries);

	ImGui::SameLine();
	if (ImGui::Button("Clear Log")) {
		std::dynamic_pointer_cast<InMemorySink>(Log::s_logger->sinks()[1])->clear(); //clear the in-memory log entries
	}

	const char* logLevels[] = { "All", "Debug", "Info", "Warning", "Error", "Critical" };
	ImGui::Combo("Log Level", &logLevelFilter, logLevels, IM_ARRAYSIZE(logLevels));

	ImGui::InputText("Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));

	spdlog::level::level_enum minLevel = spdlog::level::trace;
	if (logLevelFilter == 1) minLevel = spdlog::level::debug;
	if (logLevelFilter == 2) minLevel = spdlog::level::info;
	if (logLevelFilter == 3) minLevel = spdlog::level::warn;
	if (logLevelFilter == 4) minLevel = spdlog::level::err;
	if (logLevelFilter == 5) minLevel = spdlog::level::critical;

	//fetch logs based on the current criteria
	auto logs = FilterLogEntries(minLevel, searchBuffer);

	//and display them in a scroll box
	ImGui::BeginChild("LogWindow", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	for (const auto& log : logs) {
		assert(s_logColorMap.contains(log.level));
		ImGui::TextColored(s_logColorMap.at(log.level), log.message.c_str());
	}
	ImGui::EndChild();

	ImGui::End();
}

std::vector<InMemorySink::LogEntry> Log::FilterLogEntries(spdlog::level::level_enum min_level, const std::string& searchText) {

	auto inMemSink = std::dynamic_pointer_cast<InMemorySink>(s_logger->sinks()[1]);
	std::vector<InMemorySink::LogEntry> filteredLogs;

	assert(inMemSink != nullptr);

	std::copy_if(
		inMemSink->m_logs.begin(),
		inMemSink->m_logs.end(),
		std::back_inserter(filteredLogs),
		[&](const auto& log) {
			return (log.level >= min_level) && (log.message.find(searchText) != std::string::npos);
		}
	);

	return filteredLogs;
}

//const std::unordered_map<GLint, std::string> Log::s_glTypeNameMap = {
//	{ GL_BYTE, "byte" },
//	{ GL_UNSIGNED_BYTE, "ubyte" },
//	{ GL_SHORT, "short"},
//	{ GL_UNSIGNED_SHORT, "ushort" },
//	{ GL_INT, "int" },
//	{ GL_UNSIGNED_INT, "uint" },
//	{ GL_FLOAT, "float" },
//	{ GL_DOUBLE, "double" },
//	{ GL_FLOAT_VEC2, "vec2f" },
//	{ GL_FLOAT_VEC3, "vec3f" },
//	{ GL_FLOAT_VEC4, "vec4f" },
//	{ GL_FLOAT_MAT3, "mat3f" },
//	{ GL_FLOAT_MAT4, "mat4f" },
//	{ GL_SAMPLER_2D, "sampler2D"},
//	{ GL_VERTEX_SHADER, "vertexShader" },
//	{ GL_FRAGMENT_SHADER, "fragShader" }
//};
