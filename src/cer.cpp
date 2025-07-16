#include "../include/cer.h"
#include <queue>
#include <set>

void cer::add_rate(const std::string src, const std::string des, double rate) {
    if(rate <= 0)
    {
        throw std::runtime_error("error: rate should be positive.");
    }
    cer_list[src][des] = rate;
    cer_list[des][src] = 1.0 / rate;
}

double cer::convert(const std::string src, const std::string des, double value) {
    if (src == des) {
        return value;
    }
    if (cer_list.find(src) == cer_list.end()) {
        throw std::runtime_error("error: src currency '" + src + "' not found in exchange data.");
    }
    
    std::queue<std::pair<std::string, double>> q;
    std::set<std::string> visited;

    q.push({src, 1.0});
    visited.insert(src);

    while (!q.empty()) {
        auto [current_currency, cumulative_rate] = q.front();
        q.pop();

        if (cer_list.count(current_currency)) {
            for (const auto& edge : cer_list.at(current_currency)) {
                const std::string& neighbor_currency = edge.first;
                const double& direct_rate = edge.second;

                if (neighbor_currency == des) {
                    return value * cumulative_rate * direct_rate;
                }

                if (visited.find(neighbor_currency) == visited.end()) {
                    visited.insert(neighbor_currency);
                    double new_cumulative_rate = cumulative_rate * direct_rate;
                    q.push({neighbor_currency, new_cumulative_rate});
                }
            }
        }
    }
    throw std::runtime_error("error: Conversion path not found from " + src + " to " + des);
}

