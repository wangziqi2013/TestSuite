
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
  
  bc1.AppendBarGroup<double>("Insert\\nOnly", {3.40, 4.10});
  bc1.AppendBarGroup<double>("Read\\nOnly", {2.86, 2.96});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {2.86, 3.18});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {0.71, 0.81});
  
  bc1.AppendBarName("Independently Alloc'ed");
	bc1.AppendBarName("Pre-Alloc'ed");
	
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");

	bc1.SetLegendFlag(false);
	bc1.SetYUpperLimitMinimum(5.0);

	bc1.Draw("preallocation-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Preallocation
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("Insert\\nOnly", {0.68, 0.85});
  bc2.AppendBarGroup<double>("Read\\nOnly", {2.20, 2.64});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {2.67, 3.17});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {0.59, 0.64});
  
  bc2.AppendBarName("Independently Alloc'ed");
	bc2.AppendBarName("Pre-Alloc'ed");

	bc2.SetLegendFlag(false);
	bc2.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc2.SetYUpperLimitMinimum(5.0);

	bc2.Draw("preallocation-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email	Preallocation
	BarChart bc3{};
	
	bc3.AppendBarGroup<double>("Insert\\nOnly", {0.46, 0.56});
  bc3.AppendBarGroup<double>("Read\\nOnly", {1.22, 1.44});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {1.46, 1.68});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {0.44, 0.49});
  
  bc3.AppendBarName("Independently Alloc'ed");
	bc3.AppendBarName("Pre-Alloc'ed");

	bc3.SetLegendFlag(false);
	bc3.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc3.SetYUpperLimitMinimum(5.0);

	bc3.Draw("preallocation-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Preallocation
	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("Mono\\nInt", {1.055, 1.115});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.580, 1.615});
  bc4.AppendBarGroup<double>("Email\\nKey", {3.195, 3.270});
  
  bc4.AppendBarName("Independently Alloc'ed");
	bc4.AppendBarName("Pre-Alloc'ed");

  bc4.SetYAxisLabel("Memory (GB)");
	bc4.SetLegendFlag(false);

	bc4.Draw("preallocation-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeMicroIndex() - Plots data used in BwTree paper
 */
void PlotBwTreeMicroIndex() {
  BarChart bc1{};
  
  bc1.AppendBarGroup<double>("Insert\\nOnly", {3.21, 4.10});
  bc1.AppendBarGroup<double>("Read\\nOnly", {2.96, 2.96});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {2.63, 3.18});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {0.63, 0.81});
  
  bc1.AppendBarName("Independently Alloc'ed");
	bc1.AppendBarName("Pre-Alloc'ed");
	
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");

	bc1.SetLegendFlag(false);
	bc1.SetYUpperLimitMinimum(5.0);

	bc1.Draw("microindex-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Microindex
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("Insert\\nOnly", {0.70, 0.85});
  bc2.AppendBarGroup<double>("Read\\nOnly", {2.38, 2.64});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {2.69, 3.17});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {0.53, 0.64});
  
  bc2.AppendBarName("Independently Alloc'ed");
	bc2.AppendBarName("Pre-Alloc'ed");

	bc2.SetLegendFlag(false);
	bc2.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc2.SetYUpperLimitMinimum(5.0);

	bc2.Draw("microindex-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email Microindex
	BarChart bc3{};
	
	bc3.AppendBarGroup<double>("Insert\\nOnly", {0.49, 0.56});
  bc3.AppendBarGroup<double>("Read\\nOnly", {1.36, 1.44});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {1.53, 1.68});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {0.4, 0.49});
  
  bc3.AppendBarName("Independently Alloc'ed");
	bc3.AppendBarName("Pre-Alloc'ed");

	bc3.SetLegendFlag(false);
	bc3.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc3.SetYUpperLimitMinimum(5.0);

	bc3.Draw("microindex-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory Microindex
	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("Mono\\nInt", {1.060, 1.115});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.590, 1.615});
  bc4.AppendBarGroup<double>("Email\\nKey", {3.195, 3.270});
  
  bc4.AppendBarName("Independently Alloc'ed");
	bc4.AppendBarName("Pre-Alloc'ed");

  bc4.SetYAxisLabel("Memory (GB)");

	bc4.SetLegendFlag(false);

	bc4.Draw("microindex-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeGC() - Plot Bwtree GC result
 */
void PlotBwTreeGC() {
  LineChart lc1{};
  
  lc1.AppendXValueList<double>({1,5,10,15,20});
  
	lc1.AppendYValueList<double>({2.37, 5.34, 8.12, 12.06, 12.37});
	lc1.AppendYValueList<double>({3.18, 7.65, 19.57, 22.58, 24.38});

	lc1.AppendLineName("Centralized GC");
	lc1.AppendLineName("Distributed GC");

	lc1.SetYAxisLabel("Throughput (Million Ops/Sec)");

	lc1.SetLegendFlag(false);
	lc1.SetLegendVerticalFlag(false);

	lc1.Draw("gc-mono.pdf");
	
	///////////////////////////////////////////////////////////////////
	
	// GC random
	LineChart lc2{};
  
  lc2.AppendXValueList<double>({1,5,10,15,20});
  
	lc2.AppendYValueList<double>({2.15, 6.48, 10.37, 11.26, 11.43});
	lc2.AppendYValueList<double>({3.17, 10.01, 19.73, 23.01, 27.1});

	lc2.AppendLineName("Centralized GC");
	lc2.AppendLineName("Distributed GC");

	lc2.SetLegendFlag(false);
	lc2.SetYAxisLabel("Throughput (Million Ops/Sec)");
	lc2.SetLegendVerticalFlag(false);

	lc2.Draw("gc-rand.pdf");
	
	///////////////////////////////////////////////////////////////////
	
	// GC email
	LineChart lc3{};
  
  lc3.AppendXValueList<double>({1,5,10,15,20});
  
  lc3.AppendYValueList<double>({1.32, 4.98, 7.29, 8.06, 9.17});
  lc3.AppendYValueList<double>({1.68, 6.43, 11.85, 13.33, 15.48});

	lc3.AppendLineName("Centralized GC");
	lc3.AppendLineName("Distributed GC");

	lc3.SetLegendFlag(false);
	lc3.SetYAxisLabel("Throughput (Million Ops/Sec)");
	lc3.SetLegendVerticalFlag(false);

	lc3.Draw("gc-email.pdf");
	
	///////////////////////////////////////////////////////////////////
	// GC memory
  
  return;
}

/*
 * PlotBwTreeCompare() - Plots data used in BwTree paper
 */
void PlotBwTreeCompare() {
  BarChart bc1{};
  
	bc1.AppendBarGroup<double>("Insert\\nOnly", {2.61, 4.10, 8.09, 13.3, 11.06});
  bc1.AppendBarGroup<double>("Read\\nOnly", {3.23, 3.18, 4.74, 4.38, 12.93});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {2.37, 2.96, 4.20, 3.85, 10.46});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {0.56, 0.81, 0.06, 1.98, 0.5});
  
	bc1.AppendBarName("BwTree");
	bc1.AppendBarName("OpenBwTree");
	bc1.AppendBarName("MassTree");
	bc1.AppendBarName("BTree");
  bc1.AppendBarName("ART");
  
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc1.SetYUpperLimitMinimum(18.0);

	bc1.SetLegendFlag(false);
	bc1.SetYGridFlag(true);

	bc1.Draw("compare-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Compare
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("Insert\\nOnly", {0.63, 0.85, 1.76, 1.51, 3.24});
  bc2.AppendBarGroup<double>("Read\\nOnly", {2.34, 2.64, 4.10, 4.70, 9.83});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {2.15, 3.17, 3.66, 4.10, 9.98});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {0.48, 0.64, 0.05, 1.98, 0.29});

	bc2.AppendBarName("BwTree");
	bc2.AppendBarName("OpenBwTree");
	bc2.AppendBarName("MassTree");
	bc2.AppendBarName("BTree");
  bc2.AppendBarName("ART");

	bc2.SetLegendFlag(false);
	bc2.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc2.SetYUpperLimitMinimum(18.0);
	bc2.SetYGridFlag(true);

	bc2.Draw("compare-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email compare
	BarChart bc3{};

	bc3.AppendBarGroup<double>("Insert\\nOnly", {0.47, 0.56, 1.16, 0.92, 2.22});
  bc3.AppendBarGroup<double>("Read\\nOnly", {1.41, 1.44, 3.39, 2.10, 6.26});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {1.32, 1.68, 2.79, 1.95, 5.41});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {0.37, 0.49, 0.05, 0.73, 0.21});

  bc3.AppendBarName("BwTree");
	bc3.AppendBarName("OpenBwTree");
	bc3.AppendBarName("MassTree");
	bc3.AppendBarName("BTreeLOC");
  bc3.AppendBarName("ART");
	
	bc3.SetLegendFlag(false);
	bc3.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc3.SetYUpperLimitMinimum(18.0);
	bc3.SetYGridFlag(true);

	bc3.Draw("compare-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Compare
	BarChart bc4{};

	bc4.AppendBarGroup<double>("Mono\\nInt", {1.095, 1.115, 2.558, 0.887, 1.125});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.580, 1.615, 2.728, 0.902, 1.125});
  bc4.AppendBarGroup<double>("Email\\nKey", {3.250, 3.270, 5.255, 2.746, 2.890});

  bc4.AppendBarName("BwTree");
	bc4.AppendBarName("OpenBwTree");
	bc4.AppendBarName("MassTree");
	bc4.AppendBarName("BTreeLOC");
  bc4.AppendBarName("ART");

  bc4.SetYAxisLabel("Memory (GB)");

	bc4.SetLegendFlag(false);
	bc4.SetYGridFlag(true);

	bc4.Draw("compare-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeMTCompare() - Plots data used in BwTree paper
 */
void PlotBwTreeMTCompare() {
  BarChart bc1{};

	bc1.AppendBarGroup<double>("Insert\\nOnly", {11.25, 25.69, 20.21, 37.08, 59.32});
  bc1.AppendBarGroup<double>("Read\\nOnly",   {12.58, 42.42, 46.55, 30.39, 89.42});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {12.37, 24.38, 30.50, 22.07, 66.11});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {7.47, 10.8, 0.82, 22.32, 5.42});
  
	bc1.AppendBarName("BwTree");
	bc1.AppendBarName("OpenBwTree");
	bc1.AppendBarName("MassTree");
	bc1.AppendBarName("BTree");
  bc1.AppendBarName("ART");
	
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc1.SetYUpperLimitMinimum(110.0);

	bc1.SetLegendFlag(false);
	bc1.SetYGridFlag(true);

	bc1.Draw("mt-compare-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Rand	Compare
	BarChart bc2{};

	bc2.AppendBarGroup<double>("Insert\\nOnly", {9.84, 11.01, 20.36, 9.13, 30.33});
  bc2.AppendBarGroup<double>("Read\\nOnly",   {12.09, 35.69, 43.50, 61.62, 82.16});
  bc2.AppendBarGroup<double>("Read\\nUpdate", {11.43, 27.10, 29.48, 43.78, 42.21});
  bc2.AppendBarGroup<double>("Scan\\nInsert", {6.12, 7.04, 0.74, 20.42, 3.28});

  bc2.AppendBarName("BwTree");
	bc2.AppendBarName("OpenBwTree");
	bc2.AppendBarName("MassTree");
	bc2.AppendBarName("BTree");
  bc2.AppendBarName("ART");

	bc2.SetLegendFlag(false);
	bc2.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc2.SetYUpperLimitMinimum(110.0);
	bc2.SetYGridFlag(true);

	bc2.Draw("mt-compare-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email compare
	BarChart bc3{};

	bc3.AppendBarGroup<double>("Insert\\nOnly", {7.71, 8.28, 11.03, 7.80, 21.71});
  bc3.AppendBarGroup<double>("Read\\nOnly",   {10.24, 16.1, 33.62, 23.19, 55.77});
  bc3.AppendBarGroup<double>("Read\\nUpdate", {9.17, 15.48, 20.04, 20.98, 30.62});
  bc3.AppendBarGroup<double>("Scan\\nInsert", {4.46, 4.7, 0.71, 8.10, 2.31});

  bc3.AppendBarName("BwTree");
	bc3.AppendBarName("OpenBwTree");
	bc3.AppendBarName("MassTree");
	bc3.AppendBarName("BTree");
  bc3.AppendBarName("ART");
	
	bc3.SetLegendFlag(false);
	bc3.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc3.SetYUpperLimitMinimum(110.0);
	bc3.SetYGridFlag(true);

	bc3.Draw("mt-compare-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Compare
	BarChart bc4{};

	bc4.AppendBarGroup<double>("Mono\\nInt", {1.505, 1.539, 2.65, 0.853, 1.24});
  bc4.AppendBarGroup<double>("Rand\\nInt", {1.705, 1.757, 2.53, 0.850, 1.23});
  bc4.AppendBarGroup<double>("Email\\nKey", {3.590, 3.685, 6.250, 1.824, 2.59});

  bc4.AppendBarName("BwTree");
	bc4.AppendBarName("OpenBwTree");
	bc4.AppendBarName("MassTree");
	bc4.AppendBarName("BTree");
  bc4.AppendBarName("ART");

  bc4.SetYAxisLabel("Memory (GB)");

	bc4.SetLegendFlag(false);
	bc4.SetYUpperLimitMinimum(8.0);
	bc4.SetYGridFlag(true);

	bc4.Draw("mt-compare-memory.pdf");
	
	return;
}

void PlotBwTreeSummary() {
  BarChart bc1{};
  
  bc1.AppendBarGroup<double>("Single\\nThread", {2.15, 2.17, 2.63, 3.17, 3.11});
  bc1.AppendBarGroup<double>("20 Worker\\nThreads", {11.43, 17.68, 20.57, 22.91, 21.45});
  bc1.AppendBarName("BwTree");
  bc1.AppendBarName("+GC");
  bc1.AppendBarName("+PA");
  bc1.AppendBarName("+FC \\& SS");
  bc1.AppendBarName("+NK");
  
  /*
  bc1.AppendBarGroup<double>("BwTree", {2.15, 11.43});
  bc1.AppendBarGroup<double>("+ GC", {2.17, 17.68});
  bc1.AppendBarGroup<double>("+ PA", {2.43, 20.57});
  bc1.AppendBarGroup<double>("+ FC \\& SS", {2.86, 22.91});
  bc1.AppendBarGroup<double>("+ NK", {2.74, 21.45});
  
  bc1.AppendBarName("Single\\nThread");
  bc1.AppendBarName("20 Worker\\nThreads");
	*/
	
	bc1.SetYAxisLabel("Throughput (Million Ops/Sec)");
	bc1.SetYUpperLimitMinimum(28.0);

	bc1.SetLegendFlag(true);
	bc1.SetYGridFlag(true);

	bc1.Draw("bwtree-summary.pdf");
	
  return; 
}

void PlotBwTreeSummary2() {
  BarChart bc1{};
  
  /*
  bc1.AppendBarGroup<double>("BwTree", {11.25, 12.58, 12.37, 7.47});
  bc1.AppendBarGroup<double>("OpenBwTree", {26.41, 42.48, 26.36, 10.80});
  bc1.AppendBarName("Insert Only");
  bc1.AppendBarName("Read Only");
  bc1.AppendBarName("Read Update");
  bc1.AppendBarName("Scan Insert");
  */

  bc1.AppendBarGroup<double>("Insert\\nOnly", {11.25, 25.69});
  bc1.AppendBarGroup<double>("Read\\nOnly", {12.58, 42.42});
  bc1.AppendBarGroup<double>("Read\\nUpdate", {12.37, 24.38});
  bc1.AppendBarGroup<double>("Scan\\nInsert", {7.47, 10.80});
  
  bc1.AppendBarName("BwTree");
  bc1.AppendBarName("OpenBwTree");
	
	bc1.SetYAxisLabel("Throughput (Million Ops/Sec)");
	bc1.SetYUpperLimitMinimum(52.0);

	bc1.SetLegendFlag(true);
	bc1.SetYGridFlag(true);

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

	lc1.SetYAxisLabel("Throughput (Million Ops/Sec)");
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

	lc2.SetYAxisLabel("Throughput (Million Ops/Sec)");
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

	bc1.AppendBarGroup<double>("", {11.17, 17.13, 25.89, 29.91, 18.17});
  
	bc1.AppendBarName("BwTree");
	bc1.AppendBarName("OpenBwTree");
	bc1.AppendBarName("MassTree");
	bc1.AppendBarName("BTree");
  bc1.AppendBarName("ART");
	
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");
	bc1.SetYUpperLimitMinimum(40.0);

	bc1.SetLegendFlag(true);
	bc1.SetYGridFlag(true);

	bc1.Draw("mt-ht-mono-insert.pdf");

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
  PlotBwTreeMTCompare();
  PlotBwTreeSummary();
  PlotBwTreeSummary2();
  
	// This is plotted using Python script
  //PlotBwTreeConfig();

	PlotHighContentionMT();
  
	return 0;
}
