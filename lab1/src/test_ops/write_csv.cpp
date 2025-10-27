#include "write_csv.hpp"
#include <fstream>

void write_durations_csv(const std::string& path,
                         const std::string& op_name,
                         const std::string& map_type,
                         const std::string& hash_name,
                         const std::vector<double>& durations_us)
{
    std::ofstream file(path, std::ios::app);
    if (!file.is_open()) return;

    if (file.tellp() == 0) {
        file << "operation,map_type,hash_type,duration_us\n";
    }

    for (auto d : durations_us) {
        file << op_name << ',' << map_type << ',' << hash_name << ',' << d << '\n';
    }
}