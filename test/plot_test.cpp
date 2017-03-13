
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
  
  bc1.AppendBarGroup<double>("IO", {2.77, 3.15});
  bc1.AppendBarGroup<double>("RO", {2.13, 2.49});
  bc1.AppendBarGroup<double>("RU", {3.08, 3.27});
  bc1.AppendBarGroup<double>("SI", {0.62, 0.81});
  
  bc1.AppendBarName("Independently Alloc'ed");
	bc1.AppendBarName("Pre-Alloc'ed");
	
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");

	bc1.SetLegendFlag(false);

	bc1.Draw("preallocation-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Mono	Preallocation
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("IO", {0.78, 0.94});
  bc2.AppendBarGroup<double>("RO", {2.09, 2.46});
  bc2.AppendBarGroup<double>("RU", {2.72, 3.07});
  bc2.AppendBarGroup<double>("SI", {0.51, 0.64});
  
  bc2.AppendBarName("Independently Alloc'ed");
	bc2.AppendBarName("Pre-Alloc'ed");

	bc2.SetLegendFlag(false);

	bc2.Draw("preallocation-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email	Preallocation
	BarChart bc3{};
	
	bc3.AppendBarGroup<double>("IO", {0.52, 0.61});
  bc3.AppendBarGroup<double>("RO", {1.23, 1.42});
  bc3.AppendBarGroup<double>("RU", {1.3, 1.55});
  bc3.AppendBarGroup<double>("SI", {0.39, 0.45});
  
  bc3.AppendBarName("Independently Alloc'ed");
	bc3.AppendBarName("Pre-Alloc'ed");

	bc3.SetLegendFlag(false);

	bc3.Draw("preallocation-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Preallocation
	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("RO", {1.055, 1.115});
  bc4.AppendBarGroup<double>("RU", {1.580, 1.615});
  bc4.AppendBarGroup<double>("SI", {3.195, 3.270});
  
  bc4.AppendBarName("Independently Alloc'ed");
	bc4.AppendBarName("Pre-Alloc'ed");

  bc4.SetYAxisLabel("Memory Footprint (GB)");

	bc4.SetLegendFlag(false);

	bc4.Draw("preallocation-memory.pdf");
	
	return;
}

/*
 * PlotBwTreeMicroIndex() - Plots data used in BwTree paper
 */
void PlotBwTreeMicroIndex() {
  BarChart bc1{};
  
  bc1.AppendBarGroup<double>("IO", {2.79, 3.15});
  bc1.AppendBarGroup<double>("RO", {2.08, 2.49});
  bc1.AppendBarGroup<double>("RU", {3.01, 3.27});
  bc1.AppendBarGroup<double>("SI", {0.57, 0.81});
  
  bc1.AppendBarName("Independently Alloc'ed");
	bc1.AppendBarName("Pre-Alloc'ed");
	
	bc1.SetYAxisLabel("Throughput (Millon Ops/Sec)");

	bc1.SetLegendFlag(false);

	bc1.Draw("microindex-mono.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Mono	Preallocation
	BarChart bc2{};
	
	bc2.AppendBarGroup<double>("IO", {0.77, 0.94});
  bc2.AppendBarGroup<double>("RO", {2.11, 2.46});
  bc2.AppendBarGroup<double>("RU", {2.81, 3.07});
  bc2.AppendBarGroup<double>("SI", {0.48, 0.64});
  
  bc2.AppendBarName("Independently Alloc'ed");
	bc2.AppendBarName("Pre-Alloc'ed");

	bc2.SetLegendFlag(false);

	bc2.Draw("microindex-rand.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Email Preallocation
	BarChart bc3{};
	
	bc3.AppendBarGroup<double>("IO", {0.50, 0.61});
  bc3.AppendBarGroup<double>("RO", {1.23, 1.42});
  bc3.AppendBarGroup<double>("RU", {1.31, 1.55});
  bc3.AppendBarGroup<double>("SI", {0.32, 0.45});
  
  bc3.AppendBarName("Independently Alloc'ed");
	bc3.AppendBarName("Pre-Alloc'ed");

	bc3.SetLegendFlag(false);

	bc3.Draw("microindex-email.pdf");
	
	///////////////////////////////////////////////////////////////////

  // Memory	Preallocation
	BarChart bc4{};
	
	bc4.AppendBarGroup<double>("RO", {1.060, 1.115});
  bc4.AppendBarGroup<double>("RU", {1.590, 1.615});
  bc4.AppendBarGroup<double>("SI", {3.195, 3.270});
  
  bc4.AppendBarName("Independently Alloc'ed");
	bc4.AppendBarName("Pre-Alloc'ed");

  bc4.SetYAxisLabel("Memory Footprint (GB)");

	bc4.SetLegendFlag(false);

	bc4.Draw("microindex-memory.pdf");
	
	return;
}


int main() {
	ColorTest();
	PrintListTest();
	RoundUpTest();
	//BarPlotTest();
	//LinePlotTest();
	
	PlotBwTreePreallocation();
  PlotBwTreeMicroIndex();
  
	return 0;
}
