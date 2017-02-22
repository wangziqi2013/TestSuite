
#include "plot_suite.h"

Color RED_COLOR_SCHEME[] = {
  Color{"#fff7ec"}, 
  Color{"#fee8c8"}, 
  Color{"#fdd49e"}, 
  Color{"#fdbb84"}, 
  Color{"#fc8d59"}, 
  Color{"#ef6548"}, 
  Color{"#d7301f"}, 
  Color{"#b30000"}, 
  Color{"#7f0000"},
};

Color BLUE_COLOR_SCHEME[] = {
  Color{"#fff7fb"}, 
  Color{"#ece7f2"}, 
  Color{"#d0d1e6"}, 
  Color{"#a6bddb"}, 
  Color{"#74a9cf"}, 
  Color{"#3690c0"}, 
  Color{"#0570b0"}, 
  Color{"#045a8d"}, 
  Color{"#023858"},
};

ChartParameter default_chart_param{
  // Width 
  8,   
  // Height
  5,   
  // X tick font size
  16,  
  // y tick font size
  14,
  // x title font size  
  20,
  // y title font size
  20,
  // Legend font size
  12,
  // Legend position
  "upper left"
};
