#pragma once

// Week 1 Modules Foundation Headers

// Core Infrastructure & Types (Member 2 & Member 10)
#include "cppviz/types.hpp"
#include "cppviz/error.hpp"
#include "cppviz/text_util.hpp"

// Data Structures & CSV Ingestion (Member 3 & Member 4)
#include "cppviz/dataset.hpp"
#include "cppviz/csv_loader.hpp"

// Statistical Computations & Axis Scaling (Member 5 & Member 6)
#include "cppviz/stats.hpp"
#include "cppviz/scale.hpp"

// Canvas Buffer, Colormaps & Image Serialization (Member 7, Member 8 & Member 9)
#include "cppviz/canvas.hpp"
#include "cppviz/image_export.hpp"
#include "cppviz/colormap.hpp"

// Week 2 Modules — Preprocessing, Drawing & Categorical Scales
#include "cppviz/validation.hpp"         // Member 2
#include "cppviz/preprocess.hpp"         // Member 3
#include "cppviz/groupby.hpp"            // Member 4
#include "cppviz/summary.hpp"            // Member 5
#include "cppviz/ticks.hpp"              // Member 6
#include "cppviz/draw.hpp"               // Member 7
#include "cppviz/font.hpp"               // Member 8
#include "cppviz/categorical_scale.hpp"  // Member 10

// Week 3 Modules — Layouts, Themes, Annotations & Plot Types
#include "cppviz/figure.hpp"             // Member 1
#include "cppviz/theme.hpp"              // Member 2
#include "cppviz/axes.hpp"               // Member 3
#include "cppviz/labels.hpp"             // Member 4
#include "cppviz/pointplot.hpp"          // Member 5
#include "cppviz/heatmap.hpp"            // Member 6
#include "cppviz/swarmplot.hpp"          // Member 7
#include "cppviz/annotation.hpp"         // Member 8
#include "cppviz/colorbar.hpp"           // Member 9
#include "cppviz/legend.hpp"             // Member 10

// Week 4 Modules — Single-Call Quick API
#include "cppviz/quick.hpp"              // Member 1      

namespace cppviz {

/// Returns the library version string (e.g., "1.0.0").
const char* version();

} // namespace cppviz