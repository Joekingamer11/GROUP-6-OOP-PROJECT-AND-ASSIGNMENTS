#pragma once

#include <string>
#include <vector>
#include "cppviz/dataset.hpp"
#include "cppviz/figure.hpp"

namespace cppviz {

/// Generates a complete swarm plot figure in a single call.
/// \param data The Dataset containing categorical and numerical data.
/// \param x_column Category text column name.
/// \param y_column Numeric value column name.
/// \param title Optional title string displayed at the top.
/// \return A Figure object containing the rendered plot.
Figure quick_swarmplot(const Dataset& data,
                        const std::string& x_column,
                        const std::string& y_column,
                        const std::string& title = "");

/// Generates a complete point plot figure with 95% bootstrap CI bars in a single call.
/// \param data The Dataset containing categorical and numerical data.
/// \param x_column Category text column name.
/// \param y_column Numeric value column name.
/// \param title Optional title string displayed at the top.
/// \return A Figure object containing the rendered plot.
Figure quick_pointplot(const Dataset& data,
                        const std::string& x_column,
                        const std::string& y_column,
                        const std::string& title = "");

/// Generates a correlation heatmap figure of specified numeric columns in a single call.
/// \param data The Dataset containing numeric columns.
/// \param numeric_columns List of numeric column names to include in the correlation matrix.
/// \param title Optional title string displayed at the top.
/// \return A Figure object containing the rendered plot.
Figure quick_heatmap(const Dataset& data,
                      const std::vector<std::string>& numeric_columns,
                      const std::string& title = "");

} // namespace cppviz