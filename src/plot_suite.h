
#pragma once

#ifndef _PLOT_SUITE_H
#define _PLOT_SUITE_H

#include "Python.h"

#include "buffer.h"
#include "test_suite.h"

// Round up a double to the nearest 0.5
extern double RoundUpToPoint5(double num);
// This prints a list of doubles into a buffer in python list format
extern void PrintListDouble(const std::vector<double> data_list, 
                            Buffer *buffer_p);
  
/*
 * class PythonInterpreter - The class for constructing and destructing
 *                           the python interpreter states
 *
 * The reason that we need an empty class to represent the interpreter is that
 * we could not call python initialization routine and termiantion routine 
 * for more than once in the entire duration of this process, because otherwise
 * the second initialization would lead to segmentation fault.
 */                          
class PythonInterpreter {
  // Everything was made private such that the user could not
  // access it
 private:
  /*
   * Constructor
   */
  PythonInterpreter() {
    // Initialize python before entering main() function
    Py_Initialize();
    
    // Register finalize to be called at exit
    atexit(Py_Finalize);
    
    return;
  }
  
  // Singleton
  static PythonInterpreter *p;
};

/*
 * class Color - RGB color scheme
 */
class Color {
 public:
  uint8_t b; 
  uint8_t g;
  uint8_t r;
  
  // This is used as a padding and is not used
  uint8_t zero;
  
  /*
   * Constructor
   */
  Color() :
    b{0},
    g{0},
    r{0},
    zero{0}
  {}
  
  /*
   * Constructor
   *
   * Please note that although the internal data is stored using BGR order
   * we still pass arguments using RGB order
   */
  Color(uint8_t p_r, uint8_t p_g, uint8_t p_b) :
    b{p_b},
    g{p_g},
    r{p_r},
    zero{0}
  {}
  
  // RRGGBB is of length 6 
  static constexpr size_t RGB_STRING_LENGTH = 6;
  
  /*
   * Constructor - Use a string to initialize
   *
   * We assume the string is always of format #RRGGBB and reject all other
   * formats by thowing an error
   */
  Color(const char *s) throw(const char *) {
    assert(s != nullptr);
    
    // #RRGGBB must be of length 7
    if(strlen(s) != (RGB_STRING_LENGTH + 1)) {
      assert(false);
      throw "Invalid color format: Incorrect length";
    }
    
    // The string must begin with '#' 
    if(*s != '#') {
      assert(false);
      throw "Invalid color format: Expecting '#'";
    } else {
      s++; 
    }
    
    // We use this to hold value converted from 6 hex digits
    uint8_t hex_value[RGB_STRING_LENGTH];
    for(size_t i = 0;i < RGB_STRING_LENGTH;i++) { 
      char ch = s[i];
      
      if(ch >= '0' && ch <= '9') {
        hex_value[i] = static_cast<uint8_t>(ch) - static_cast<uint8_t>('0'); 
      } else if(ch >= 'a' && ch <= 'f') {
        hex_value[i] = \
          10 + static_cast<uint8_t>(ch) - static_cast<uint8_t>('a');  
      } else if(ch >= 'A' && ch <= 'F') {
        hex_value[i] = \
          10 + static_cast<uint8_t>(ch) - static_cast<uint8_t>('A');  
      } else {
        assert(false);
        throw "Invalid color format: Unknown character"; 
      }
    }
    
    // Then ser RGB fields
    r = (hex_value[0] << 4) + hex_value[1];
    g = (hex_value[2] << 4) + hex_value[3];
    b = (hex_value[4] << 4) + hex_value[5];
    zero = 0;
    
    return;
  }
  
  /*
   * AppendToBuffer() - Appends the value of the color to the buffer
   *
   * The color will be appended like this: #RRGGBB where RR GG BB are 8 bit
   * color values in hex
   */
  void AppendToBuffer(Buffer *buffer_p) const {
    buffer_p->Append('#');
    
    // Then Append RGB respectively
    buffer_p->Printf("%02X", static_cast<uint32_t>(r));
    buffer_p->Printf("%02X", static_cast<uint32_t>(g));
    buffer_p->Printf("%02X", static_cast<uint32_t>(b));
    
    return;
  }
};

// These are predefined color schemes
// These schemes only have a limited set of colors, so if the 
// scale of the plot is too large then the user should provide 
// a customized one

extern Color RED_COLOR_SCHEME[];
extern Color BLUE_COLOR_SCHEME[];
extern Color BROWN_COLOR_SCHEME[];
extern Color MIXED_COLOR_SCHEME[];
extern Color GRAY_SCALE_SCHEME[];

// This is the predefined marker scheme for line plots
extern char MARKER_SCHEME[];
// This is the predefined filling scheme for bar plots
extern char HATCH_SCHEME[];

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class BarGroup - This class represents a group in the graph
 */
class BarGroup {
 private:
  // The x tick of the group. This will be drawn on the 
  // x axis. Its font size of x_tick_font_size
  std::string title; 
   
  // The list of data the bar chart will be showing
  // The size of the data list need not be the same 
  // across groups, because the drawing routine could recognize it
  // correctly
  std::vector<double> data_list; 

 public:
  
  // The list of positions. This list must be of the same 
  // length of data list 
  std::vector<double> pos_list;

  // This is a list of errors which will be displayed as the 
  // error bar in tha bar chart
  // This member is optional - we could choose not to add any
  // error, in which case the erorr bar will NOT be printed 
  // (it is different from havinf an error bar of 0)

  // This is the error less than the value of the bar
  std::vector<double> error_below_list;
  std::vector<double> error_above_list;
  
  // The position of the X axis tick 
  // Note that this is the central of the x tick
  // label as required by matplotlib 
  double x_tick_pos; 
   
 public: 
  /*
   * Constructor
   */
  BarGroup(const char *p_title) :
    title{p_title},
    data_list{}, 
    pos_list{},
    error_below_list{},
    error_above_list{},
    x_tick_pos{} 
  {}
  
  /*
   * Constructor
   */
  BarGroup(const std::string p_title) :
    BarGroup{p_title.c_str()} 
  {}
  
  /*
   * Append() - Appends data into the data list
   *
   * This function is templatized such that we could append an array
   * of any type, and they will be converted to double type when we append them
   */
  template <typename T>
  void Append(size_t count, const T *data_p) {
    // Convert every element to double and then push back to
    // the data vector
    for(size_t i = 0;i < count;i++) {
      data_list.push_back(static_cast<double>(data_p[i]));
    }
    
    return; 
  }

  /*
   * AppendError() - Appends an error into the corresponding position
   */
  template <typename T>
  void AppendError(size_t count, const std::pair<T, T> *data_p) {
    for(size_t i = 0;i < count;i++) {
      error_below_list.push_back(data_p[i].first);
      error_above_list.push_back(data_p[i].second);
    }

    return;
  }
  
  /*
   * Append() - Append a vector into the current data list
   *
   * This function is a wrapper over the array based Append()
   */
  template <typename T>
  inline void Append(const std::vector<T> &v) {
    // For vector it is guaranteed that all elements are stored in 
    // a consecutive chunk of memory
    Append(v.size(), &v[0]);
    
    return;
  }

  /*
   * AppendError() - Appends a vector of error points
   */
  template <typename T>
  inline void AppendError(const std::vector<std::pair<T, T>> &v) {
    AppendError(v.size(), &v[0]);

    return;
  }
  
  /*
   * Append() - This function appends a single element into the data list
   */
  template <typename T>
  inline void Append(T data) {
    Append(1, &data);
    
    return; 
  }

  /*
   * AppendError() - Appends a single error point
   */
  template <typename T>
  inline void AppendError(std::pair<T, T> data) {
    AppendError(1, &data);

    return;
  }
  
  /*
   * Reset() - Clears the data list and set its length to 0, i.e. no data
   */ 
  inline void Reset() {
    data_list.clear();
    
    return;
  }
  
  /*
   * GetSize() - Returns the size of the bar group's data vector 
   */
  inline size_t GetSize() const {
    return data_list.size(); 
  }
  
  /*
   * GetDataList() - Returns a reference of the data list
   */
  inline std::vector<double> &GetDataList() {
    return data_list;
  } 
  
  inline const std::vector<double> &GetDataList() const {
    return data_list;
  }
  
  /*
   * The following allows the syntactic sugar for(it : obj) to be applied on
   * this object
   */
  
  inline std::vector<double>::iterator begin() {
    return data_list.begin();
  }
  
  inline std::vector<double>::iterator end() {
    return data_list.end();
  }
  
  inline std::vector<double>::const_iterator begin() const {
    return data_list.begin();
  }
  
  inline std::vector<double>::const_iterator end() const {
    return data_list.end();
  }
  
  /*
   * GetTitle() - Returns the title of this bar group which is usually used
   *              as the X axis tick
   */
  inline const std::string &GetTitle() const {
    return title; 
  }
  
  inline std::string &GetTitle() {
    return title; 
  }
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class ChartParameter - This class stores the metadata that is applicable
 *                        in general to many kinds of plots
 *
 * This class should be kept as a POD type, such that a simple memcpy() could
 * duplicate its content
 */
class ChartParameter {
 public:
  // Total size of thje plot (in inches)
  double width;
  double height;
  
  // The font size of ticks (i.e. interval markers) on both axes
  uint32_t x_tick_font_size;
  uint32_t y_tick_font_size;
  
  // The font size of titles on both axes
  uint32_t x_font_size;
  uint32_t y_font_size;
  
  // Legend font size
  uint32_t legend_font_size;

  // This is the font size for number or text over each bar in the bar chart
  uint32_t overbar_font_size;
  
  // This is for determining the value of y limit
  // We multiply this value and the maximum y value as the y limit
  double y_limit_ratio;
  
  // This is the position of the legend; expressed in the format that
  // matplotlib could recognize (i.e. we do not use C++ constants for 
  // this field)
  std::string legend_position;

  // The color and size of the error bar
  // Color is a HEX representation of the color: #RRGGBB
  std::string error_bar_color;
  uint32_t error_bar_size;
  uint32_t error_bar_line_width;
  
  /*
   * Constructor
   */
  ChartParameter(double p_width, 
                 double p_height,
                 uint32_t p_x_tick_font_size,
                 uint32_t p_y_tick_font_size,
                 uint32_t p_x_font_size,
                 uint32_t p_y_font_size,
                 uint32_t p_legend_font_size,
                 uint32_t p_overbar_font_size,
                 double p_y_limit_ratio,
                 const std::string &p_legend_position,
                 const std::string &p_error_bar_color,
                 uint32_t p_error_bar_size,
                 uint32_t p_error_bar_line_width) :
    width{p_width},
    height{p_height},
    x_tick_font_size{p_x_tick_font_size},
    y_tick_font_size{p_y_tick_font_size},
    x_font_size{p_x_font_size},
    y_font_size{p_y_font_size},
    legend_font_size{p_legend_font_size},
    overbar_font_size{p_overbar_font_size},
    y_limit_ratio{p_y_limit_ratio},
    legend_position{p_legend_position},
    error_bar_color{p_error_bar_color},
    error_bar_size{p_error_bar_size},
    error_bar_line_width{p_error_bar_line_width}
  {}
  
  /*
   * Copy Constructor
   */
  ChartParameter(const ChartParameter &other) :
    width{other.width},
    height{other.height},
    x_tick_font_size{other.x_tick_font_size},
    y_tick_font_size{other.y_tick_font_size},
    x_font_size{other.x_font_size},
    y_font_size{other.y_font_size},
    legend_font_size{other.legend_font_size},
    overbar_font_size{other.overbar_font_size},
    y_limit_ratio{other.y_limit_ratio},
    legend_position{other.legend_position},
    error_bar_color{other.error_bar_color},
    error_bar_size{other.error_bar_size},
    error_bar_line_width{other.error_bar_line_width}
  {}
};

// This is the default parameter for charts
// We could customize one by ourselves but this should be sufficient for
// most charts
extern ChartParameter default_chart_param;
extern const size_t MAX_COLOR_COUNT;

// import statements
extern const char *PYTHON_IMPORT_PROLOGUE;
  
// Latex statements
extern const char *PYTHON_TEX_PROLOGUE;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class BarChart - Represents a bar chart, including its data and metadata
 *                  such as graph title,  
 */
class BarChart {
  void RoundUpTest();
  void PlotTest();
  
  friend void RoundUpTest();
  friend void PlotTest();
 private:
  // This stores data and group title.
  // Note that it is not required that all group have the same length
  // However, when drawing the diagram, we will calculate the maximum
  // number of elements among all groups, and use this value to determine
  // the width of each group 
  std::vector<BarGroup> group_list;
  
  // This is a pointer to the color scheme, which could be predefined
  // or customized
  const Color *color_scheme_p;
  
  // The following are parameters that could be tweaked but usually kept
  // as-is
  ChartParameter param;
  
  // The following two are X and Y labels
  // By default they are not set, and also will not be printed
  std::string x_label;
  std::string y_label;
  
  // This is the buffer in which we print the pythob script into
  Buffer buffer;
  // If we also draws the legend then this is the buffer
  Buffer legend_buffer;
  
  // This is the width of the bar
  // It will be set in SetPosition()
  double bar_width;
  
  // This is set either using the value returned by GetYUpperLimit() or 
  // specify a value manually
  double y_upper_limit;
  double y_lower_limit;
  
  // This is the mininum value for y upper limit
  double y_upper_limit_minimum;
  
  // Element i in this list is the name that will be printed on the 
  // legend for bar i in the plot
  std::vector<std::string> bar_name_list;
  
  // This is the label of X and Y axis
  // If it is empty string we do not draw them
  std::string x_axis_label;
  std::string y_axis_label;
  
  // Whether or not to draw legend. By default this is turned on
  bool draw_legend_flag;
  
  // If this is true then legend items stack up vertically
  // Otherwise they are aligned horizontally
  // This is by default set to true
  bool legend_vertical_flag;

  // Whether we draw grid on the plot
  bool draw_x_grid_flag;
  bool draw_y_grid_flag;

  // Whether we draw an error bar. If this is turned on then the error
  // must be added
  bool draw_error_flag;

  // Whether we draw a hatch on each bar in the group
  bool draw_hatch_flag;

  int legend_col_num;
 private: 
  
  /*
   * GetBarWidth() - Returns the width of each bar in the graph
   *
   * We compute the width of the bar by the following process:
   *   (1) Sum the total number of data points in all groups, let it be s 
   *   (2) The width of each bar is (Total width / (s + 2)) 
   * The essence of this process is that we need to add two padding "bars"
   * at the left and right of the diagram 
   *
   * This function always returns a positive number, even in the corner case
   * that the number of bar group is 0 
   */ 
  double GetBarWidth() const {
    // Start with 1 as the right padding 
    size_t bar_count = 1UL;
    for(const BarGroup &bg : group_list) {
      bar_count += (bg.GetSize() + 1); 
    } 
    
    // Then divide width by the bar count 
    return param.width / static_cast<double>(bar_count); 
  } 
  
  /*
   * GetYLowerLimit() - Returns the lower limit of Y
   *
   * This will be 0 if all data points are >= 0; Otherwise will be
   * set to the minimum negative value
   */
  double GetYLowerLimit() {
    double min = 0.0L;
    
    for(const BarGroup &bg : group_list) {
      for(double data : bg) {
        if(data < min) {
          min = data; 
        }
      }
    }
    
    // Most likely be 0
    return min;
  }
  
  /*
   * GetYUpperLimit() - Returns the y limit
   *
   * We compute y upper limit by multiplying the ratio in param object with the
   * maximum value of all bars, and then round it up to .5
   */
  double GetYUpperLimit() {
    double max = 0.0;
    bool first_time = true;
    
    // Iterate over each data point in each bar group
    for(const BarGroup &bg : group_list) {
      for(double data : bg) {
        if(first_time == true) {
          first_time = false;
          max = data;
        } else {
          if(data > max) {
            max = data;
          }
        }
      }
    }
    
    // Y limit is the max Y value multiplied by a constant defined in
    // the param object
    max *= param.y_limit_ratio;
    
    double rounded_value = RoundUpToPoint5(max);
    if(rounded_value < y_upper_limit_minimum) {
      rounded_value = y_upper_limit_minimum;
    }
    
    return rounded_value;
  }
  
  /*
   * SetPosition() - Sets the position of all bars and X axis ticks
   *
   * We loop through all bar groups and assign them the position of each bar
   * and also a position of the X tick (which is the position of its central
   * point)
   */ 
  void SetPosition() {
    // This is the basic unit for drawing the diagram
    const double width = GetBarWidth();
    // Also assign it to the instance wide bar width for reuse
    bar_width = width;
    
    // This is the current position of the bar group
    // Note that this position is the left padding
    double current_pos = 0.0L;
    for(BarGroup &bg : group_list) {
      // This is the size of the bar group (i.e. number of data points)
      size_t bg_size = bg.GetSize();
      
      double start_pos = current_pos + width;
      double end_pos = start_pos + static_cast<double>(bg_size) * width;
      
      // The position of the x label is the midpoint of these twp
      bg.x_tick_pos = (start_pos + end_pos) / 2;
      
      // Then prepare space for the position list for each bar
      bg.pos_list.resize(bg_size, 0.0L);
      for(size_t i = 0;i < bg_size;i++) {
        // The position of each bar is 
        bg.pos_list[i] = (start_pos + width * i); 
      }
      
      // Update the current position for the next group
      current_pos = end_pos;
    }
    
    return;
  } 
  
  /*
   * GetMaximumGroupSize() - Returns the maximum group size
   *
   * Since we do not restrict the size of each bar group, they could be
   * of different size. In order to uniformly draw the graph we need to 
   * calculate the maximum size of the bar group to determine the width
   * of each bargroup
   *
   * If there is not yet any bar group this function returns -1UL 
   */
  size_t GetMaximumGroupSize() {
    // Corner case 
    if(group_list.size() == 0UL) {
      dbg_printf("Bar group is empty while calculating the"
                 " maximum group size\n"); 
      
      return -1UL; 
    } 
    
    // The max is set to the first here, and we update it later 
    size_t max_size = group_list[0].GetSize(); 
    
    // Loop over each bar group and get its size 
    for(const BarGroup &bg : group_list) {
      size_t current_size = bg.GetSize();
      
      if(current_size > max_size) {
        max_size = current_size; 
      }  
    }
    
    return max_size; 
  }
  
  /*
   * PrintPrologue() - Prints the prologue of the drawing script
   */
  void PrintPrologue() {
    buffer.Append(PYTHON_IMPORT_PROLOGUE);
    buffer.Append(PYTHON_TEX_PROLOGUE);
    
    return;
  }

  /*
   * RoundToSignificantDigits() - Round the given number to a given number of
   *                              significant digits
   */
  static void RoundToSignificantDigits(const double num, 
                                       const int digits, 
                                       char output[]) {
    assert(digits > 0);
    if(digits <= 0) {
      throw "Significant digits must be > 0";
    }

    // We first print #digit digits after the point to the buffer
    sprintf(output, "%.*lf", digits, num);

    // We must be able to see the dot, because we always print digits after 
    // the dot
    int dot_index;
    for(int i = 0;;i++) {
      char ch = output[i];
      if(ch == '\0') {
        assert(false);
        throw "Did not see dot in the floating point number";
      } else if(ch == '.') {
        dot_index = i;
        break;
      }
    }

    if(dot_index >= digits) {
      // Fill all remaining bits before the dot as digit 0
      for(int i = digits;i < dot_index;i++) {
        output[i] = '0';
      }

      // Terminate the string
      output[dot_index] = '\0';
    } else {
      // In this branch, we just need to find the last significant digit and 
      // set the char after it as NUL
      output[digits + 1] = '\0';
    }

    return;
  }
  
  /*
   * PrintBarPlot() - This is middle part of the plot script that draws
   *                  bars using data points
   */
  void PrintBarPlot() {
    // This defines the size of the diagram
    buffer.Printf("fig = plot.figure(figsize=(%f, %f))\n", 
                  param.width, 
                  param.height);
    // This obtains the plot object
    buffer.Append("ax = fig.add_subplot(111)\n\n");

    // We need to start with the first to the last in each group
    // Currently because t=of color configuration problems we only
    // support a maximum of certain number of bars in a group
    size_t max_group_size = GetMaximumGroupSize();

    // We start with the first bar in each group, and then the second, etc.
    for(size_t i = 0;i < max_group_size;i++) {
      // Need these two buffer to represent data list and pos list
      Buffer data_buffer;
      Buffer pos_buffer;
      // This is a 2 * N array, because we have two numbers for each bar in the
      // bar group
      Buffer error_buffer;
      
      data_buffer.Append('[');
      pos_buffer.Append('[');
      error_buffer.Append("[[");
      
      // Then for each group check their i-th bar, and if it exists
      // we add it to the buffer
      for(const BarGroup &bg : group_list) {
        if(i < bg.GetSize()) {
          data_buffer.Printf("%f, ", bg.GetDataList().at(i)); 
          pos_buffer.Printf("%f, ", bg.pos_list.at(i));

          if(draw_error_flag == true) {
            error_buffer.Printf("%f, ", bg.error_below_list.at(i));
          }
        }
      }

      error_buffer.Append("], [");
      for(const BarGroup &bg : group_list) {
        if(i < bg.GetSize()) {
          if(draw_error_flag == true) {
            error_buffer.Printf("%f, ", bg.error_above_list.at(i));
          }
        }
      }
      
      // Close the two Python lists
      data_buffer.Append(']');
      pos_buffer.Append(']');
      error_buffer.Append("]]");
      
      // First of the line calling bar() method
      // and then save the rect object on variable "rect"
      buffer.Append("rect_list = ax.bar(");
      
      // Add pos list
      buffer.Append(pos_buffer);
      buffer.Append(", ");
      
      // Add data list
      buffer.Append(data_buffer);
      
      // And then add width and color
      buffer.Printf(", %f, color=\"", bar_width);
      color_scheme_p[i].AppendToBuffer(&buffer);
      
      buffer.Printf("\", label=\"");
      buffer.Append(bar_name_list[i].c_str());
      buffer.Append("\"");
      
      // This adds an extra argument: yerr to the draw routine
      // and it carries the error buffer we just generated
      if(draw_error_flag == true) {
        buffer.Append(", yerr=");
        buffer.Append(error_buffer);
        // Append a dict object to the parameter list
        // which specifies the color and cap size of the error bar
        buffer.Printf(", error_kw={\"ecolor\": \"%s\""
                      ", \"capsize\": %u"
                      ", \"elinewidth\": %u}",
                      param.error_bar_color.c_str(),
                      param.error_bar_size,
                      param.error_bar_line_width);
      }

      // If we draw the filling pattern then just use the one 
      // character hatch
      if(draw_hatch_flag == true) {
        char hatch = HATCH_SCHEME[i];
        if(hatch == '\\') {
          buffer.Printf(", hatch=\"\\%c\"", hatch);
        } else {
          buffer.Printf(", hatch=\"%c\"", hatch);
        }
      }

      buffer.Printf(")\n");
      
      int j = 0;
      // We need to do that using the scale
      const double gap_between_text_and_bar = 0.015 * (y_upper_limit - y_lower_limit);
      
      for(const BarGroup &bg : group_list) {
        double gap_with_error_bar = gap_between_text_and_bar;
        // If we draw error bar, then also need to adjust the text up to
        // avoid colliding with the error bar
        // The unit to go up is the error bar length above the bar
        if(draw_error_flag == true) {
          gap_with_error_bar += bg.error_above_list[i];
        }

        char digit_buffer[256];
        RoundToSignificantDigits(bg.GetDataList()[i], 2, digit_buffer);

        if(i < bg.GetSize()) {
          // This is the drawing statement for numeric labels over each bar
          buffer.Printf("ax.text(rect_list[%d].get_x() + "
                        "rect_list[%d].get_width() / %lf, %f, \"%s\","
                        " ha='center', va='bottom', rotation='vertical',"
                        " fontsize=%u)\n", 
                        j, 
                        j, 
                        1.7,  // This is a minor tweak to adjust horizontal 
                              // position of the text
                        bg.GetDataList()[i] + gap_with_error_bar, 
                        digit_buffer, // This is printed text for value
                                      // Note: We have a dedicated argument for 
                                      // this
                        param.overbar_font_size);
          
          j++;
        }
      }
    }
    
    buffer.Append('\n');
    
    return;
  }
  
  /*
   * PrintTickPlot() - This plots ticks on the X axis
   */
  void PrintTickPlot() {
    // Set x axis ticks
    buffer.Append("ax.set_xticks([");
    // Loop over each bar group and then form a list
    for(const BarGroup &bg : group_list) {
      buffer.Printf("%f, ", bg.x_tick_pos);
    }
    
    buffer.Append("])\n");
    
    // Then set labels
    buffer.Append("ax.set_xticklabels([");
    // Loop over each bar group and then form a list
    for(const BarGroup &bg : group_list) {
      buffer.Printf("\"%s\", ", bg.GetTitle().c_str());
    }
    
    buffer.Append("])\n\n");
    
    // Then change tick font size
    buffer.Append("for label in ax.get_xticklabels():\n");
    buffer.Printf("    label.set_fontsize(%lu)\n\n", param.x_tick_font_size);
    
    // Then change tick font size
    buffer.Append("for label in ax.get_yticklabels():\n");
    buffer.Printf("    label.set_fontsize(%lu)\n\n", param.y_tick_font_size);
    
    return;
  }
  
  /*
   * PrintLabelPlot() - This prints labels in the script
   */
  void PrintLabelPlot() {
    if(x_axis_label.size() != 0UL) {
      buffer.Printf("ax.set_xlabel(\"%s\", fontsize=%lu, weight='bold')\n",
                    x_axis_label.c_str(), param.x_font_size);
    }
    
    if(y_axis_label.size() != 0UL) {
      buffer.Printf("ax.set_ylabel(\"%s\", fontsize=%lu, weight='bold')\n",
                    y_axis_label.c_str(), param.y_font_size);
    }
    
    buffer.Append('\n');
    
    return;
  }
  
  /*
   * ExecutePython() - Executes python code stored in a char array
   */
  static void ExecutePython(const char *data_p) {
    // Execute the code using Python
    PyRun_SimpleString(data_p);
    
    
    return;
  }
  
  /*
   * Verify() - This checks whether the bar chart is consistent
   *
   * We check the following:
   *   (1) The number of data points in each BarGroup should be the same
   *   (2) The number of data points in every BarGroup must equal the number
   *       of bar names
   *   (3) The number of bars in each group must be smaller than or equal to
   *       the maximum value supported (which is a global variable)
   *   (4) There is at least one group
   *   (5) The number of errors should be consistent with each other and also
   *       be consistent with the size of the bar group, if error flag is on
   *
   * If any of the above conditions fails we throw an exception
   */
  void Verify() const {
    // This checks condition (4)
    if(group_list.size() == 0UL) {
      assert(false);
      throw "There must be at least one group to draw the plot"; 
    }
    
    // Otherwise we know there is at least one group, so just get
    // its bar count and compare with all the rest
    const size_t bar_count = group_list.at(0).GetSize();
    
    // This checks condition (1)
    for(const BarGroup &bg : group_list) {
      if(bar_count != bg.GetSize()) {
        assert(false);
        throw "Inconsistent bar count in BarGroup objects";
      }

      // This checks (5) if we do want to draw error
      if(draw_error_flag == true && 
         (bar_count != bg.error_above_list.size() || \
          bar_count != bg.error_below_list.size())) {
        assert(false);
        throw "Inconsistent bar count and error points";
      }
    }
    
    // This checks condition (2)
    if(bar_count != bar_name_list.size()) {
      assert(false);
      throw "Inconsistent bar count and bar names"; 
    }
    
    // This checks condition (3)
    if(bar_count > MAX_COLOR_COUNT) {
      assert(false);
      throw "Exceeded the maximum number of bars allowed";
    }
    
    return;
  }
 
 public: 
  /*
   * Constructor
   */
  BarChart() :
    group_list{},
    color_scheme_p{MIXED_COLOR_SCHEME},
    // Use the default parameters inside the constructor
    // if we need to modify this then just modify it later
    param{default_chart_param},
    // By default should be empty
    x_label{},
    y_label{},
    buffer{},
    legend_buffer{},
    bar_width{},
    y_upper_limit{},
    y_lower_limit{},
    y_upper_limit_minimum{}, 
    bar_name_list{},
    x_axis_label{},
    y_axis_label{},
    draw_legend_flag{true},
    legend_vertical_flag{true},
    draw_x_grid_flag{false},
    draw_y_grid_flag{false},
    draw_error_flag{false},
    draw_hatch_flag{false},
    legend_col_num{1}
  {}
  
  /*
   * Destructor
   */
  ~BarChart() {}
  
  /*
   * SetLegendPosition() - Sets the position of the legend in param
   */
  inline void SetLegendPosition(const std::string &pos) {
    param.legend_position = pos;
    return;
  }

  /*
   * SetYUpperLimitMinimum() - Sets the minimum value of the upper limit
   *                          to enforce a lower bound for the range
   */
  inline void SetYUpperLimitMinimum(double value) {
    y_upper_limit_minimum = value;
    
    return; 
  }
  
  /*
   * AppendBarGroup() - Appends a new bar group into the chart
   *
   * This function will add a new bar group instance and append it after all
   * existing bar groups
   */
  template <typename T> 
  void AppendBarGroup(const std::string &x_tick, 
                      size_t data_count,
                      const T *data_p) {
    // Construct a bg object at the back of the current bg list
    group_list.emplace_back(x_tick);
    
    // This is a pointer to the bar group we just added on the back
    BarGroup *bg_p = &group_list.back();
    
    // Copy data points into the bar group
    bg_p->Append<T>(data_count, data_p);
    
    return;
  }
  
  /*
   * AppendBarGroup() - Appends a vector
   */
  template <typename T>
  inline void AppendBarGroup(const std::string &x_tick, 
                             const std::vector<T> &v) {
    // Just call the overloaded version
    AppendBarGroup<T>(x_tick, v.size(), &v[0]);
    
    return;
  }
  
  /*
   * AppendBarGroup() - Appends an empty bar group
   */
  inline void AppendBarGroup(const std::string &x_tick) {
    // Construct a bg object at the back of the current bg list
    group_list.emplace_back(x_tick);
    
    return;
  }
  
  /*
   * AppendData() - This function appends a data point into the last
   *                bar group
   *
   * If the bar group is empty then raise an exception
   */
  template<typename T>
  inline void AppendData(T data) {
    // If there is no existing bar group just report error
    if(group_list.size() == 0UL) {
      dbg_printf("Trying to append data to an empty group list\n");
      
      throw "Could not append data to an empty group list";  
    }
    
    // Get the last element and then append the data
    BarGroup *bg_p = &group_list.back(); 
    bg_p->Append<T>(data);
    
    return;
  }
  
  /*
   * AppendBarName() - Appends the name of a bar
   *
   * The name will be printed in the legend
   */
  inline void AppendBarName(const std::string &name) {
    bar_name_list.push_back(name);
    
    return;
  }

  /*
   * AppendError() - Appends a list of errors into the last bar group
   * 
   * Note that we represent errors as a pair of floating point numbers. The
   * first field is the error below the value, and the second field is the
   * error above the value
   */
  template <typename T>
  inline void AppendError(const std::vector<std::pair<T, T>> &error_list) {
    BarGroup *bg_p = &group_list.back(); 
    bg_p->AppendError<T>(error_list);

    return;
  }

  /*
   * AppendErrorAtIndex() - This function appends error bar information
   *                        at a given index
   * 
   * If index is invalid then throw exception. This function allows users to
   * append the error bar after they have all finished adding data.
   */
  template <typename T>
  inline void AppendErrorAtIndex(size_t index, 
                                 const std::vector<std::pair<T, T>> &error_list) {
    // Check whether the index is valid
    if(index >= group_list.size()) {
      assert(false);
      throw "Index exceeds the maximum number of bars in the plot";
    }

    BarGroup *bg_p = &group_list.at(index);
    bg_p->AppendError<T>(error_list);

    return;
  }

  /*
   * AppendErrorAtName() - Appends the error array into a bar group with a 
   *                       given name
   * 
   * If the name is not found we just raise an exception
   */
  template <typename T>
  inline void AppendErrorAtName(const std::string &name,
                                const std::vector<std::pair<T, T>> &error_list) {
    // This is the index of the bar group with a matching name  
    size_t index = 0;  
    for(const std::string &bg_name : bar_name_list) {
      if(bg_name == name) {
        // We have found the bar group, and now just insert the error into
        // the list of bar groups
        AppendErrorAtIndex(index, error_list);
        return;
      }

      index++;
    }

    // Fail here
    assert(false);
    throw "Bar group name not found";
  }
  
  /*
   * SetXAxisLabel() - Sets the label on the X axis
   */
  inline void SetXAxisLabel(const std::string &x_label) {
    x_axis_label = x_label;
    
    return;
  }
  
  /*
   * SetYAxisLabel() - Sets the label on the X axis
   */
  inline void SetYAxisLabel(const std::string &y_label) {
    y_axis_label = y_label;
    
    return;
  }
  
  /*
   * SetLegendFlag() - Sets whether to draw legend
   */
  inline void SetLegendFlag(bool value) {
    draw_legend_flag = value;
    
    return; 
  }

  /*
   * SetXGridFlag() - Sets whether to draw grid for X axis
   */
  inline void SetXGridFlag(bool value) {
    draw_x_grid_flag = value;

    return;
  }

  /*
   * SetYGridFlag() - Sets whether to draw grid for Y axis
   */
  inline void SetYGridFlag(bool value) {
    draw_y_grid_flag = value;

    return;
  }

  inline void SetWidth(double width) {
    param.width = width;

    return;
  }

  inline void SetLegendColumnNum(int num) {
    legend_col_num = num;

    return;
  }
  
  /*
   * SetLegendVerticalFlag() - If this is set to true then we draw all legend
   *                           items as a stack of vertical items
   */
  inline void SetLegendVerticalFlag(bool value) {
    legend_vertical_flag = value;
    
    return;
  }

  /*
   * SetColorScheme() - Sets the color scheme by giving an array of 
   *                    Color objects
   */
  inline void SetColorScheme(const Color *color_p) {
    color_scheme_p = color_p;

    return;
  }

  /*
   * SetDrawErrorFlag() - Sets whether to draw error bar
   */
  inline void SetDrawErrorFlag(bool value) {
    draw_error_flag = value;

    return;
  }

  /*
   * SetDrawHatchFlag() - Sets whether to draw the filling patterns
   */
  inline void SetDrawHatchFlag(bool value) {
    draw_hatch_flag = value;

    return;
  }
  
  /*
   * Draw() - Draw the dirgram into a given file name
   */
  void Draw(const std::string output_file_name) {
    Verify();
    buffer.Reset();
    
    SetPosition();
    
    // Get y axis upper and lower limit
    y_upper_limit = GetYUpperLimit();
    y_lower_limit = GetYLowerLimit();
    
    PrintPrologue();
    PrintBarPlot();
    PrintTickPlot();
    PrintLabelPlot();
    
    // Then print statement to set Y limit values
    buffer.Printf("ax.set_ylim(%f, %f)\n\n", y_lower_limit, y_upper_limit);
    
    // At last set legend based on the flag
    if(draw_legend_flag == true) {
      if(legend_vertical_flag == false) {
        legend_col_num = GetMaximumGroupSize();
      }

      // Draw the legends
      buffer.Printf("ax.legend(loc=\"%s\", prop={'size':%lu}, ncol=%d)\n\n",
                    param.legend_position.c_str(),
                    param.legend_font_size,
                    legend_col_num);
    }

    // If we need to draw grid as the background then just set the grids
    if(draw_x_grid_flag == true) {
      buffer.Append("ax.xaxis.grid(True)\n\n");
    }

    if(draw_y_grid_flag == true) {
      buffer.Append("ax.yaxis.grid(True)\n\n");
    }

    // This function call makes sure that the grid is under the 
    // bar chart, not above it
    buffer.Append("ax.set_axisbelow(True)\n\n");
    
    // The last step is to output the file
    buffer.Printf("plot.savefig(\"%s\", bbox_inches='tight')\n\n", 
                  output_file_name.c_str());
                  
    // Make it a C language string
    buffer.Append('\0');

    // At last execute is using the interpreter
    ExecutePython(buffer.GetCharData());
    
    return;
  }
  
  /*
   * DrawLegend() - Draws the legend in a separate plot and output to a file
   *
   * The argument "vertical" specifies whether the items should be stacked
   * vertically or aligned horizontally
   */
  void DrawLegend(const std::string &output_file_name) {
    // We do not need to verify the consistency of the data, because
    // As long as we have the label and color, hatch for each bar, it is
    // possible to draw the buffer
    //Verify();
    
    // Clear all contents of the buffer
    legend_buffer.Reset();
    
    legend_buffer.Append(PYTHON_IMPORT_PROLOGUE);
    legend_buffer.Append(PYTHON_TEX_PROLOGUE);
    // Make the figure small enough such that it can be fully covered by
    // the legend
    legend_buffer.Append("fig = plot.figure(figsize=(0.001, 0.001))\n\n"
                         "ax = fig.add_subplot(111)\n\n");

    // We need this to address color object
    int index = 0;
    // For each bar name in the bar list we draw the legend
    for(const std::string bar_name : bar_name_list) {
      // Thie creates a patch object using the color and label
      // Linewidth draw the border, and edge color must be black to make it
      // prominent; facecolor is the filling color of the block
      legend_buffer.Printf("ax.bar(0, 0, label=\"%s\", color=\"",
                           bar_name.c_str(),
                           HATCH_SCHEME[index]);
      
      // Print color into the buffer  
      color_scheme_p[index].AppendToBuffer(&legend_buffer);
      legend_buffer.Append('\"');
      
      // If we draw filling patterns on the bar, we also draw them here
      if(draw_hatch_flag == true) {
        char hatch = HATCH_SCHEME[index];
        if(hatch == '\\') {
          legend_buffer.Printf(", hatch=\"\\%c\"", hatch);
        } else {
          legend_buffer.Printf(", hatch=\"%c\"", hatch);
        }
      }
      
      // Close the function call
      legend_buffer.Append(")\n");
      
      index++;
    }
    
    legend_buffer.Append('\n');
    
    // We do not check flag for drawing legend, but we do need to check for
    // the shape of the legend
    if(legend_vertical_flag == false) {
      legend_col_num = GetMaximumGroupSize();
    }

    // Draw the legends
    // Make it always certer to overlap with the bar data
    legend_buffer.Printf("ax.legend(loc=\"%s\", prop={'size':%lu}, ncol=%d)\n\n",
                          "center",  
                          param.legend_font_size,
                          legend_col_num);
    
    legend_buffer.Printf("plot.savefig(\"%s\", bbox_inches='tight')\n\n", 
                         output_file_name.c_str());
    
    // Make it a C language string
    legend_buffer.Append('\0');
    
    // At last execute it              
    ExecutePython(legend_buffer.GetCharData());
    
    return;
  }

  /*
   * GetBuffer() - This function returns a char pointer to the internal buffer
   */
  const char *GetBuffer() const {
    return buffer.GetCharData();
  }
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class LineChart - This class represents a line chart 
 */
class LineChart {
  void LinePlotTest();
  friend void LinePlotTest();
 private:
  // This is a list of x values
  std::vector<double> x_list;
  
  // This is a list of y values
  // Each y value set is represented using a vector
  std::vector<std::vector<double>> y_list_list; 
  
  // This is a list of names for lines in the plot
  std::vector<std::string> line_name_list;
  
  // The following two labels. And they will be ignored if they
  // are empty strings
  
  // This is the label that will be displayed on the X axis
  std::string x_label;
  // This is the label that will be displayed on the Y axis
  std::string y_label;
  
  // This is the parameter value for common attributes
  ChartParameter param;
  
  // This points to the color scheme of the plot
  const Color *color_scheme_p;
  
  // These two are buffers that are used to print python code
  Buffer buffer;
  Buffer legend_buffer;
  
  // This flag determines whether we draw legend
  bool draw_legend_flag;
  
  // This flag determines whether legends are stacked together in the
  // vertical direction
  bool legend_vertical_flag;

  bool draw_x_grid_flag;
  bool draw_y_grid_flag;

  int marker_size;

 private:
   
  /*
   * Verify() - Verifies invariants
   *
   * This function checks the following conditions:
   *   (1) All Y value lists need to have the same length
   *   (2) All Y value lists need to have the same length as the X value list
   *   (3) The numbre of Y value lists must equal the number of 
   *       line names 
   *   (4) The numbre of Y value lists must not exceed the maximum number 
   *       of colors allowed
   *   (5) There must be at least one Y value list
   */
  void Verify() {
    // This checks condition (5)
    if(y_list_list.size() == 0UL) {
      assert(false);
      throw "There must be at least one line in the plot"; 
    }
    
    // Otherwise the length of the first list is the one we use
    // as the standard for checking other lists
    size_t y_count = y_list_list[0].size();
    
    // For each y list check its length against the expected length
    for(const std::vector<double> &y_list : y_list_list) {
      // This checks condition (1)
      if(y_list.size() != y_count) {
        assert(false);
        throw "The numbre of points in each line must be equal";
      }
    }
    
    // This checks condition (2)
    if(y_count != x_list.size()) {
      assert(false);
      throw "The number of x points does not match the number of y points";
    }
    
    // This checks condition (3)
    if(y_list_list.size() != line_name_list.size()) {
      assert(false);
      throw "The number of names for lines does not match"
            " the number of x lines"; 
    }
    
    // This checks condition (4)
    if(y_list_list.size() > MAX_COLOR_COUNT) {
      assert(false);
      throw "The number of lines exceeds the maximum number allowed"; 
    }
    
    return;
  }
  
  /*
   * GetYUpperLimit() - Returns the upper limit of the Y axis
   *
   * We compute the maximum value in all line points, and multiply it 
   * by a constant set in the parameter. The result is rounded up to
   the nearest .5 and then returned
   */
  double GetYUpperLimit() {
    double max;
    bool first_time = true;
    
    // For every point in every line compute the maximum
    for(const std::vector<double> &y_list : y_list_list) {
      for(double data : y_list) {
        if(first_time == true) {
          first_time = false;
          max = data;
        } else {
          if(data > max) {
            max = data;
          }
        }
      }
    }
    
    // Need to make it a little bit larger
    max *= param.y_limit_ratio;
    
    // And then round to the nearest 0.5
    return 20.0;//RoundUpToPoint5(max);
  }
  
  /*
   * GetYLowerLimit() - Returns the lower limit of the Y axis
   *
   * If all numbers are positive, then the return value is always 0.0
   * Otherwise the return value is the minimum negative
   */
  double GetYLowerLimit() {
    double min = 0.0L;
    
    for(const std::vector<double> &y_list : y_list_list) {
      for(double data : y_list) {
        if(data < min) {
          min = data; 
        }
      }
    }
    
    // Most likely be 0
    return min;
  }
  
  /*
   * PrintPrologue() - Prints import and tex prologue
   */
  void PrintPrologue() {
    buffer.Append(PYTHON_IMPORT_PROLOGUE);
    buffer.Append(PYTHON_TEX_PROLOGUE);
    
    return;
  }
  
  /*
   * ExecutePython() - Executes python code stored in a char array
   *
   * Note that the argumment must be a valid C-string (i.e. terminated 
   * by '\0') and the executor environment must have already been initialized
   */
  static void ExecutePython(const char *data_p) {
    PyRun_SimpleString(data_p);
     
    return;
  }

 public:
  /*
   * LineChart() - Draw line chart using matplotlib
   */
  LineChart() :
    x_list{},
    y_list_list{},
    line_name_list{},
    x_label{},
    y_label{},
    param{default_chart_param},
    color_scheme_p{RED_COLOR_SCHEME},
    buffer{},
    legend_buffer{},
    draw_legend_flag{true},
    legend_vertical_flag{true},
    draw_x_grid_flag{false},
    draw_y_grid_flag{false},
    marker_size{8}
  {}
  
  /*
   * Destructor
   */
  ~LineChart() {}

  /*
   * SetLegendPosition() - Sets the position of the legend in param
   */
  inline void SetLegendPosition(const std::string &pos) {
    param.legend_position = pos;
    return;
  }

  /*
   * SetColorScheme() - Sets the color scheme by giving an array of 
   *                    Color objects
   */
  inline void SetColorScheme(const Color *color_p) {
    color_scheme_p = color_p;

    return;
  }
  
  /*
   * AppendXValueList() - Adding values into the x axis data points
   */
  template <typename T>
  void AppendXValueList(size_t count, const T *data_p) {
    // Reserve enough space first to reduce memory allocation
    x_list.reserve(count + x_list.size());
    
    // Then do an element copy
    for(size_t i = 0;i < count;i++) {
      x_list.push_back(static_cast<double>(data_p[i]));
    }
    
    return;
  }
  
  /*
   * AppendXValueList() - Adding X values in a vector
   */
  template <typename T>
  inline void AppendXValueList(const std::vector<T> &data_list) {
    AppendXValueList(data_list.size(), &data_list[0]);
    
    return;
  }
  
  /*
   * AppendXValue() - Appends a X value into the x value list
   */
  template <typename T>
  void AppendXValue(T value) {
    x_list.push_back(static_cast<double>(value));
    
    return; 
  }
  
  /*
   * AppendYValueList() - This function appends a new array into Y list
   *
   * A new list is created in order to receive these values
   */
  template <typename T>
  void AppendYValueList(size_t count, T *data_p) {
    // Create a new list of y values
    NewYValueList();
    
    std::vector<double> *v_p = &y_list_list.back();
    v_p->resize(count);
    
    // Convert and copy
    for(size_t i = 0;i < count;i++) {
      (*v_p)[i] = static_cast<double>(data_p[i]);
    }
    
    return;
  }
  
  /*
   * AppendYValueList() - This function appends a new list of values into 
   *                      the y list
   */
  template <typename T>
  void AppendYValueList(const std::vector<T> data_list) {
    AppendYValueList(data_list.size(), &data_list[0]);
    
    return;
  }
  
  /*
   * AppendYValue() - Appends a new Y value into the current list
   */
  template <typename T>
  void AppendYValue(T value) {
    if(y_list_list.size() == 0) {
      assert(false); 
      throw "There is no list of Y values created yet";
    }
    
    // Push back the value into the list
    y_list_list.back().push_back(static_cast<double>(value));
    
    return;
  }
  
  /*
   * NewYValueList() - This function creates a new Y value list
   */
  inline void NewYValueList() {
    y_list_list.emplace_back();
    
    return;
  }
  
  /*
   * AppendLineName() - Appends the name of a line into the name list
   */
  inline void AppendLineName(const std::string &name) {
    line_name_list.push_back(name);
    
    return;
  }
  
  /*
   * SetYAxisLabel() - Sets the label for Y axis
   */
  inline void SetYAxisLabel(const std::string &s) {
    y_label = s;
    
    return; 
  }
  
  /*
   * SetXAxisLabel() - Sets the label for X axis
   */
  inline void SetXAxisLabel(const std::string &s) {
    x_label = s;
    
    return;
  }
  
  /*
   * SetLegendFlag() - Sets the draw_legend_flag member
   */
  inline void SetLegendFlag(bool value) {
    draw_legend_flag = value;
    
    return;
  }
  
  /*
   * SetLegendVerticalFlag() - Sets how legends are drawn
   */
  inline void SetLegendVerticalFlag(bool value) {
    legend_vertical_flag = value;
    
    return;
  }
  
  /*
   * Draw() - Draw the line plot using data
   */
  void Draw(const std::string &output_file_name) {
    // This checks whether the plot is consistent
    Verify();
    // Clear what we have drawn
    buffer.Reset();
    
    // Print import and tex statements
    PrintPrologue();
    
    // Create the figure and axis
    buffer.Printf("fig = plot.figure(figsize=(%f, %f))\n", 
                  param.width, 
                  param.height);
    buffer.Append("ax = fig.add_subplot(111)\n\n");
    
    // The index is used to address color and line name
    int index = 0;
    for(const std::vector<double> &y_list : y_list_list) {
      buffer.Append("ax.plot(");
      
      // Print x and y respectively
      PrintListDouble(x_list, &buffer);
      buffer.Append(", ");
      PrintListDouble(y_list, &buffer);
      
      // Then add color specification
      buffer.Append(", color=\"");
      color_scheme_p[index].AppendToBuffer(&buffer);
      
      // Next add label
      buffer.Printf("\", label=\"%s\", linewidth=4, marker='%c', markersize=%d)\n", 
                    line_name_list[index].c_str(), 
                    MARKER_SCHEME[index],
                    marker_size);
      
      index++;
    }
    
    buffer.Append('\n');
    
    // Set x label if it is not empty
    if(x_label.size() != 0UL) {
      buffer.Printf("ax.set_xlabel(\"%s\", fontsize=%lu, weight='bold')\n", 
                    x_label.c_str(), 
                    param.x_font_size);
    }
    
    // Set y label if it is not empty
    if(y_label.size() != 0UL) {
      buffer.Printf("ax.set_ylabel(\"%s\", fontsize=%lu, weight='bold')\n", 
                    y_label.c_str(), 
                    param.y_font_size);
    }
    
    buffer.Append('\n');
    
    // Then set tick size for X and Y axis
    
    buffer.Append("for label in ax.get_xticklabels():\n");
    buffer.Printf("    label.set_fontsize(%lu)\n\n", 
                  param.x_tick_font_size);
    
    buffer.Append("for label in ax.get_yticklabels():\n");
    buffer.Printf("    label.set_fontsize(%lu)\n\n", 
                  param.y_tick_font_size);
    
    // Get y axis upper and lower limit
    double y_upper_limit = GetYUpperLimit();
    double y_lower_limit = GetYLowerLimit();
    
    // Then print statement to set Y limit values
    buffer.Printf("ax.set_ylim(%f, %f)\n\n", y_lower_limit, y_upper_limit);
    
    // The below code is copied from class BarChart
    if(draw_legend_flag == true) {
      int legend_col_num = 1;
      if(legend_vertical_flag == false) {
        // We use the size of line names as the number of lines
        legend_col_num = line_name_list.size();
      }
      
      // Draw the legends
      buffer.Printf("ax.legend(loc=\"%s\", prop={'size':%lu}, ncol=%d)\n\n",
                    param.legend_position.c_str(),
                    param.legend_font_size,
                    legend_col_num);
    }


    buffer.Append("ax.set_xlim(0, 21)\n");

    // If we need to draw grid as the background then just set the grids
    if(draw_x_grid_flag == true) {
      buffer.Append("ax.xaxis.grid(True)\n\n");
    }

    if(draw_y_grid_flag == true) {
      buffer.Append("ax.yaxis.grid(True)\n\n");
    }
    
    // Specify output file name
    buffer.Printf("plot.savefig(\"%s\", bbox_inches='tight')\n", 
                  output_file_name.c_str());
    
    // Finish it as a string
    buffer.Append('\0');
    
    buffer.WriteToFile(stderr);
    
    // At last execute the script
    ExecutePython(buffer.GetCharData());
    
    return;
  }
  
  /*
   * DrawLegend() - Draws the legend only and output it to a file
   */
  void DrawLegend(const std::string &file_name) {
    
  }

  /*
   * SetXGridFlag() - Sets whether to draw grid for X axis
   */
  inline void SetXGridFlag(bool value) {
    draw_x_grid_flag = value;

    return;
  }

  /*
   * SetYGridFlag() - Sets whether to draw grid for Y axis
   */
  inline void SetYGridFlag(bool value) {
    draw_y_grid_flag = value;

    return;
  }

  inline void SetMarkerSize(int size) {
    marker_size = size;

    return;
  }
};

#endif
