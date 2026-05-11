#include "Metrics.h"
#include <iostream>
#include <iomanip>

void Metrics::reset() {
    comparisons         = 0;
    swaps               = 0;
    memAccesses         = 0;
    maxRecursionDepth   = 0;
    auxMemBytes         = 0;
    timeNs              = 0;
}

void Metrics::print(const std::string& label) const {
    if (!label.empty()) std::cout << "--- " << label << " ---\n";
    std::cout << std::left
                << std::setw(22) << "  Comparacoes:"    << comparisons          << "\n"
                << std::setw(22) << "  Trocas:"         << swaps                << "\n"
                << std::setw(22) << "  Acessos mem.:"   << memAccesses          << "\n"
                << std::setw(22) << "  Prof. recursao:" << maxRecursionDepth    << "\n"
                << std::setw(22) << "  Mem. auxiliar:"  << auxMemBytes          << " bytes\n"
                << std::setw(22) << "  Tempo:"          << timeNs               << " ns\n";
}