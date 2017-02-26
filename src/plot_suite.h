
#pragma once

#ifndef _PLOT_SUITE_H
#define _PLOT_SUITE_H

#include "buffer.h"
#include "test_suite.h"

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
        hex_value[i] = static_cast<uint8_t>(ch) - static_cast<uint8_t>('a');  
      } else if(ch >= 'A' && ch <= 'F') {
        hex_value[i] = static_cast<uint8_t>(ch) - static_cast<uint8_t>('A');  
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
  void AppendToBuffer(Buffer *buffer_p) {
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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class BarGroup - This class represents a group in the graph
 */
class BarGroup {
 private:
  // The title of the group. This will be drawn on the 
  // x axis. Its font size of x_tick_font_size
  std::string title; 
   
  // The list of data the bar chart will be showing
  // The size of the data list need not be the same 
  // across groups, because the drawing routine could recognize it
  // correctly
  std::vector<double> data_list; 
 public:
  
  /*
   * Constructor
   */
  BarGroup(const std::string p_title) :
    title{p_title},
    data_list{} 
  {}
  
  /*
   * Constructor
   */
  BarGroup(const char *p_title) :
    title{p_title},
    data_list{}
  {}
  
  /*
   * Append() - Appends data into the data list
   *
   * This function is templatized such that we could append an array
   * of any type, and they will be converted to double type when we append them
   */
  template <typename T>
  void Append(size_t count, T *data_p) {
    // Convert every element to double and then push back to
    // the data vector
    for(size_t i = 0;i < count;i++) {
      data_list.push_back(static_cast<double>(data_p[i]));
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
   * Append() - This function appends a single element into the data list
   */
  template <typename T>
  inline void Append(T data) {
    Append(1, &data);
    
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
  uint32_t width;
  uint32_t height;
  
  // The font size of ticks (i.e. interval markers) on both axes
  uint32_t x_tick_font_size;
  uint32_t y_tick_font_size;
  
  // The font size of titles on both axes
  uint32_t x_font_size;
  uint32_t y_font_size;
  
  // Legend font size
  uint32_t legend_font_size;
  
  // This is the position of the legend; expressed in the format that
  // matplotlib could recognize (i.e. we do not use C++ constants for 
  // this field)
  std::string legend_position;
  
  /*
   * Constructor
   */
  ChartParameter(uint32_t p_width, 
                 uint32_t p_height,
                 uint32_t p_x_tick_font_size,
                 uint32_t p_y_tick_font_size,
                 uint32_t p_x_font_size,
                 uint32_t p_y_font_size,
                 uint32_t p_legend_font_size,
                 const std::string &p_legend_position) :
    width{p_width},
    height{p_width},
    x_tick_font_size{p_x_tick_font_size},
    y_tick_font_size{p_y_tick_font_size},
    x_font_size{p_x_font_size},
    y_font_size{p_y_font_size},
    legend_font_size{p_legend_font_size},
    legend_position{p_legend_position}
  {}
  
  /*
   * Copy Constructor
   */
  ChartParameter(const ChartParameter &other) :
    width{other.width},
    height{other.width},
    x_tick_font_size{other.x_tick_font_size},
    y_tick_font_size{other.y_tick_font_size},
    x_font_size{other.x_font_size},
    y_font_size{other.y_font_size},
    legend_font_size{other.legend_font_size},
    legend_position{other.legend_position}
  {}
};

// This is the default parameter for charts
// We could customize one by ourselves but this should be sufficient for
// most charts
extern ChartParameter default_chart_param;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class BarChart - Represents a bar chart, including its data and metadata
 *                  such as graph title,  
 */
class BarChart {
 private:
  // This stores data and group title.
  // Note that it is not required that all group have the same length
  // However, when drawing the diagram, we will calculate the maximum
  // number of elements among all groups, and use this value to determine
  // the width of each group 
  std::vector<BarGroup> group_list;
  
  // The title of the bar chart. If its length is 0 we do not draw anything 
  std::string chart_title;
  
  // This is a pointer to the color scheme, which could be predefined
  // or customized
  Color *color_scheme_p;
  
  // This corresponds with the color - the color on index i
  // has a legend title on this list at position i
  std::vector<std::string> legend_title_list;
  
  // The following are parameters that could be tweaked but usually kept
  // as-is
  ChartParameter param;
  
  // The following two are X and Y labels
  // By default they are not set, and also will not be printed
  std::string x_label;
  std::string y_label;
  
  // This is the buffer in which we print the pythob script into
  Buffer buffer;
  
  /*
   * Constructor
   */
  BarChart(const std::string &p_chart_title) :
    group_list{},
    chart_title{p_chart_title},
    color_scheme_p{RED_COLOR_SCHEME},
    legend_title_list{},
    // Use the default parameters inside the constructor
    // if we need to modify this then just modify it later
    param{default_chart_param},
    // By default should be empty
    x_label{},
    y_label{},
    buffer{}
  {}
  
  /*
   * Constructor - Anonymous graph which does not have a title
   */
  BarChart() :
    BarChart{""}
  {}
  
  /*
   * Destructor
   */
  ~BarChart() {}
  
  /*
   * GetMaximumGroupSize() - Returns the maximum group size
   *
   * Since we do not restrict the size of each bar group, they could be
   * of different size. In order to uniformly draw the graph we need to 
   * calculate the maximum size of the bar group to determine the width
   * of each bargroup
   */
  size_t GetMaximumGroupSize() {
    // Loop over each bar group and get its size 
    for(const BarGroup &bg : group_list) {
      
    }
  }
};

#endif
