#pragma once
#include <string>
#include <vector>

void write_durations_csv(const std::string& path,
                         const std::string& op_name,
                         const std::string& hash_name,
                         const std::string& map_type,
                         const std::vector<double>& durations);