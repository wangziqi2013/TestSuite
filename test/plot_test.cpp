
/*
 * plot_test.cpp - Tests plot wrapper
 */
 
#include "plot_suite.h" 
#include "test_suite.h"
 
/*
 * ColorTest() - Tests whether color works
 */
void ColorTest() {
  _PrintTestName();
  
  Color c1{"#123456"};
  Buffer buffer;
  
  c1.AppendToBuffer(&buffer);
  dbg_printf("Color 1: ");
  
  buffer.Append('\n');
  buffer.Append('\0');
  buffer.WriteToFile(stderr);
  buffer.Reset();
  
  Color c2{0x12, 0x34, 0x56};
  c2.AppendToBuffer(&buffer);
  dbg_printf("Color 2: ");
  
  buffer.Append('\n');
  buffer.Append('\0');
  buffer.WriteToFile(stderr);
  buffer.Reset();
  
  return;
}

/*
 * RoundUpTest() - Tests whether rounding works
 */
void RoundUpTest() {
  _PrintTestName(); 
  
  double num = 5.92L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 7.00L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 9.15L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 15.55L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 27.41L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  return;
}

/*
 * PlotTest() - Check whether we could plot using the class
 */
void PlotTest() {
  BarChart bc;
  
  bc.AppendBarGroup("Group 1");
  bc.AppendData<int>(1);
  bc.AppendData<int>(2);
  bc.AppendData<int>(3);
  bc.AppendBarGroup("Group 2");
  bc.AppendData<int>(2);
  bc.AppendData<int>(3);
  bc.AppendData<int>(1);
  bc.AppendBarGroup("Group 3");
  bc.AppendData<int>(3);
  bc.AppendData<int>(1);
  bc.AppendData<int>(2);
  bc.AppendBarGroup("Group 4");
  bc.AppendData<int>(3);
  bc.AppendData<int>(2);
  bc.AppendData<int>(1);
  
  bc.AppendBarName("First");
  bc.AppendBarName("Second");
  bc.AppendBarName("Third");
  
  bc.SetYAxisLabel("Y Label");
  bc.SetLegendFlag(true);
  bc.SetLegendVerticalFlag(true);
  
  //bc.Draw("File name.pdf");
  dbg_printf("Finished plotting bar chart\n");
  bc.DrawLegend("File name legend.pdf");
  
  //bc.buffer.WriteToFile(stderr);
  
  return;
}

int main() {
  ColorTest();
  RoundUpTest();
  PlotTest();
  
  return 0; 
} 
