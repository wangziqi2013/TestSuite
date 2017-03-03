
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
 * PrintListTest() - Tests printing a list of doubles into a buffer
 */
void PrintListTest() {
  _PrintTestName();
  
  std::vector<double> t;
  t.push_back(1.1);
  t.push_back(2.2);
  t.push_back(3.3);
  t.push_back(4.4);
  
  Buffer buffer;
  PrintListDouble(t, &buffer);
  buffer.Append('\n');
  
  buffer.WriteToFile(stderr);
  
  return;
}

/*
 * RoundUpTest() - Tests whether rounding works
 */
void RoundUpTest() {
  _PrintTestName(); 
  
  double num = 5.92L;
  dbg_printf("Prev = %f; After = %f\n", num, RoundUpToPoint5(num));
  
  num = 7.00L;
  dbg_printf("Prev = %f; After = %f\n", num, RoundUpToPoint5(num));
  
  num = 9.15L;
  dbg_printf("Prev = %f; After = %f\n", num, RoundUpToPoint5(num));
  
  num = 15.55L;
  dbg_printf("Prev = %f; After = %f\n", num, RoundUpToPoint5(num));
  
  num = 27.41L;
  dbg_printf("Prev = %f; After = %f\n", num, RoundUpToPoint5(num));
  
  return;
}

/*
 * BarPlotTest() - Check whether we could plot bar chart using the class
 */
void BarPlotTest() {
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
  
  bc.Draw("BarChart.pdf");
  dbg_printf("Finished plotting bar chart\n");
  bc.DrawLegend("BarChartLegend.pdf");
  
  //bc.buffer.WriteToFile(stderr);
  
  return;
}

/*
 * LinePlotTest() - Tests whether we could plot line charts
 */
void LinePlotTest() {
  LineChart lc{};
  
  lc.AppendXValueList<double>({1,5,10,15,20,25,30});
  lc.AppendYValueList<double>({1,2,3,4,5,6,7});
  lc.AppendYValueList<double>({1,2,4,8,16,32,64});
  lc.AppendYValueList<double>({1, 1.5, 2.4, 3.9, 5.0, 7.8, 9.5});
  
  lc.AppendLineName("Line 1");
  lc.AppendLineName("Line 2");
  lc.AppendLineName("Line 3");
  
  lc.SetYAxisLabel("Y Label");
  lc.SetXAxisLabel("X Label");
  
  lc.Draw("LineChart.pdf");
  
  //lc.buffer.WriteToFile(stderr);
  
  return;
}

int main() {
  ColorTest();
  PrintListTest();
  RoundUpTest();
  BarPlotTest();
  LinePlotTest();
  
  return 0; 
} 
