#pragma once

// Week 1 Modules Foundation Headers

// Core Infrastructure & Types (Member 2 & Member 10)
#include "intriviz/types.hpp"
#include "intriviz/error.hpp"
#include "intriviz/text_util.hpp"

// Data Structures & CSV Ingestion (Member 3 & Member 4)
#include "intriviz/dataset.hpp"
#include "intriviz/csv_loader.hpp"

// Statistical Computations & Axis Scaling (Member 5 & Member 6)
#include "intriviz/stats.hpp"
#include "intriviz/scale.hpp"

// Canvas Buffer, Colormaps & Image Serialization (Member 7, Member 8 & Member 9)
#include "intriviz/canvas.hpp"
#include "intriviz/image_export.hpp"
#include "intriviz/colormap.hpp"

// Week 2 Modules — Preprocessing, Drawing & Categorical Scales
#include "intriviz/validation.hpp"         // Member 2
#include "intriviz/preprocess.hpp"         // Member 3
#include "intriviz/groupby.hpp"            // Member 4
#include "intriviz/summary.hpp"            // Member 5
#include "intriviz/ticks.hpp"              // Member 6
#include "intriviz/draw.hpp"               // Member 7
#include "intriviz/font.hpp"               // Member 8
#include "intriviz/categorical_scale.hpp"  // Member 10

// Week 3 Modules — Layouts, Themes, Annotations & Plot Types
#include "intriviz/figure.hpp"             // Member 1
#include "intriviz/theme.hpp"              // Member 2
#include "intriviz/axes.hpp"               // Member 3
#include "intriviz/labels.hpp"             // Member 4
#include "intriviz/pointplot.hpp"          // Member 5
#include "intriviz/heatmap.hpp"            // Member 6
#include "intriviz/swarmplot.hpp"          // Member 7
#include "intriviz/annotation.hpp"         // Member 8
#include "intriviz/colorbar.hpp"           // Member 9
#include "intriviz/legend.hpp"             // Member 10

// Week 4 Modules — Single-Call Quick API
#include "intriviz/quick.hpp"              // Member 1      

namespace intriviz {

/// Returns the library version string (e.g., "1.0.0").
const char* version();

} // namespace intriviz