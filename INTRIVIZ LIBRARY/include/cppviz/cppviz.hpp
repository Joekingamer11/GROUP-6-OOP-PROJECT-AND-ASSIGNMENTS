#pragma once

// Week 1 Foundation Headers
#include "cppviz/types.hpp"
#include "cppviz/error.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/csv_loader.hpp"
#include "cppviz/stats.hpp"
#include "cppviz/scale.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/image_export.hpp"
#include "cppviz/colormap.hpp"
#include "cppviz/text_util.hpp"

// Week 2 Data Processing & Drawing Headers
#include "cppviz/categorical_scale.hpp"
#include "cppviz/validation.hpp"
#include "cppviz/groupby.hpp"
#include "cppviz/summary.hpp"
#include "cppviz/ticks.hpp"
#include "cppviz/draw.hpp"
#include "cppviz/font.hpp"
#include "cppviz/preprocess.hpp"

// Week 3 Axes, Themes, Layout & Plot Headers
#include "cppviz/theme.hpp"
#include "cppviz/axes.hpp"
#include "cppviz/labels.hpp"
#include "cppviz/annotation.hpp"
#include "cppviz/pointplot.hpp"
#include "cppviz/swarmplot.hpp"
#include "cppviz/heatmap.hpp"
#include "cppviz/colorbar.hpp"
#include "cppviz/legend.hpp"
#include "cppviz/figure.hpp"

namespace cppviz {

/// Returns the library version string.
const char* version();

} // namespace cppviz