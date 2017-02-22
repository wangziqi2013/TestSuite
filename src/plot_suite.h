
#pragma once

#ifndef _PLOT_SUITE_H
#define _PLOT_SUITE_H

#include "buffer.h"
#include "test_suite.h"

/*
 * class BarGroup - This class represents a group in the graph
 */
class BarGroup {
 private:
  // The title of the group. This will be drawn on the 
  // x axis
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
};

#endif
