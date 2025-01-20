#pragma once

#include "Log.h"

bool Log::s_showLog = true;
size_t Log::s_maxEntries = 1000;
std::vector<LogEntry> Log::s_log;

std::unordered_map<LogEntry::Severity, const char*> Log::s_msgSeverityMap = {
	{ LogEntry::Severity::Info, "[INFO]" },
	{ LogEntry::Severity::Warning, "[WARNING]" },
	{ LogEntry::Severity::Error, "[ERROR]" }
};
