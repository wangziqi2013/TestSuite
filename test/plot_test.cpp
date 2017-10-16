
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

	Color c1 {"#123456"};
	Buffer buffer;

	c1.AppendToBuffer(&buffer);
	dbg_printf("Color 1: ");

	buffer.Append('\n');
	buffer.Append('\0');
	buffer.WriteToFile(stderr);
	buffer.Reset();

	Color c2 {0x12, 0x34, 0x56};
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
	_PrintTestName();

	LineChart lc {};

	lc.AppendXValueList<double>({1,5,10,15,20,25,30});
	lc.AppendYValueList<double>({1,2,3,4,5,6,7});
	lc.AppendYValueList<double>({1,2,4,8,16,32,64});
	lc.AppendYValueList<double>({1, 1.5, 2.4, 3.9, 5.0, 7.8, 9.5});

	lc.AppendLineName("Line 1");
	lc.AppendLineName("Line 2");
	lc.AppendLineName("Line 3");

	lc.SetYAxisLabel("Y Label");
	lc.SetXAxisLabel("X Label");

	lc.SetLegendFlag(true);
	lc.SetLegendVerticalFlag(false);

	lc.Draw("LineChart.pdf");

	//lc.buffer.WriteToFile(stderr);

	return;
}

/*
 * PlotBwTreePreallocation() - Plots data used in BwTree paper
 */
void PlotBwTreePreallocation() {
  BarChart bc1{};
  
  bc1.AppendBarGroup<double>("Insert\\nOnly", {3.17, 3.82});
  bc1.AppendBarGroup<double>("Read\\nOnly", {1.39, 1.44});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {1.08, 1.2});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {0.61, 0.7});
  
  bc1.AppendBarName("Independently Alloc'ed");
	bc1.AppendBarName("Pre-Alloc'ed");
	
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");

	bc1.SetLegendFlag(true);
	bc1.SetLegendPosition("upper right");
	bc1.SetYUpperLimitMinimum(5.0);
	bc1.SetYGridFlag(true);
	bc1.SetDrawHatchFlag(true);
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("preallocation-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Preallocation
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("Insert\\nOnly", {0.67, 0.84});
  bc2.AppendBarGroup<double>("Read\\nOnly", {0.94, 1.13});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {0.87, 1.04});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {0.38, 0.42});
  
  bc2.AppendBarName("Independently Alloc'ed");
	bc2.AppendBarName("Pre-Alloc'ed");

	bc2.SetLegendFlag(true);
	bc2.SetLegendPosition("upper right");
	bc2.SetYAxisLabel("Throughput (MOps/Sec)");
	bc2.SetYUpperLimitMinimum(3.0);
	bc2.SetYGridFlag(true);
	bc2.SetDrawHatchFlag(true);
	bc2.SetColorScheme(MIXED_COLOR_SCHEME);

	bc2.Draw("preallocation-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email	Preallocation
	BarChart bc3{};
	
	bc3.AppendBarGroup<double>("Insert\\nOnly", {0.46, 0.55});
  bc3.AppendBarGroup<double>("Read\\nOnly", {1.15, 1.37});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {1.43, 1.65});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {0.22, 0.25});
  
  bc3.AppendBarName("Independently Alloc'ed");
	bc3.AppendBarName("Pre-Alloc'ed");

	bc3.SetLegendFlag(true);
	bc3.SetLegendPosition("upper right");
	bc3.SetYAxisLabel("Throughput (MOps/Sec)");
	bc3.SetYUpperLimitMinimum(3.0);
	bc3.SetYGridFlag(true);
	bc3.SetDrawHatchFlag(true);
	bc3.SetColorScheme(MIXED_COLOR_SCHEME);

	bc3.Draw("preallocation-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Preallocation
	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("Mono\\nInt", {1.98, 2.16});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.28, 1.53});
  bc4.AppendBarGroup<double>("Email\\nKey", {2.80, 3.05});
  
  bc4.AppendBarName("Independently Alloc'ed");
	bc4.AppendBarName("Pre-Alloc'ed");

  bc4.SetYAxisLabel("Memory (GB)");
	bc4.SetLegendFlag(false);
	bc4.SetYUpperLimitMinimum(4.0);
	bc4.SetYGridFlag(true);
	bc4.SetColorScheme(MIXED_COLOR_SCHEME);

	bc4.Draw("preallocation-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeMicroIndex() - Plots data used in BwTree paper
 */
void PlotBwTreeMicroIndex() {
  BarChart bc1{};
  
  bc1.AppendBarGroup<double>("Insert\\nOnly", {2.98, 3.82});
  bc1.AppendBarGroup<double>("Read\\nOnly", {1.44, 1.44});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {0.99, 1.20});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {0.54, 0.70});
  
  bc1.AppendBarName("FC \\& SS");
	bc1.AppendBarName("No FC \\& SS");
	
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");

	bc1.SetLegendFlag(true);
	bc1.SetLegendPosition("upper right");
	bc1.SetYUpperLimitMinimum(5.0);
	bc1.SetYGridFlag(true);
	bc1.SetDrawHatchFlag(true);
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("microindex-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Microindex
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("Insert\\nOnly", {0.68, 0.84});
  bc2.AppendBarGroup<double>("Read\\nOnly", {1.01, 1.13});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {0.88, 1.04});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {0.34, 0.42});
  
  bc2.AppendBarName("FC \\& SS");
	bc2.AppendBarName("No FC \\& SS");

	bc2.SetLegendFlag(true);
	bc2.SetLegendPosition("upper right");
	bc2.SetYAxisLabel("Throughput (MOps/Sec)");
	bc2.SetYUpperLimitMinimum(3.0);
	bc2.SetYGridFlag(true);
	bc2.SetDrawHatchFlag(true);
	bc2.SetColorScheme(MIXED_COLOR_SCHEME);

	bc2.Draw("microindex-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email Microindex
	BarChart bc3{};
	
	bc3.AppendBarGroup<double>("Insert\\nOnly", {0.47, 0.55});
  bc3.AppendBarGroup<double>("Read\\nOnly", {1.29, 1.37});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {1.49, 1.65});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {0.20, 0.25});
  
  bc3.AppendBarName("FC \\& SS");
	bc3.AppendBarName("No FC \\& SS");

	bc3.SetLegendFlag(true);
	bc3.SetLegendPosition("upper right");
	bc3.SetYAxisLabel("Throughput (MOps/Sec)");
	bc3.SetYUpperLimitMinimum(3.0);
	bc3.SetYGridFlag(true);
	bc3.SetDrawHatchFlag(true);
	bc3.SetColorScheme(MIXED_COLOR_SCHEME);

	bc3.Draw("microindex-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory Microindex
	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("Mono\\nInt", {2.160, 2.160});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.520, 1.530});
  bc4.AppendBarGroup<double>("Email\\nKey", {3.01, 3.050});
  
  bc4.AppendBarName("FC \\& SS");
	bc4.AppendBarName("No FC \\& SS");

  bc4.SetYAxisLabel("Memory (GB)");

	bc4.SetLegendFlag(false);
	bc4.SetYGridFlag(true);
	bc4.SetYUpperLimitMinimum(4.0);
	bc4.SetColorScheme(MIXED_COLOR_SCHEME);

	bc4.Draw("microindex-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeGC() - Plot Bwtree GC result
 */
void PlotBwTreeGC() {
  LineChart lc1{};
  
  lc1.AppendXValueList<double>({1,5,10,15,20});
  
	lc1.AppendYValueList<double>({1.41, 5.77, 9.87, 6.80, 6.53});
	lc1.AppendYValueList<double>({1.44, 5.39, 11.84, 13.00, 14.53});

	lc1.AppendLineName("Centralized GC");
	lc1.AppendLineName("Distributed GC");

	lc1.SetYAxisLabel("Throughput (MOps/Sec)");

	lc1.SetLegendFlag(true);
	lc1.SetLegendPosition("upper right");
	lc1.SetLegendVerticalFlag(false);
	lc1.SetYGridFlag(true);
	lc1.SetMarkerSize(12);
	lc1.SetColorScheme(MIXED_COLOR_SCHEME);
	lc1.SetXAxisLabel("Number of Worker Threads");

	lc1.Draw("gc-mono.pdf");
	
	///////////////////////////////////////////////////////////////////
	
	// GC random
	LineChart lc2{};
  
  lc2.AppendXValueList<double>({1,5,10,15,20});
  
	lc2.AppendYValueList<double>({1.13, 5.25, 9.09, 6.60, 6.83});
	lc2.AppendYValueList<double>({1.10, 5.28, 9.90, 11.38, 12.87});

	lc2.AppendLineName("Centralized GC");
	lc2.AppendLineName("Distributed GC");

	lc2.SetLegendFlag(true);
	lc2.SetMarkerSize(12);
	lc2.SetLegendPosition("upper right");
	lc2.SetYAxisLabel("Throughput (MOps/Sec)");
	lc2.SetLegendVerticalFlag(false);
	lc2.SetYGridFlag(true);
	lc2.SetColorScheme(MIXED_COLOR_SCHEME);
	lc2.SetXAxisLabel("Number of Worker Threads");

	lc2.Draw("gc-rand.pdf");
	
	///////////////////////////////////////////////////////////////////
	
	// GC email
	LineChart lc3{};
  
  lc3.AppendXValueList<double>({1,5,10,15,20});
  
  lc3.AppendYValueList<double>({1.26, 6.78, 9.28, 9.66, 9.19});
  lc3.AppendYValueList<double>({1.37, 6.25, 9.35, 11.1, 12.94});

	lc3.AppendLineName("Centralized GC");
	lc3.AppendLineName("Distributed GC");

	lc3.SetLegendFlag(true);
	lc3.SetMarkerSize(12);
	lc3.SetLegendPosition("upper right");
	lc3.SetYAxisLabel("Throughput (MOps/Sec)");
	lc3.SetLegendVerticalFlag(false);
	lc3.SetYGridFlag(true);
	lc3.SetColorScheme(MIXED_COLOR_SCHEME);
	lc3.SetXAxisLabel("Number of Worker Threads");

	lc3.Draw("gc-email.pdf");
	
	///////////////////////////////////////////////////////////////////
	// GC memory

	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("Mono\\nInt", {2.320, 2.160});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.590, 1.530});
  bc4.AppendBarGroup<double>("Email\\nKey", {3.104, 3.050});
  
  bc4.AppendBarName("Centralized GC");
	bc4.AppendBarName("Distributed GC");

  bc4.SetYAxisLabel("Memory (GB)");
	bc4.SetLegendFlag(false);
	bc4.SetYGridFlag(true);
	bc4.SetYUpperLimitMinimum(4.0);
	bc4.SetColorScheme(MIXED_COLOR_SCHEME);

	bc4.Draw("gc-memory.pdf");
  
  return;
}

/*
 * PlotBwTreeCompare() - Plots data used in BwTree paper
 */
void PlotBwTreeCompare() {
  BarChart bc1{};
  
	bc1.AppendBarGroup<double>("Insert\\nOnly", {2.43, 3.82, 4.13, 8.4, 14.17, 10.95});
  bc1.AppendBarGroup<double>("Read\\nOnly", {1.41, 1.44, 1.69, 2.17, 1.98, 5.69});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {0.89, 1.2, 1.51, 1.82, 1.82, 3.88});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {0.48, 0.7, 0.54, 0.78, 1.98, 0.48});
  
	bc1.AppendBarName("BwTree");
	bc1.AppendBarName("OpenBwTree");
	bc1.AppendBarName("Skiplist");
	bc1.AppendBarName("MassTree");
	bc1.AppendBarName("B+Tree");
  bc1.AppendBarName("ART");
  
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");
	bc1.SetYUpperLimitMinimum(18.0);

	bc1.SetLegendFlag(false);
	bc1.SetYGridFlag(true);
	bc1.SetDrawHatchFlag(true);
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("compare-mono.pdf");

	// We want a horizontal legend
	bc1.SetLegendVerticalFlag(false);
	bc1.DrawLegend("compare-legend.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Compare
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("Insert\\nOnly", {0.62, 0.84, 1.21, 1.75, 1.57, 3.32});
  bc2.AppendBarGroup<double>("Read\\nOnly", {1.00, 1.13, 1.44, 1.95, 2.27, 3.75});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {0.70, 1.04, 1.12, 1.67, 2.13, 3.08});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {0.31, 0.42, 0.33, 0.28, 1.59, 0.29});

	bc2.AppendBarName("BwTree");
	bc2.AppendBarName("OpenBwTree");
	bc2.AppendBarName("Skiplist");
	bc2.AppendBarName("MassTree");
	bc2.AppendBarName("B+Tree");
  bc2.AppendBarName("ART");

	bc2.SetLegendFlag(false);
	bc2.SetYAxisLabel("Throughput (MOps/Sec)");
	bc2.SetYUpperLimitMinimum(6.0);
	bc2.SetYGridFlag(true);
	bc2.SetDrawHatchFlag(true);
	bc2.SetColorScheme(MIXED_COLOR_SCHEME);

	bc2.Draw("compare-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email compare
	BarChart bc3{};

	bc3.AppendBarGroup<double>("Insert\\nOnly", {0.46, 0.55, 0.78, 1.17, 0.89, 2.15});
  bc3.AppendBarGroup<double>("Read\\nOnly", {1.26, 1.37, 2.26, 3.34, 2.07, 5.96});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {1.29, 1.65, 1.94, 2.78, 1.89, 4.21});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {0.18, 0.25, 0.11, 0.24, 0.72, 0.21});

  bc3.AppendBarName("BwTree");
	bc3.AppendBarName("OpenBwTree");
	bc3.AppendBarName("Skiplist");
	bc3.AppendBarName("MassTree");
	bc3.AppendBarName("BTreeLOC");
	bc3.SetDrawHatchFlag(true);
  bc3.AppendBarName("ART");
	
	bc3.SetLegendFlag(false);
	bc3.SetYAxisLabel("Throughput (MOps/Sec)");
	bc3.SetYUpperLimitMinimum(8.0);
	bc3.SetYGridFlag(true);
	bc3.SetColorScheme(MIXED_COLOR_SCHEME);

	bc3.Draw("compare-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Compare
	BarChart bc4{};

	bc4.AppendBarGroup<double>("Mono Int", {1.97, 2.16, 1.87, 2.35, 1.59, 0.41});
  bc4.AppendBarGroup<double>("Rand Int", {1.24, 1.53, 2.09, 2.84, 1.13, 1.44});
  bc4.AppendBarGroup<double>("Email Key", {2.77, 3.05, 2.82, 6.92, 2.79, 1.67});

  bc4.AppendBarName("BwTree");
	bc4.AppendBarName("OpenBwTree");
	bc4.AppendBarName("Skiplist");
	bc4.AppendBarName("MassTree");
	bc4.AppendBarName("B+Tree");
  bc4.AppendBarName("ART");

  bc4.SetYAxisLabel("Memory (GB)");

	bc4.SetLegendFlag(true);
	bc4.SetLegendVerticalFlag(true);
	bc4.SetLegendPosition("upper left");
	bc4.SetYUpperLimitMinimum(9.0);
	bc4.SetLegendColumnNum(2);
	bc4.SetYGridFlag(true);
	bc4.SetDrawHatchFlag(true);
	bc4.SetColorScheme(MIXED_COLOR_SCHEME);
	bc4.SetWidth(18.0);

	bc4.Draw("compare-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeMTCompare() - Plots data used in BwTree paper
 */
void PlotBwTreeMTCompare() {
  BarChart bc1{};

	bc1.AppendBarGroup<double>("Insert\\nOnly", {11.25, 23.31, 19.18, 26.28, 55.31, 101.47});
	bc1.AppendError<double>({{1.47, 1.72}, {2.10, 1.78}, {1.58, 2.03}, {3.17, 2.96}, {6.84, 9.25}, {20.19, 11.34}});
  bc1.AppendBarGroup<double>("Read\\nOnly",   {12.58, 21.48, 20.05, 27.66, 26.77, 82.21});
	bc1.AppendError<double>({{0.98, 1.59}, {1.13, 1.74}, {1.62, 2.21}, {2.10, 2.96}, {5.52, 7.10}, {10.50, 12.26}});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {12.37, 16.72, 13.24, 21.99, 22.95, 53.79});
	bc1.AppendError<double>({{1.33, 1.15}, {1.96, 1.54}, {2.07, 1.38}, {2.12, 3.15}, {4.21, 2.26}, {6.87, 9.52}});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {5.80, 8.4, 5.25, 7.51, 19.14, 5.42});
	bc1.AppendError<double>({{0.79, 0.52}, {0.77, 0.60}, {0.54, 0.31}, {0.51, 0.22}, {0.43, 0.84}, {0.48, 0.53}});
  
	bc1.AppendBarName("BwTree");
	bc1.AppendBarName("OpenBwTree"); 
	bc1.AppendBarName("Skiplist");
	bc1.AppendBarName("MassTree");
	bc1.AppendBarName("B+Tree");
  bc1.AppendBarName("ART");
	
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");
	bc1.SetYUpperLimitMinimum(140.0);

	bc1.SetLegendFlag(false);
	bc1.SetYGridFlag(true);
	bc1.SetDrawErrorFlag(true);
	bc1.SetDrawHatchFlag(true);
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("mt-compare-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Compare
	BarChart bc2{};

	bc2.AppendBarGroup<double>("Insert\\nOnly", {7.36, 9.94, 9.07, 17.97, 8.09, 44.95});
	bc2.AppendError<double>({{0.89, 0.74}, {0.54, 1.23}, {1.09, 0.68}, {1.37, 0.86}, {0.65, 1.22}, {6.27, 5.68}});
  bc2.AppendBarGroup<double>("Read\\nOnly",   {12.09, 15.51, 13.26, 30.54, 29.03, 51.52});
	bc2.AppendError<double>({{0.77, 1.12}, {0.63, 0.92}, {1.23, 0.95}, {0.68, 1.04}, {1.32, 0.89}, {4.45, 5.93}});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {9.68, 13.3, 11.67, 22.07, 25.15, 42.97});
	bc2.AppendError<double>({{0.63, 0.70}, {0.52, 0.54}, {1.29, 0.75}, {1.82, 2.90}, {1.20, 1.62}, {3.54, 2.88}});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {4.07, 5.43, 4.73, 3.68, 18.96, 3.43});
	bc2.AppendError<double>({{0.22, 0.10}, {0.10, 0.00}, {0.20, 0.10}, {0.08, 0.10}, {0.85, 1.24}, {0.21, 0.11}});

  bc2.AppendBarName("BwTree");
	bc2.AppendBarName("OpenBwTree");
	bc2.AppendBarName("Skiplist");
	bc2.AppendBarName("MassTree");
	bc2.AppendBarName("B+Tree");
  bc2.AppendBarName("ART");

	bc2.SetLegendFlag(false);
	bc2.SetYAxisLabel("Throughput (MOps/Sec)");
	bc2.SetYUpperLimitMinimum(70.0);
	bc2.SetYGridFlag(true);
	bc2.SetDrawHatchFlag(true);
	bc2.SetDrawErrorFlag(true);
	bc2.SetColorScheme(MIXED_COLOR_SCHEME);

	bc2.Draw("mt-compare-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email compare
	BarChart bc3{};

	bc3.AppendBarGroup<double>("Insert\\nOnly", {5.37, 6.4, 8.25, 12.48, 6.74, 24.01});
	bc3.AppendError<double>({{0.21, 0.33}, {0.19, 0.54}, {0.67, 0.70}, {1.02, 0.90}, {0.65, 0.87}, {3.51, 1.91}});
  bc3.AppendBarGroup<double>("Read\\nOnly",   {10.24, 15.59, 15.05, 37.63, 23.73, 69.66});
	bc3.AppendError<double>({{0.48, 0.54}, {0.60, 0.81}, {1.11, 1.72}, {2.03, 1.49}, {2.50, 2.71}, {4.46, 3.81}});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {9.17, 16.69, 13.26, 26.52, 22.06, 46.68});
	bc3.AppendError<double>({{0.55, 0.49}, {0.83, 1.09}, {0.73, 1.11}, {2.54, 1.33}, {2.27, 2.05}, {3.29, 4.61}});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {2.22, 2.95, 1.34, 2.90, 7.45, 2.37});
	bc3.AppendError<double>({{0.12, 0.08}, {0.11, 0.05}, {0.07, 0.11}, {0.08, 0.10}, {0.76, 0.82}, {0.16, 0.09}});

  bc3.AppendBarName("BwTree");
	bc3.AppendBarName("OpenBwTree");
	bc3.AppendBarName("Skiplist");
	bc3.AppendBarName("MassTree");
	bc3.AppendBarName("B+Tree");
  bc3.AppendBarName("ART");
	
	bc3.SetLegendFlag(false);
	bc3.SetYAxisLabel("Throughput (MOps/Sec)");
	bc3.SetYUpperLimitMinimum(90.0);
	bc3.SetYGridFlag(true);
	bc3.SetDrawHatchFlag(true);
	bc3.SetDrawErrorFlag(true);
	bc3.SetColorScheme(MIXED_COLOR_SCHEME);

	bc3.Draw("mt-compare-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Compare
	BarChart bc4{};

	bc4.AppendBarGroup<double>("Mono Int", {2.05, 2.34, 2.07, 3.37, 1.59, 0.42});
  bc4.AppendBarGroup<double>("Rand Int", {1.36, 1.79, 2.18, 2.86, 1.15, 1.44});
  bc4.AppendBarGroup<double>("Email Key", {2.86, 3.22, 3.10, 6.95, 2.79, 1.66});

  bc4.AppendBarName("BwTree");
	bc4.AppendBarName("OpenBwTree");
	bc4.AppendBarName("Skiplist");
	bc4.AppendBarName("MassTree");
	bc4.AppendBarName("B+Tree");
  bc4.AppendBarName("ART");

  bc4.SetYAxisLabel("Memory (GB)");

	bc4.SetLegendFlag(true);
	bc4.SetLegendPosition("upper left");
	bc4.SetLegendVerticalFlag(true);
	bc4.SetYUpperLimitMinimum(9.0);
	bc4.SetYGridFlag(true);
	bc4.SetLegendColumnNum(2);
	bc4.SetDrawHatchFlag(true);
	bc4.SetColorScheme(MIXED_COLOR_SCHEME);
	bc4.SetWidth(18.0);

	bc4.Draw("mt-compare-memory.pdf");
	
	return;
}

void PlotBwTreeSummary() {
  BarChart bc1{};
  
  bc1.AppendBarGroup<double>("Single Thread", {0.7, 0.67, 0.88, 1.04, 0.96});
  bc1.AppendBarGroup<double>("20 Worker Threads", {9.68, 9.71, 11.82, 13.3, 12.8});
  bc1.AppendBarName("Bw-Tree");
  bc1.AppendBarName("+GC");
  bc1.AppendBarName("+PA");
  bc1.AppendBarName("+FC \\& SS");
  bc1.AppendBarName("+NK");
	
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");
	bc1.SetYUpperLimitMinimum(16.0);

	bc1.SetLegendFlag(true);
	bc1.SetYGridFlag(true);
	bc1.SetDrawHatchFlag(true);
	bc1.SetLegendPosition("upper left");
	bc1.SetColorScheme(BROWN_COLOR_SCHEME);

	bc1.Draw("bwtree-summary.pdf");
	
  return; 
}

void PlotBwTreeSummary2() {
  BarChart bc1{};

  bc1.AppendBarGroup<double>("Insert\\nOnly", {7.36, 9.94});
  bc1.AppendBarGroup<double>("Read\\nOnly", {12.09, 15.51});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {9.68, 13.3});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {4.07, 5.43});
  
  bc1.AppendBarName("Bw-Tree");
  bc1.AppendBarName("OpenBw-Tree");
	
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");
	bc1.SetYUpperLimitMinimum(30.0);

	bc1.SetLegendFlag(true);
	bc1.SetYGridFlag(true);
	bc1.SetDrawHatchFlag(true);
	bc1.SetLegendPosition("upper left");
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("bwtree-summary-2.pdf");
	
  return; 
}

/*
 * PlotBwTreeConfig() - Plots the configuration change diagram
 */
void PlotBwTreeConfig() {
  LineChart lc1{};
  
  lc1.AppendXValueList<double>({8, 16, 24, 32});
  
	lc1.AppendYValueList<double>({16.14, 18.55, 21.59, 20.92});
	lc1.AppendYValueList<double>({19.11, 21.54, 22.98, 22.53});
	lc1.AppendYValueList<double>({20.64, 22.91, 25.13, 23.81});

	lc1.AppendLineName("Size 32");
	lc1.AppendLineName("Size 64");
	lc1.AppendLineName("Size 128");

	lc1.SetYAxisLabel("Throughput (MOps/Sec)");
	lc1.SetXAxisLabel("Maximum Delta Chain Length");

	lc1.SetLegendFlag(true);
	lc1.SetLegendVerticalFlag(false);
	//lc1.SetYUpperLimitMinimum(28.0);

	lc1.Draw("config-ru.pdf");
	
  //////////////////////////////////////////////////
	
	LineChart lc2{};
  
  lc2.AppendXValueList<double>({8, 16, 24, 32});
  
	lc2.AppendYValueList<double>({7.39, 8.56, 8.73, 7.35});
	lc2.AppendYValueList<double>({8.75, 9.26, 9.83, 9.22});
	lc2.AppendYValueList<double>({10.63, 11.01, 10.29, 9.62});

	lc2.AppendLineName("Size 32");
	lc2.AppendLineName("Size 64");
	lc2.AppendLineName("Size 128");

	lc2.SetYAxisLabel("Throughput (MOps/Sec)");
	lc2.SetXAxisLabel("Maximum Delta Chain Length");

	lc2.SetLegendFlag(true);
	lc2.SetLegendVerticalFlag(false);
	//lc2.SetYUpperLimitMinimum(28.0);

	lc2.Draw("config-io.pdf");
	
	return;
}

/*
 * PlotHighContentionMT() - This function plots high contention monotomic
 *                          integer key insertion troughput
 */
void PlotHighContentionMT() {
	BarChart bc1{};

	bc1.AppendBarGroup<double>("20 Threads\\nNUMA 0", {0.87, 1.15, 2.23, 4.03, 2.69, 3.98});
	bc1.AppendBarGroup<double>("20 Threads\\nNUMA 0 + 1", {0.31, 0.53, 1.68, 1.88, 1.1, 1.28});
	bc1.AppendBarGroup<double>("40 Threads\\nNUMA 0 + 1", {0.31, 0.55, 1.70, 2.1, 0.92, 1.51});
  
	bc1.AppendBarName("Bw-Tree");
	bc1.AppendBarName("OpenBw-Tree");
	bc1.AppendBarName("Skiplist");
	bc1.AppendBarName("Masstree");
	bc1.AppendBarName("B+Tree");
  bc1.AppendBarName("ART");
	
	bc1.SetYAxisLabel("Throughput (MOps/Sec)");
	bc1.SetYUpperLimitMinimum(5.0);

	// Do not use legend
	bc1.SetLegendFlag(false);
	bc1.SetDrawHatchFlag(true);
	bc1.SetYGridFlag(true);
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("mt-ht-mono-insert.pdf");

	///////////////////////////////////////////////////////////////////

	BarChart bc2{};

	bc2.AppendBarGroup<double>("20 Threads\\nMono Int", {11.25, 23.31, 19.18, 26.28, 55.31, 101.47});
	bc2.AppendBarGroup<double>("20 Threads\\nHigh Contention", {0.87, 1.15, 2.23, 4.03, 2.69, 3.98});

	bc2.AppendBarName("Bw-Tree");
	bc2.AppendBarName("OpenBw-Tree");
	bc2.AppendBarName("Skiplist");
	bc2.AppendBarName("Masstree");
	bc2.AppendBarName("B+Tree");
  bc2.AppendBarName("ART");
	
	bc2.SetYAxisLabel("Throughput (MOps/Sec)");
	bc2.SetYUpperLimitMinimum(130.0);

	// Do not use legend
	bc2.SetLegendFlag(false);
	bc2.SetWidth(16);
	bc2.SetLegendFlag(true);
	bc2.SetLegendPosition("upper right");
	bc2.SetDrawHatchFlag(true);
	bc2.SetYGridFlag(true);
	bc2.SetColorScheme(MIXED_COLOR_SCHEME);

	bc2.Draw("mt-ht-mono-insert-2.pdf");

	return;
}

/*
 * PlotHighContentionMTMem() - This function plots high contention monotomic
 *                             integer key insertion DRAM access rate
 */
void PlotHighContentionMTMem() {
	BarChart bc1{};

	bc1.AppendBarGroup<double>("20 Threads\\nNUMA 0", {58.2552, 72.496, 10.91, 7.254, 2.0444, 2.786});
	bc1.AppendBarGroup<double>("20 Threads\\nNUMA 0 + 1", {24.5582, 36.0824, 16.77, 11.2424, 5.588, 6.4512});
	bc1.AppendBarGroup<double>("40 Threads\\nNUMA 0 + 1", {29.7786, 45.364, 21.25, 14.742, 5.9248, 8.8486});
  
	bc1.AppendBarName("Bw-Tree");
	bc1.AppendBarName("OpenBw-Tree");
	bc1.AppendBarName("Skiplist");
	bc1.AppendBarName("Masstree");
	bc1.AppendBarName("B+Tree");
  bc1.AppendBarName("ART");
	
	bc1.SetYAxisLabel("DRAM Local Access Rate\\n(MAccess/Sec)");
	bc1.SetYUpperLimitMinimum(90.0);

	bc1.SetLegendFlag(false);
	bc1.SetDrawHatchFlag(true);
	bc1.SetYGridFlag(true);
	//bc1.SetWidth(18);
	bc1.SetColorScheme(MIXED_COLOR_SCHEME);

	bc1.Draw("mt-ht-mono-insert-numa-local.pdf");

	//////////////////////////////////////////////////////////////

	BarChart bc2{};

	bc2.AppendBarGroup<double>("20 Threads\\nNUMA 0", {0.3654, 0.437, 0.04, 0.042718, 0.032818, 0.0398});
	bc2.AppendBarGroup<double>("20 Threads\\nNUMA 0 + 1", {39.618, 62.0948, 21.79, 14.5888, 9.834, 9.6});
	bc2.AppendBarGroup<double>("40 Threads\\nNUMA 0 + 1", {39.6862, 64.306, 29.34, 19.068, 11.04, 13.439});
  
	bc2.AppendBarName("Bw-Tree");
	bc2.AppendBarName("OpenBw-Tree");
	bc2.AppendBarName("Skiplist");
	bc2.AppendBarName("Masstree");
	bc2.AppendBarName("B+Tree");
  bc2.AppendBarName("ART");
	
	bc2.SetYAxisLabel("DRAM Remote Access Rate\\n(MAccess/Sec)");
	bc2.SetYUpperLimitMinimum(90.0);

	bc2.SetLegendFlag(false);
	bc2.SetDrawHatchFlag(true);
	bc2.SetYGridFlag(true);
	//bc2.SetWidth(18);
	bc2.SetColorScheme(MIXED_COLOR_SCHEME);

	bc2.Draw("mt-ht-mono-insert-numa-remote.pdf");

	return;
}

int main() {
	//ColorTest();
	//PrintListTest();
	//RoundUpTest();
	//BarPlotTest();
	//LinePlotTest();
	
	//PlotBwTreePreallocation();
  //PlotBwTreeMicroIndex();
  //PlotBwTreeGC();
  PlotBwTreeCompare();
  //PlotBwTreeMTCompare();
  //PlotBwTreeSummary();
  //PlotBwTreeSummary2();
  
	// This is plotted using Python script
  //PlotBwTreeConfig();

	//PlotHighContentionMT();
	//PlotHighContentionMTMem();
  
	return 0;
}
