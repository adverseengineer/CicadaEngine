#pragma once

#include "Util.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <glm/matrix.hpp>

namespace Cicada {

class JsonUtils {
public:

    inline static bool ReadJson(const std::string& path, rapidjson::Document& out_result) {

        std::string jsonSource;
        if (!Util::ReadFileToString(path.c_str(), jsonSource))
            return false;

        out_result.Parse(jsonSource);
        if (out_result.HasParseError()) {
            Log::Writef(LogEntry::Severity::Error, "file {:?} is not valid JSON", path);
            Log::Writef(
                LogEntry::Severity::Error,
                "Error(offset {:d}): {:s}",
                out_result.GetErrorOffset(),
                rapidjson::GetParseError_En(out_result.GetParseError())
            );
            return false;
        }

        return true;
    }

    inline static bool ValidateAgainstSchema(const rapidjson::Document& schema, const rapidjson::Document& json) {

        //create a schema validator from the provided json document
        rapidjson::SchemaDocument schemaDoc(schema);
        rapidjson::SchemaValidator validator(schemaDoc);

        if (!json.Accept(validator)) {
            // Get error message
            rapidjson::StringBuffer sb;
            validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
            Log::Writef(LogEntry::Severity::Error, "Invalid schema: {:s}", sb.GetString());
            Log::Writef(LogEntry::Severity::Error, "Invalid keyword: {:s}", validator.GetInvalidSchemaKeyword());
            sb.Clear();
            validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
            Log::Writef(LogEntry::Severity::Error, "Invalid document: {:s}", sb.GetString());
            return false;
        }
        return true;
    }

    inline static std::string AsString(const rapidjson::Value& json) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        json.Accept(writer);
        return buffer.GetString();
    }

    template<size_t C, size_t R>
    inline static bool ParseMatrix(const rapidjson::Value& json, glm::mat<C, R, float>& matrix) {

        assert(json.IsArray());
        const auto& matrixJson = json.GetArray();
        assert(matrixJson.Size() == R);

        glm::mat<C, R, float> temp; //only create after as many checks as possible have passed

        for (size_t i = 0; i < R; i++) {
            const auto& matrixRowJson = matrixJson[i];
            assert(matrixRowJson.IsArray());
            assert(matrixRowJson.Size() == C);
            for (size_t j = 0; j < C; j++) {
                assert(matrixRowJson[j].IsFloat());
                temp[i][j] = matrixJson[i][j].GetFloat();
            }
        }

        matrix = temp;
        return true;
    }

    template <glm::length_t L>
    inline static void ParseVector(const rapidjson::GenericArray<true, rapidjson::Value>& arr, glm::vec<L, float> vec) {
        assert(arr.Size() == L);
        for (glm::length_t i = 0; i < L; i++) {
            assert(arr[i].IsFloat());
            vec[i] = arr[i];
        }
    }
};

}
