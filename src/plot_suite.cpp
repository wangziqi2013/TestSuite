
#include "plot_suite.h"

PythonInterpreter *PythonInterpreter::p = new PythonInterpreter{};

// This is the maximum number of bars in a group
const size_t MAX_COLOR_COUNT = 7;

Color RED_COLOR_SCHEME[] = {
  //Color{"#fff7ec"}, 
  //Color{"#fee8c8"}, 
  Color{"#fdd49e"}, 
  Color{"#fdbb84"}, 
  Color{"#fc8d59"}, 
  Color{"#ef6548"}, 
  Color{"#d7301f"}, 
  Color{"#b30000"}, 
  Color{"#7f0000"},
};

Color BLUE_COLOR_SCHEME[] = {
  //Color{"#fff7fb"}, 
  //Color{"#ece7f2"}, 
  Color{"#d0d1e6"}, 
  Color{"#a6bddb"}, 
  Color{"#74a9cf"}, 
  Color{"#3690c0"}, 
  Color{"#0570b0"}, 
  Color{"#045a8d"}, 
  Color{"#023858"},
};

// This is the marker we use for line plots
char MARKER_SCHEME[] = {
  'o', 'x', 'v', '*', '+', '>', '^',
};

ChartParameter default_chart_param{
  // Width 
  8.0L,   
  // Height
  5.0L,   
  // X tick font size
  18,  
  // y tick font size
  16,
  // x title font size  
  20,
  // y title font size
  20,
  // Legend font size
  12,
  // y limit ratio
  1.1L,
  // Legend position
  //"right"
	//"center left"
	//"upper right"
	//"lower right"
	//"best"
	//"center"
	//"lower left"
	//"center right"
	//"upper left"
	//"upper center"
	//"lower center"
  "upper left"
  //"best"
};

// import statements
const char *PYTHON_IMPORT_PROLOGUE = \
  "import sys\n"
  "import matplotlib as mpl\n"
  "import matplotlib.pyplot as plot\n"
  "import matplotlib.ticker as ticker\n\n";

// Latex statements
const char *PYTHON_TEX_PROLOGUE = \
  "mpl.rcParams['ps.useafm'] = True\n"
  "mpl.rcParams['pdf.use14corefonts'] = True\n"
  "mpl.rcParams['text.usetex'] = True\n"
  "mpl.rcParams['text.latex.preamble'] = [\n"
  "  r'\\usepackage{siunitx}',\n"
  "  r'\\sisetup{detect-all}',\n"
  "  r'\\usepackage{helvet}',\n"
  "  r'\\usepackage{sansmath}',\n"
  "  r'\\sansmath'\n"
  "]\n\n";

/*
 * RoundUpToPoint5() - Round up a float number to the nearest 0.5
 */
double RoundUpToPoint5(double num) {
  // First make it 10 times larger such that we could perform
  // integer divisio on the number
  num *= 10L;
  size_t temp = static_cast<size_t>(num);
  
  // If the number is, say, 15.55, then 155.5 will be converted to
  // 155, and the result is incorrect
  if(static_cast<double>(temp) < num) {
    temp++; 
  }
  
  // If it is not already a multiple of 5, we need to round it up
  // to the nearest multiple of 5
  if(temp % 5 != 0) {
    temp = ((temp + 4) / 5) * 5;
  }
  
  // And then convert it back
  return static_cast<double>(temp) / 10L;
}

/*
 * PrintListDouble() - Prints a list of doubles in Python list format
 */
void PrintListDouble(const std::vector<double> data_list, 
                     Buffer *buffer_p) {
  buffer_p->Append('[');
  for(double data : data_list) {
    buffer_p->Printf("%f, ", data); 
  }
  
  buffer_p->Append(']');
  
  return;
}
